#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsItemAnimation>
#include <QRandomGenerator>
#include <QTimeLine>
#include <QFileDialog>
#include <QImageWriter>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QFontDialog>
#include <QQueue>
#include <QSlider>
#include <QTreeWidgetItem>
#include "cxpCard.h"
#include <qtreeview.h>
#include <QDomElement>
#include <QVBoxLayout>
#include "openglwidget.h"


//QQueue<Mat>  qImageBuffer;
//Mat     qImageBuffer;
bool    newFrame=false;
ulong     framecount=0;
ulong     oldframecount=0;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

   // QString AppName="bg-test  frame rate:";
    //qInstallMessageHandler(LogOut);
    ui->setupUi(this);

  //  qDebug()<<"open device is success"<<ptCxpCard->GetDeviceHandle(0);
    createActions();                    	//创建主窗体的所有动作
    createMenus();

        //创建主窗体的菜单栏
    labViewCord=new QLabel("View 坐标："); //创建状态栏上的标签
    labViewCord->setMinimumWidth(150);
    ui->statusbar->addWidget(labViewCord);

    labSceneCord=new QLabel("Scene 坐标：");
    labSceneCord->setMinimumWidth(150);
    ui->statusbar->addWidget(labSceneCord);

    labItemCord=new QLabel("Item 坐标：");
    labItemCord->setMinimumWidth(150);
    ui->statusbar->addWidget(labItemCord);

    labItemInfo=new QLabel("ItemInfo: ");
    labItemInfo->setMinimumWidth(200);
    ui->statusbar->addWidget(labItemInfo);

    QTimer frameupdate;
    QTimer *timer = new QTimer(this);
    timer->setInterval(1000); // 设置超时间隔为1000毫秒，即1秒
    connect(timer, &QTimer::timeout, this, &MainWindow::updateframerate);
    timer->start();
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建布局
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // 创建OpenGL部件
    m_openGLWidget = new OpenGLWidget(this);
    mainLayout->addWidget(m_openGLWidget, 1);

    ShowImage->load("image.png");
    if(ShowImage->isNull())
    {

        qDebug()<<"Load image is faill";

    }

    this->setMouseTracking(true);
    resize(1280,720);
    setWindowTitle(tr("Camera Test"));
    connect(m_openGLWidget, &OpenGLWidget::zoomChanged, this, &MainWindow::onZoomChanged);
    CamIsOn = false;
}

MainWindow::~MainWindow()
{
    slotClear();
    if(CamIsOn ==true)
    {
        slotCloseCam();
    }

    if(m_pcMyCamera!=NULL)
        delete m_pcMyCamera ;
    delete ShowImage;
    if(ptCxpCard)
        delete ptCxpCard;
}


void MainWindow::updateframerate(void )
{
    if (framecount<0)
        framecount=0;
    QString  qFrame;
    qFrame=qFrame.asprintf("framerate:%d----total frame:%d",framecount-oldframecount,framecount);
    setWindowTitle(AppName+qFrame);
    oldframecount=framecount;
    if(CamIsOn==true)
    {

        if(!m_pcMyCamera )
        {
            if(  (m_pcMyCamera->CameraStreamOn==true)&&(m_pcMyCamera->LineCameraOn==false) )
            {
                QString  image_hei ;
                image_hei=image_hei.asprintf("图像：%d x %d",
                       m_pcMyCamera ->BgFrameWidth,
                       m_pcMyCamera ->BgFrameHeight);
                labItemInfo->setText(image_hei);
                qDebug()<<"updateframerate print image_hei:"<<image_hei;
            }

        }
    }
}



