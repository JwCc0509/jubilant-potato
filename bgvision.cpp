#include "bgvision.h"
#include <QtDebug>
//#include "cvMat2Qimage.h"
#include <QDir>
#include <QQueue>
#include "CEasyCapS.h"
#include "CEasyCapSEx.h"
//#include "load_gentl_cti.h"
#include <QMessageBox>
#include <QRandomGenerator>
#include <stdio.h>
#include <stdlib.h>
#include "mw_base_converter.h"
#include <QMutex>
#ifndef DELETE_ARRAY
#define DELETE_ARRAY( x ) \
if (NULL != x)      \
    {                   \
            delete[] x;     \
            x = nullptr;       \
    }
#endif
//extern QQueue<Mat>  qImageBuffer;
extern   ulong     framecount;
extern Mat   qImageBuffer;
extern bool    newFrame;
std::mutex  CameraMatLock;
typedef_camera_pixfmt bg_cam_pixel=Mono8;
int  breakNumber=0;


int SaveMat2Pic(cv::Mat MatTemp,ulong framecount,char savetype=0)
{
    int res=0 ;
    mw_converter::raw_to_file_param_t param;
    param.pixelfmt = (uint32_t)(mw_converter::r_mono8);
    param.p_data = (uint8_t*)MatTemp.data;
    param.width = (uint32_t)MatTemp.cols;
    param.height = (uint32_t)MatTemp.rows;
    param.data_len =(uint64_t)MatTemp.cols*MatTemp.rows;
    param.conv_file_type = mw_converter::conv_opencv_bmp;
    if(savetype==1)
    {
        std::string str = "D:\\pic\\" + std::to_string(framecount) + ".bmp";
        qDebug()<<"save :"<<str;
        res = mw_conv_raw_to_file(&param, str.c_str());
    }
    else  if(savetype==2)
    {
        param.conv_file_type = mw_converter::conv_raw;
        std::string str = "D:\\pic\\" + std::to_string(framecount) + ".raw";
        res = mw_conv_raw_to_file(&param, str.c_str());
    }

    else
    {
        param.conv_file_type = mw_converter::conv_tiff;
        std::string str = "D:\\pic\\" + std::to_string(framecount) + ".tiff";
        res = mw_conv_raw_to_file(&param, str.c_str());
    }


    return res;
}
static  void callback_get_data(void* p_cb_contex, GenTL::BUFFER_HANDLE h_buf, void* p_base, size_t size)
{
    if (p_cb_contex == NULL)
        return;
    BgVisionCamera *BgCamera =(BgVisionCamera*) p_cb_contex;
   // qDebug()<<"the gentl addr:"<<p_cb_contex<<"---BUFFER_HANDLE:"<<h_buf<<"---p_base:"<<p_base;
    //qDebug()<<"-";
    Q_UNUSED(size);
    if (p_base)
    {
        cxp_buf_info_t buf_info;
        memset(&buf_info, 0, sizeof(cxp_buf_info_t));

        if (scap_get_buf_info(BgCamera->h_device, h_buf, &buf_info, true))
        {
            framecount++;
            BgCamera->BgFrameWidth=	buf_info.width;		//宽
            BgCamera->BgFrameHeight=buf_info.height; 			//高
            if (buf_info.p_base != NULL) {
                CameraMatLock.lock();
                if(bg_cam_pixel==BGR8)
                {
                    qImageBuffer = cv::Mat(buf_info.height , buf_info.width , CV_8UC3, (void *)buf_info.p_base); //h_,w_是图像长宽
                    qImageBuffer.copyTo(BgCamera->BgFrameBuffer) ;
                }
                else  if(bg_cam_pixel==Mono8)
                {
                    qImageBuffer = cv::Mat(buf_info.height , buf_info.width , CV_8UC1, (void *)buf_info.p_base);
                    qImageBuffer.copyTo(BgCamera->BgFrameBuffer) ;
                }
                else  if(bg_cam_pixel==Mono10||bg_cam_pixel==Mono12)
                {
                    qImageBuffer = cv::Mat(buf_info.height , buf_info.width , CV_16UC1, (void *)buf_info.p_base);
                    qImageBuffer.copyTo(BgCamera->BgFrameBuffer) ;
                }
                CameraMatLock.unlock();
                //演示存成tiff
                //SaveMat2Pic(qImageBuffer,framecount);
                scap_back_buffer(BgCamera->h_device, h_buf);
                newFrame=true;
            }
            else {
                qDebug()<<"the frame data is null ";
            }
        }
    }
}

 GC_ERROR create_user_fifo(SCAP_HANDLE h_cap, sim_user_data_t* p_user_data_array, uint32_t buf_num, size_t specify_buf_size)
 {
    bool malloc_success = true;
    if (buf_num > 0 && p_user_data_array !=NULL)
    {
        //get payload_size
        if (specify_buf_size == 0)
        {
            cxp_ds_info_t ds_info;
            if (!scap_get_data_stream_info(h_cap, &ds_info))
            {
                return GC_ERR_NOT_INITIALIZED;
            }
            specify_buf_size = ds_info.payload_size;
        }
        //malloc buffer
        for (uint32_t i = 0; i < buf_num; i++)
        {
            p_user_data_array[i].addr = malloc(specify_buf_size);
          //  p_user_data_array[i].addr =(void*) (new(std::nothrow) char[specify_buf_size]);
            p_user_data_array[i].size = specify_buf_size;
            p_user_data_array[i].index = i;
            if (p_user_data_array[i].addr == NULL)
            {
                malloc_success = false;
                break;
            }
        }
        if (!malloc_success)
        {
            for (uint32_t i = 0; i < buf_num; i++)
            {

                if (NULL != p_user_data_array[i].addr  )
                {

                    free(p_user_data_array[i].addr)  ;
                        p_user_data_array[i].addr = nullptr;
                }
            }
            return GC_ERR_OUT_OF_MEMORY;
        }
        return GC_ERR_SUCCESS;
    }
    else
    {
        return GC_ERR_INVALID_PARAMETER;
    }
 }

 void  destroy_user_fifo(sim_user_data_t* p_user_data_array, uint32_t buf_num)
 {
    if (p_user_data_array)
    {
        for (uint32_t i = 0; i < buf_num; i++)
        {
            free(p_user_data_array[i].addr);
            p_user_data_array[i].addr = nullptr;

        }
    }
    return ;
 }


