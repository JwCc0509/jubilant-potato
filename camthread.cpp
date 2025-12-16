#include "camthread.h"
//#include "cvMat2Qimage.h"
#include <QImageWriter>
#include <QFile>
#include <QDir>

extern typedef_camera_pixfmt bg_cam_pixel;


CameraThread::CameraThread(QObject *parent)
    : QThread{parent}
{
    myImage = new QImage();
}

CameraThread::~CameraThread()
{
    delete myImage;
    if(cameraPtr == NULL)
    {
        delete cameraPtr;
    }
    if(imagePtr == NULL)
    {
        delete imagePtr;
    }
}

void CameraThread::getCameraPtr(BgVisionCamera *camera)
{
    cameraPtr = camera;
}

void CameraThread::soltSavePic( )
{
    qDebug()<<"solt save picture";
    tSavePic = true;
}
void CameraThread::getImagePtr(cv::Mat *image)
{
    imagePtr = image;
}

cv::Mat  qImageBuffer;
extern bool newFrame;

extern int SaveMat2Pic(cv::Mat MatTemp,ulong framecount);
std::mutex  CameraLock;
void CameraThread::run()
{
    if(cameraPtr == NULL)
    {

        return;
    }
    if(imagePtr == NULL)
    {

        return;
    }

    while(!isInterruptionRequested())
    {

        /***
         *
        使用回调函数的方法图片图片
        ***/
        if(cameraPtr->CameraStutas==START)
        {

            if(newFrame==true)
            {

                newFrame=false;
                cameraPtr->FrameCount++;
                //qDebug()<<"Capture Frame:"<<cameraPtr->FrameCount;
                if(qImageBuffer.empty())
                {
                    qDebug()<<"the mat is empty";
                    return ;
                }
                //SaveMat2Pic(qImageBuffer,cameraPtr->FrameCount);
                CameraLock.lock();
               // qImageBuffer.copyTo(cameraPtr->BgFrameBuffer) ;
                CameraLock.unlock();

                QImage ImageTemp;
                size_t targetW=cameraPtr->BgFrameBuffer.cols/2;
                size_t targetH=cameraPtr->BgFrameBuffer.rows/2;
                cv::Mat showImage ;
                resize(cameraPtr->BgFrameBuffer,showImage,cv::Size(targetW, targetH));
                qDebug()<<"imageW:"<<targetW <<"imageH:"<<targetH ;
                if(cameraPtr->BgFrameBuffer.empty())
                    qDebug()<<"the buffer is empty";

                if(bg_cam_pixel==BGR8)
                {
                    ImageTemp= QImage((const uchar*)(showImage.data),
                                       showImage.cols,showImage.rows, showImage.step,
                                       QImage::Format_RGB888 );
                }
                else  if(bg_cam_pixel==Mono8)
                {
                    ImageTemp= QImage( (const uchar*)(showImage.data),
                                       showImage.cols,showImage.rows, showImage.step,
                                       QImage::Format_Grayscale8 );
                }
                else  if(bg_cam_pixel==Mono10||bg_cam_pixel==Mono12)
                {
                    ImageTemp= QImage((const uchar*)(showImage.data),
                                       showImage.cols,showImage.rows, showImage.step,
                                       QImage::Format_Grayscale16 );
                }
                //ImageTemp=ImageTemp.convertToFormat(QImage::Format_RGBA8888);
                *myImage=ImageTemp.convertToFormat(QImage::Format_RGBA8888).copy();
                qDebug()<<"send imageW:"<<myImage->width()<<"iamgeH:"<<myImage->height();
                emit signal_messImage(*myImage);
                /*
                 * 在open camera后，要start cap，在此处等待新的图片后再进入下一个流程
                   */
                cameraPtr->CameraStutas=STOP  ;
            }


        }
        if(cameraPtr->CameraStutas==STOP)
        {

            cameraPtr->StopCap( );
            cameraPtr->CameraStutas=SETFEATURE;

        }
        if(cameraPtr->CameraStutas==SETFEATURE)
        {

            cameraPtr->CameraStutas=WAIT;
           // cameraPtr->SetBgCameraFeatures( );
        }
        if(cameraPtr->CameraStutas==WAIT)
        {

           // if( tSavePic==true)
           // {
           //     cameraPtr->CameraStutas=SAVEPIC;
           //     qDebug()<<"save picture is true";
           //     tSavePic=false;
           // }
           // else{
           //     cameraPtr->CameraStutas=START;
           //     cameraPtr->StartCap( );
           // }

            cameraPtr->CameraStutas=START;
            cameraPtr->StartCap( );
        }
        if(cameraPtr->CameraStutas==SAVEPIC)
        {
            qDebug()<<"stutas is WAIT ";
            QString filePath;
            QString fileName = "image-" + QString::number(cameraPtr->FrameCount) + ".jpeg";
            qDebug()<<"save pic"<<fileName;
            //filePath =  "./imagesave/" + fileName;
            filePath =  fileName;
            QImageWriter writer(filePath);
            writer.setQuality(100);
            //writer.setFormat("PNG"); // 指定图像格式
            writer.setFormat("JPEG"); // 指定图像格式
            writer.write(*myImage);
            cameraPtr->CameraStutas=START;
            cameraPtr->StartCap( );
        }
        msleep(10);
    }
}