void MainWindow::createActions()       	//创建主窗体的所有动作
{
    newAct = new QAction(tr("新建窗口"),this);
    openFileAct = new QAction(tr("打开文件"),this);
    clearAct = new QAction(tr("清除"),this);
    savepicAct = new QAction(tr("保存图像"),this);
    setbreakAct = new QAction(tr("设置相机break"),this);
    exitAct = new QAction(tr("退出"),this);
    addOpenCardAct = new QAction(tr("打开采集卡"),this);
    addOpenCamAct = new QAction(tr("打开相机0"),this);
    addOpenCam1Act = new QAction(tr("打开相机1"),this);
    addCloseCamAct = new QAction(tr("关闭相机"),this);
    addPauseCapAct = new QAction(tr("暂停/开始"),this);
    add1SecCapAct = new QAction(tr("定时采集"),this);
    addStartCapAct = new QAction(tr("开始采集"),this);
    addStopCapAct = new QAction(tr("关闭采集"),this);
   // addGetXmlAct = new QAction(tr("获取相机xml"),this);
    addOpenCam1Act->setEnabled(false);
    addOpenCamAct->setEnabled(false);
    addCloseCamAct->setEnabled(false);
    addStartCapAct->setEnabled(false);
    addStopCapAct->setEnabled(false);
    addPauseCapAct->setEnabled(false);
    add1SecCapAct->setEnabled(false);


    connect(newAct,SIGNAL(triggered()),this,SLOT(slotNew()));
    connect(openFileAct,SIGNAL(triggered()),this,SLOT(slotOpenFile()));
    connect(clearAct,SIGNAL(triggered()),this,SLOT(slotClear()));


    connect(exitAct,SIGNAL(triggered()),this,SLOT(close()));
    connect(addOpenCardAct,SIGNAL(triggered()),this,SLOT (slotOpenCard()));
    connect(addOpenCamAct,SIGNAL(triggered()),this,SLOT (slotOpenCam()));
   // connect(addGetXmlAct,SIGNAL(triggered()),this,SLOT (slotGetXml()));

    connect(addOpenCam1Act,SIGNAL(triggered()),this,SLOT (slotOpenCam1()));
    connect(addCloseCamAct,SIGNAL(triggered()),this,SLOT (slotCloseCam()));
    connect(addStartCapAct,SIGNAL(triggered()),this,SLOT (slotStartCap()));
    connect(addStopCapAct,SIGNAL(triggered()),this,SLOT (slotStopCap()));
    connect(addPauseCapAct,SIGNAL(triggered()),this,SLOT (slotPauseCap()));
    connect(add1SecCapAct,SIGNAL(triggered()),this,SLOT (slotContinusCap()));


    connect(exitAct,SIGNAL(triggered()),this,SLOT(close()));

}

void MainWindow::slotSaveButton( )
{

    if(mCamThread!=NULL)
        mCamThread->tSavePic=true;
}