BgVisionCamera::BgVisionCamera(BgCxpCard *ptCxpCard)  //:mgCxpCard(ptCxpCard)
{

    mgCxpCard=ptCxpCard;
    CardName=mgCxpCard->GetCardName( );
    FrameCount=0;
}
BgVisionCamera::BgVisionCamera( )  //:mgCxpCard(ptCxpCard)
{
    qDebug()<<"the card name"<<mgCxpCard->GetCardName( ) ;
    FrameCount=0;
}

BgVisionCamera::~BgVisionCamera()
{
    BgFrameBuffer.release();
}


uint32_t  BgVisionCamera::GetDeviceNum()
{
    uint32_t DeviceNum=0;
    DeviceNum=scap_get_device_num(mgCxpCard->GetCardHandle(0),0);
    return DeviceNum;

}

cxp_std_features_t  BgVisionCamera::GetCamInfo(void)
{
    return CamInfo;
}


/*  如果张卡，接一个相机，DevIdx为0,如果接多个相机，DevIdx为相机在采集卡中的序号。
 *  一张卡，最多接四个相机， DevIdx的范围为0--3,
 *  0为打开device0，1为device1, 2为device2, 3为device3
 * uint32_t DevIdx
 */
int  BgVisionCamera::OpenBgCamera(uint32_t DevIdx)
{

    if(    LineCameraOn==false)
    {

        /*
           2,链接相机获取操作句柄，加载配置文件，或者配置参数；
               -相机操作资源，常用2层：采集卡设备层（scap_device）、远程相机层（scap_camera）

        */
        qDebug()<<"上层传过来的设备号："<<DevIdx;
        uint32_t DeviceNum=GetDeviceNum();
        CurrDevIdx=DevIdx;
        if(DeviceNum==0)
        {
            return -1;
        }
        if(DevIdx>=DeviceNum)
        {
            return -1;
        }

        h_device= scap_create_by_camera((CARD_HANDLE)mgCxpCard->GetCardHandle(0), DevIdx);
        qDebug()<<"get the camera handle"<<h_device;
       // GetCameraXml();
        if(h_device ==NULL)
        {
            QMessageBox::warning(0,"警告","创建设备失败！");
            qDebug()<<"scap device is faill:"<<DevIdx;
            return -1;
        }
        bool b_res = false;
        LineCameraOn=true;

        b_res = scap_set_param_by_file(h_device, scap_device, ConfigDeviceFileName.c_str() );// 设备层
        if(!b_res)
        {
            qDebug()<<"write "<<ConfigDeviceFileName<<"error";
        }  else
        {
            qDebug()<<"write "<<ConfigDeviceFileName<<"sucess";
        }
        b_res=scap_set_param_by_file(h_device, scap_camera, ConfigCameraFileName.c_str());//相机层，请用客户端保存相机配置，在这里替换掉配置文件。这里以mik相机配置文件举例
        if(!b_res)
        {
            qDebug()<<"write "<<ConfigCameraFileName<<"error";
        }  else
        {
            qDebug()<<"write "<<ConfigCameraFileName<<"sucess";
        }


        char CamScanType[128] = { 0 };
        size_t id_size = sizeof(CamScanType);
        scap_get_string(h_device, scap_device, "OperationMode", CamScanType, &id_size);

        int64_t  linescanbuffheight=0;
        if (strcmp(CamScanType, "TDI") == 0)
        {
            qDebug()<<"the camera is tdi mode:";
        }
        else
        {
            qDebug()<<"the camera is tdi mode:";
        }

        b_res =scap_get_integer(h_device, scap_camera, "Width", &linescanbuffheight);
        if(!b_res)
            qDebug()<<"get image  width error";
        else
        {
            BgVisionCamera::BgFrameWidth=linescanbuffheight;
        }
		

        if(!scap_set_string(h_device, scap_camera, "FPNSelector", "FPN0"))
            qDebug()<<"set FPNSelector FPN0 error";

        if(!scap_set_command(h_device, scap_camera,"FPNLoad"))
            qDebug()<<"FPNLoad command error";

        if(!scap_set_string(h_device, scap_camera, "FPNEnable", "true"))
            qDebug()<<"set FPNEnable  error";


        if(!scap_set_string(h_device, scap_camera, "PRNUSelector", "PRNU0"))
            qDebug()<<"set PRNUSelector PRNU0 error";

        if(!scap_set_command(h_device, scap_camera,"PRNULoad"))
            qDebug()<<"PRNULoad command error";

        if(!scap_set_string(h_device, scap_camera, "PRNUEnable", "true"))
            qDebug()<<"set PRNUEnable PRNU0 error";




        scap_get_camera_info(h_device, &CamInfo,0);

        if(strcmp(CamInfo.format_str, "Mono8") == 0)
        {
            bg_cam_pixel=Mono8;
        }

        else if(strcmp(CamInfo.format_str, "Mono10") == 0)
        {

            bg_cam_pixel=Mono10;
        }

        else if(strcmp(CamInfo.format_str, "Mono12") == 0)
        {
            bg_cam_pixel=Mono12;
        }

        else if(strcmp(CamInfo.format_str, "RGB8") == 0)
        {
            bg_cam_pixel=BGR8;
        }
        else
        {
            bg_cam_pixel=BGR8;
        }

        buffer_num = 1;
        user_array = new(std::nothrow) sim_user_data_t[buffer_num];
        if (!user_array)
        {
            qDebug()<<"Malloc failed\n";
            return -1 ;
        }
        else
        {
            memset(user_array, 0, sizeof(sim_user_data_t)* buffer_num); //init 0
        }
        //3.1 create buffer
        create_user_fifo(h_device, user_array, buffer_num, 0);//可以申请一个比较大的size，涵盖可能变化的buffer大小，比如：create_user_fifo(h_cap0, user_array, buffer_num, 5150*5150*2);
        qDebug()<<"creat buffer";
        //3.2 register user buffer mode
        scap_register_user_buffer(h_device, user_array, buffer_num);
        qDebug()<<"the h_device addr:"<<h_device;
        StartCap();

        return 0;
    }else
    {
        return -1;

    }


}

void BgVisionCamera::closeBgCamera( )
{

    if(CameraStreamOn==true)
    {
        CameraStreamOn=false;
        StopCap( );
    }
    if(LineCameraOn==true)
    {
        qDebug()<<"close camera ";
        scap_unregister_user_buffer(h_device);
       // destroy_user_fifo(user_array, buffer_num);
        DELETE_ARRAY(user_array);
        scap_close_by_camera(h_device);//关掉相机
        LineCameraOn=false;
    }

}
void BgVisionCamera::signal_ConfigDevice(std::string ConfigDevice )
{
    BgVisionCamera::ConfigDeviceFileName=ConfigDevice ;
}

void BgVisionCamera::signal_ConfigCamera(std::string ConfigCameraFileName)
{
    BgVisionCamera::ConfigCameraFileName=ConfigCameraFileName;
}


void BgVisionCamera::StartCap( )
{
    if(CameraStreamOn==false)    {
        CameraStreamOn=true;        
        scap_start_capture(h_device , callback_get_data,  this, buffer_num, -1, true);//申请缓存16帧， 采集10帧结束（或设置-1一直采集）,false参数代表自动处理buf的排队
    }
}

void BgVisionCamera::StopCap( )
{
    if(newFrame==true)    {
        newFrame=false;
    }
    if(CameraStreamOn==true)    {
        scap_stop_capture(h_device );
        CameraStreamOn=false;
    }
}