void MainWindow::createMenus()         	//创建主窗体的菜单栏
{
    QMenu *fileMenu = menuBar()->addMenu(tr("文件"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openFileAct);
    fileMenu->addAction(clearAct);
    fileMenu->addAction(savepicAct);

    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

  //  DeviceNumber=ptCxpCard ->GetDeviceNumber();
  //  qDebug()<<"creat menu find device:"<<DeviceNumber;


    QMenu *itemsMenu = menuBar()->addMenu(tr("相机"));
    itemsMenu->addAction(addOpenCardAct);
    itemsMenu->addAction(addOpenCamAct);
    itemsMenu->addAction(addOpenCam1Act);
    itemsMenu->addAction(addCloseCamAct);
    itemsMenu->addAction(addStartCapAct);
    itemsMenu->addAction(addStopCapAct);
  //  itemsMenu->addAction(addGetXmlAct);

}


void MainWindow::slotNew()        		//新建一个显示窗体
{
    slotClear();


    MainWindow *newWin = new MainWindow;
    newWin->show();
}




void MainWindow::slotOpenFile()        		//
{
    QString fileName =QFileDialog::getOpenFileName(this,tr("打开"),"D:" ,
          tr("JPEG Images (*.jpeg *.jpg);"
             ";BMP Images (*.bmp);"
             ";PNG Images (*.png);"
             ";All Files (*)")  );

    if(!fileName.isEmpty())
    {
       // ShowImage->load(fileName);
        if (m_openGLWidget->loadImage(fileName)) {
            statusBar()->showMessage("已加载图片: " + fileName);
        } else {
            QMessageBox::warning(this, "错误", "无法加载图片: " + fileName);
            statusBar()->showMessage("加载图片失败: " + fileName);
        }
    }


}
void MainWindow::onZoomChanged(float zoom)
{
   statusBar()->showMessage(QString("缩放: %1%").arg(qRound(zoom * 100)));
}
void MainWindow::slotOpenConfigDeviceFile(std::string *DeviceFlie)
{
    QString fileName =QFileDialog::getOpenFileName(this,tr("打开"),"D:" ,tr("json(*device.json)")  );
    if(!fileName.isEmpty())
    {
        *DeviceFlie=fileName.toStdString();
    }

}
void MainWindow::slotOpenConfigCameraFile(std::string *CameraFlie)
{
    QString fileName =QFileDialog::getOpenFileName(this,tr("打开"),"D:" ,tr("json(*camera.json)")  );

    if(!fileName.isEmpty())
    {
        *CameraFlie=fileName.toStdString();
    }

}
void MainWindow::slotShowImage(QImage CurImage)
{
    if(CurImage.isNull())
        return;

    // if(!ShowImage->isNull())
    // {
    //    *ShowImage=CurImage.copy();

    // }
    // else{
    //     qDebug()<<"ShowImage is null";
    //     return;
    // }
    m_openGLWidget->ShowImage(CurImage);
}


void MainWindow::slotClear()        	//清除场景中所有的图元
{

}

void MainWindow::slotOpenCard( )
{

    qDebug()<<"now start init the card";
    ptCxpCard=new BgCxpCard;

    if(!ptCxpCard->BgCardCreat())
    {
        addOpenCamAct->setEnabled(false);
        addCloseCamAct->setEnabled(false);
        addOpenCam1Act->setEnabled(false);
        addStartCapAct->setEnabled(false);
        addStopCapAct->setEnabled(false);
        addPauseCapAct->setEnabled(false);
        add1SecCapAct->setEnabled(false);
        qDebug()<<"open grabber interface is fail";

    }
    else
    {
        addOpenCamAct->setEnabled(true);
        addOpenCam1Act->setEnabled(true);
        addCloseCamAct->setEnabled(false);
        addStartCapAct->setEnabled(false);
        addStopCapAct->setEnabled(false);
        addPauseCapAct->setEnabled(false);
        add1SecCapAct->setEnabled(false);
    }

    qDebug()<<"open card is success"<<ptCxpCard->GetCardHandle(0);
}
void MainWindow::slotOpenCam( )
{
    if(CamIsOn == false)
    {
        qDebug()<<"start open the device:";
        qDebug()<<"the  card class address:"<<ptCxpCard;
        m_pcMyCamera  = new BgVisionCamera(ptCxpCard);
        uint32_t DeviceNum=0;
        DeviceNum=m_pcMyCamera ->GetDeviceNum();
        if(DeviceNum>0)
            qDebug()<<"have %d  device"<<DeviceNum;
        myImage = new Mat();
        if(myImage==NULL)
        {
            qDebug()<<"image creat fail";
            return;
        }
        //qDebug()<<myImage;
        mCamThread = new CameraThread();
        if(mCamThread==NULL)
        {
            qDebug()<<"thread creat fail";
            return;
        }
        //  connect(ui->graphicsView,&camGraphicsView::mouseWheel,this, &MainWindow::do_mouseWheel);
        QMessageBox::warning(this,"警告","先选择采集卡设备参数文件 ，后选择远程相机参数文件");
        connect(mCamThread,SIGNAL(signal_messImage(QImage)),this,SLOT(slotShowImage(QImage) ));
       // connect(savepicAct,SIGNAL(triggered()),myThread,SLOT(soltSavePic()));

        slotOpenConfigDeviceFile(&m_pcMyCamera ->ConfigDeviceFileName);
        slotOpenConfigCameraFile(&m_pcMyCamera ->ConfigCameraFileName);

        int nRet ;
        uint32_t DevIndex=0;
        nRet = m_pcMyCamera ->OpenBgCamera(DevIndex );

        if(0 != nRet)
        {
            delete m_pcMyCamera ;
            QMessageBox::warning(this,"警告","打开失败！");
            return;
        }
        //m_pcMyCamera->SetFrameQueLen(200);
        mCamThread->getCameraPtr(m_pcMyCamera );
        mCamThread->getImagePtr(myImage);
        CamIsOn = true;

        if(!mCamThread->isRunning())
        {
            mCamThread->start();
        }
        m_pcMyCamera->CameraStutas=START;
        addOpenCamAct->setEnabled(false);
        addCloseCamAct->setEnabled(true);
        addStartCapAct->setEnabled(false);
        addStopCapAct->setEnabled(false);
        addPauseCapAct->setEnabled(false);
        add1SecCapAct->setEnabled(false);
        Q_ASSERT(m_pcMyCamera  != nullptr);
    }
    else
    {
        if(mCamThread->isRunning())
        {
            mCamThread->requestInterruption();
            mCamThread->wait();
            if(m_pcMyCamera )
            {
                m_pcMyCamera ->closeBgCamera( );
                CamIsOn = false;
            }
        }

    }
}

void MainWindow::slotOpenCam1( )
{
    if(CamIsOn == false)
    {
        qDebug()<<"start open the device:";
        qDebug()<<"the  card class address:"<<ptCxpCard;
        m_pcMyCamera  = new BgVisionCamera(ptCxpCard);
        uint32_t DeviceNum=0;
        DeviceNum=m_pcMyCamera ->GetDeviceNum();
        if(DeviceNum>0)
            qDebug()<<"have %d  device"<<DeviceNum;
        myImage = new Mat();
        if(myImage==NULL)
        {
            qDebug()<<"image creat fail";
            return;
        }
        //qDebug()<<myImage;
        mCamThread = new CameraThread();
        if(mCamThread==NULL)
        {
            qDebug()<<"thread creat fail";
            return;
        }
        //  connect(ui->graphicsView,&camGraphicsView::mouseWheel,this, &MainWindow::do_mouseWheel);
        QMessageBox::warning(this,"警告","先选择采集卡设备参数文件 ，后选择远程相机参数文件");
        connect(mCamThread,SIGNAL(signal_messImage(QImage)),this,SLOT(slotShowImage(QImage) ));
        connect(savepicAct,SIGNAL(triggered()),mCamThread,SLOT(soltSavePic()));

        slotOpenConfigDeviceFile(&m_pcMyCamera ->ConfigDeviceFileName);
        slotOpenConfigCameraFile(&m_pcMyCamera ->ConfigCameraFileName);

        int nRet ;
        uint32_t DevIndex=1;
        nRet = m_pcMyCamera ->OpenBgCamera(DevIndex );

        if(0 != nRet)
        {
            delete m_pcMyCamera ;
            QMessageBox::warning(this,"警告","打开失败！");
            return;
        }
        //m_pcMyCamera->SetFrameQueLen(200);
        mCamThread->getCameraPtr(m_pcMyCamera );
        mCamThread->getImagePtr(myImage);
        CamIsOn = true;

        if(!mCamThread->isRunning())
        {
            mCamThread->start();
        }
        m_pcMyCamera->CameraStutas=START;
        addOpenCamAct->setEnabled(false);
        addCloseCamAct->setEnabled(true);
        addStartCapAct->setEnabled(false);
        addStopCapAct->setEnabled(false);
        addPauseCapAct->setEnabled(false);
        add1SecCapAct->setEnabled(false);
        Q_ASSERT(m_pcMyCamera  != nullptr);
    }
    else
    {
        if(mCamThread->isRunning())
        {
            mCamThread->requestInterruption();
            mCamThread->wait();
            if(m_pcMyCamera )
            {
                m_pcMyCamera ->closeBgCamera( );
                CamIsOn = false;
            }
        }

    }
}

void MainWindow::slotCloseCam()
{
    if( CamIsOn == true)
    {
        if(mCamThread->isRunning())
        {
            mCamThread->requestInterruption();
            mCamThread->wait();
            if(m_pcMyCamera )
            {
                m_pcMyCamera ->closeBgCamera( );
                CamIsOn = false;
                addOpenCamAct->setEnabled(true  );
                addCloseCamAct->setEnabled(false);
                addStartCapAct->setEnabled(false);
                addStopCapAct->setEnabled(false);
                addPauseCapAct->setEnabled(false);
                add1SecCapAct->setEnabled(false);
            }
        }

    }
    else{
        qDebug()<<"camera was closed";
    }

}


void MainWindow::slotStartCap()
{
    m_pcMyCamera ->StartCap( );
    addOpenCamAct->setEnabled(false);
    addCloseCamAct->setEnabled(true);
    addStartCapAct->setEnabled(false);
    addStopCapAct->setEnabled( true);
    addPauseCapAct->setEnabled(true);
    add1SecCapAct->setEnabled(true);
}
void MainWindow::slotPauseCap()
{

    if(m_pcMyCamera ==NULL)
    {
        qDebug()<<"m_pcMyCamera is null";
    }
    else
    {
        setWindowTitle("Camera Test__Tricount:"+QString::number(m_pcMyCamera ->FrameCount)  );
        qDebug()<<"camera soft triggered";
    }
}


void MainWindow::slotContinusCap()
{
    if(TimerOn==false)
    {
        TimerOn=true;
        tim->start();

        addOpenCamAct->setEnabled(false);
        addCloseCamAct->setEnabled(false);
        addStartCapAct->setEnabled(false);
        addStopCapAct->setEnabled( false);
        addPauseCapAct->setEnabled(false);
        add1SecCapAct->setEnabled(true);
    }
    else
    {
        TimerOn=false;
        tim->stop();

        addOpenCamAct->setEnabled(false);
        addCloseCamAct->setEnabled(false);
        addStartCapAct->setEnabled(false);
        addStopCapAct->setEnabled( true);
        addPauseCapAct->setEnabled(true);
        add1SecCapAct->setEnabled(true);
    }
}
void MainWindow::slotStopCap()
{
    m_pcMyCamera ->StopCap( );
    addOpenCamAct->setEnabled(false);
    addCloseCamAct->setEnabled(true);
    addStartCapAct->setEnabled(true);
    addStopCapAct->setEnabled(false);
}
//鼠标移动，point是视图的坐标,物理坐标
// void MainWindow::do_mouseMovePoint(QPoint point)
// {
//     labViewCord->setText(QString::asprintf(
//         "View ：%d,%d", point.x(),point.y()));

//     // labSceneCord->setText(
//     //     QString::asprintf(
//     //     "Scene ：%.0f,%.0f : ", pointScene.x(),pointScene.y()
//     //             )
//     //      );
// }

// //鼠标单击事件
// void MainWindow::do_mouseClicked(QPoint point)
// {
//     // QPointF pointScene=ui->OpenGLWidget ->mapToScene(point);     //转换到Scene坐标
//     // QGraphicsItem  *item=NULL;
//     // item=scene->itemAt(pointScene,ui->OpenGLWidget ->transform());   //获取光标下的图形项

//     // if (item != NULL)       //有图形项
//     // {
//     //     QPointF pointItem=item->mapFromScene(pointScene); //转换为图形项的局部坐标
//     //     qDebug()<<QString::asprintf(
//     //         "Item坐标：%.0f,%.0f ",pointItem.x(),pointItem.x()
//     //          );
//     //     qreal x=pointItem.x();
//     //     qreal y=pointItem.y();

//     //     QRgb  pixelvaule=CameraItem->pixmap().toImage().pixel(
//     //         x,
//     //         y);
//     //     qDebug()<<QString::asprintf(
//     //         "Item坐标：%.0f,%.0f 红色值：%.0d  绿色值：%.0d  蓝色值：%.0d",x,y,
//     //         qRed(pixelvaule),qGreen(pixelvaule),qBlue(pixelvaule) );
//     //     labItemCord->setText(QString::asprintf(
//     //         "Item坐标：%.0f,%.0f 红色值：%.0d  绿色值：%.0d  蓝色值：%.0d",x,y,
//     //             qRed(pixelvaule),qGreen(pixelvaule),qBlue(pixelvaule) )
//     //          );

//     // }
// }

// void MainWindow::do_mouseRightClicked(QPoint point)
// {
//     // QPointF pointScene=ui->OpenGLWidget ->mapToScene(point);     //转换到Scene坐标
//     // QGraphicsItem  *item=NULL;
//     // item=scene->itemAt(pointScene,ui->OpenGLWidget ->transform());   //获取光标下的图形项
//     // if (item != NULL)       //有图形项
//     // {
//     //   QPointF pointItem=item->mapFromScene(pointScene); //转换为图形项的局部坐标
//     //   labItemCord->setText(QString::asprintf(
//     //       "right Item 坐标：%.0f,%.0f",pointItem.x(),pointItem.y()));
//     //   if(ShowImage->isNull())
//     //       return;
//     // }
// }
void MainWindow::slotSaveImage( )
{
     QString dateTime = QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss");
     // 将文件名设置为当前时间
     QString fileName = "image-" + dateTime + ".png";
     // 打开文件对话框以选择文件保存路径
     QString filePath = QFileDialog::getSaveFileName(nullptr, "Save Image", fileName, "Images (*.png *.bmp *.jpg)");
     if (!filePath.isEmpty()) {
         // 创建目录
         QString directory = QFileInfo(filePath).dir().path() ;
         QDir dir;
         if(!dir.exists(directory))
         {
             if(QDir().mkpath(directory))
             {
                 qDebug()<<"文件夹创建失败"  ;
             }
         }
        // // 在目录中保存图像到文件
         filePath = directory + "/" + fileName;
         QImageWriter writer(filePath);
         writer.setFormat("PNG"); // 指定图像格式
         writer.write(*ShowImage);

       }

}
void MainWindow::slotSetImageVale( )
{

   // intMat2Char();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{

    event->accept();
}



//鼠标双击事件,调用相应的对话框，设置填充颜色、线条颜色或字体
// void MainWindow::do_mouseDoubleClick(QPoint point)
// {
//     // QPointF pointScene=ui->OpenGLWidget ->mapToScene(point);    //转换到Scene坐标
//     // QGraphicsItem  *item=NULL;
//     // item=scene->itemAt(pointScene,ui->OpenGLWidget ->transform());    //获取光标下的图形项

//     // if (item == NULL)
//     //     return;
//     // if(CamItemIsMove==false)
//     // {
//     //     CamItemIsMove=true;
//     //     item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
//     // }
//     // else{
//     //     CamItemIsMove=false;
//     //     item->setFlags(QGraphicsItem::ItemIsSelectable  );

//     // }

// }


// void MainWindow::do_mouseWheel(int angleDetla)
// {

//     // if (scene->selectedItems().count()!=1)
//     // {
//     //     qDebug()<<"没有选中视图";
//     //     return;     //没有选中的图形项，或选中的多于1个
//     // }

//     // QGraphicsItem   *item=scene->selectedItems().at(0);
//     // if (item == NULL)
//     //     return;

//     // if (angleDetla>0)  //放大
//     // {
//     //     qreal scaleimage;
//     //     scaleimage=0.1+item->scale();
//     //     if(scaleimage>50.0)
//     //         scaleimage=50.0;
//     //     if(scaleimage<0.1)
//     //         scaleimage=0.1;
//     //     item->setScale(scaleimage);
//     // }
//     // else   //缩小
//     // {
//     //     qreal scaleimage;
//     //     scaleimage=-0.1+item->scale();
//     //     if(scaleimage>50.0)
//     //         scaleimage=50.0;
//     //     if(scaleimage<0.1)
//     //         scaleimage=0.1;
//     //     item->setScale(scaleimage);

//     // }
//     return ;

// }

// //按键事件
// void MainWindow::do_keyPress(QKeyEvent *event)
// {
//     // if (scene->selectedItems().count()!=1)
//     // {
//     //     qDebug()<<"没有选中视图";
//     //     return;     //没有选中的图形项，或选中的多于1个
//     // }

//     // QGraphicsItem   *item=scene->selectedItems().at(0);

//     // if (event->key()==Qt::Key_Delete)       //删除
//     // {
//     //     //scene->removeItem(item);
//     //     //qDebug()<<"press delete key";
//     // }
//     // else if (event->key()==Qt::Key_Space)   //顺时针旋转90度
//     // {
//     //     QPointF centerPos=item->boundingRect().center();
//     //     item->setTransformOriginPoint(centerPos);
//     //     item->setRotation(90+item->rotation());
//     //     //qDebug()<<"press space key";
//     // }
//     // else if (event->key()==Qt::Key_PageUp)  //放大
//     // {
//     //     item->setScale(0.1+item->scale());
//     //     //qDebug()<<"press pageup key";
//     // }
//     // else if (event->key()==Qt::Key_PageDown)//缩小
//     // {
//     //     item->setScale(-0.1+item->scale());
//     //     //qDebug()<<"press pagedown key";
//     // }
//     // else if (event->key()==Qt::Key_Left)    //左移
//     // {
//     //     item->setX(-1+item->x());
//     //     //qDebug()<<"press left key";
//     // }
//     // else if (event->key()==Qt::Key_Right)   //右移
//     // {
//     //     item->setX(1+item->x());
//     //     //qDebug()<<"press right key";
//     // }
//     // else if (event->key()==Qt::Key_Up)      //上移
//     // {
//     //     item->setY(-1+item->y());
//     //     //qDebug()<<"press up key";
//     // }
//     // else if (event->key()==Qt::Key_Down)    //下移
//     // {
//     //     item->setY(1+item->y());
//     //     //qDebug()<<"press down key";
//     // }
// }


// void MainWindow::contextMenuEvent(QContextMenuEvent *event)
// {
//     Q_UNUSED(event)

//     // QAction  *addSaveImageAct = new QAction(tr("保存图像"),this);
//     // QAction  *addSetValueAct = new QAction(tr("设置值"),this);
//     // QMenu *RightButtonMenu = new QMenu(this);
//     // //QMenu *RightButtonMenu = menuBar()->addMenu(tr("右键"));

//     // RightButtonMenu->addAction(addSaveImageAct);
//     // RightButtonMenu->addAction(addSetValueAct);
//     // RightButtonMenu->exec(QCursor::pos());

//     // if(myThread){

//     //     connect(
//     //         addSaveImageAct,  &QAction::triggered,
//     //         myThread, &MyThread::soltSavePic   );
//     //     qDebug()<<"myThread exist";
//     // }
//     // else{

//     //     qDebug()<<"myThread is null";
//     // }

//     // connect(addSetValueAct,SIGNAL(triggered()),this,SLOT (slotSetImageVale()));

// }