void BgVisionCamera::SetBgCameraFeatures( )
{
    if(FrameCount%2)
    {
        if(!scap_set_command(h_device, scap_camera,"AcquisitionStop"))
            qDebug()<<"AcquisitionStop error";

        if(!scap_set_string(h_device, scap_camera, "ScanDirection", "Forward"))
            qDebug()<<"set ScanDirection Forward error";

        if(!scap_set_string(h_device, scap_data_stream, "CameraTapGeometry", "Geometry_1X_1Y"))
            qDebug()<<"set CameraTapGeometry Geometry_1X_1Y error";

        if(!scap_set_command(h_device, scap_camera,"AcquisitionStart"))
            qDebug()<<"AcquisitionStart error";

       // qDebug()<<"run ScanDirection Forward----";
    }
    else
    {
        if(!scap_set_command(h_device, scap_camera,"AcquisitionStop"))
            qDebug()<<"AcquisitionStop";

        if(!scap_set_string(h_device, scap_camera, "ScanDirection", "Reverse"))
            qDebug()<<"set ScanDirection Reverse error";

        if(!scap_set_string(h_device, scap_data_stream, "CameraTapGeometry", "Geometry_1X_1YE"))
            qDebug()<<"set CameraTapGeometry Geometry_1X_1YE error";

        if(!scap_set_command(h_device, scap_camera,"AcquisitionStart"))
            qDebug()<<"AcquisitionStart  error";

       // qDebug()<<"run ScanDirection Reverse******";
    }
}


