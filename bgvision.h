#ifndef _BGVISION_H_
#define _BGVISION_H_
#include<QQueue>
#include "opencv2/opencv.hpp"
#include<QString>
#include "CEasyCapS.h"
#include "cxpCard.h"
using namespace std;
using namespace cv;

typedef enum _typedef_camera_status
{
    STOP = 0,
    START = 1,
    WAIT = 2,
    PAUSE =3,
    SETFEATURE=4,
    SAVEPIC=5,
}typedef_camera_status;

typedef enum _typedef_camera_pixfmt
{
    BGR8 =0,
    Mono8 = 1,
    Mono10 = 2,
    Mono12 = 3,

}typedef_camera_pixfmt;




class BgVisionCamera
{

public:
/***************************
 *   class data
 ***************************/
    QQueue<Mat>  BgFrmBufQue;
    int  BgFrmBufQueLen=1;
    Mat  BgFrameBuffer;
    int  BgFrameWidth=0;
    int  BgFrameHeight=0;
    bool CameraStreamOn=false;
    bool LineCameraOn=false;
    std::string CardName;
    typedef_camera_status  CameraStutas;
    sim_user_data_t* user_array=NULL;
    uint32_t buffer_num=0;
    LONGLONG  FrameCount=0;
    std::string ConfigDeviceFileName;
    std::string  ConfigCameraFileName;
    PORT_HANDLE h_device=NULL;
    cxp_std_features_t CamInfo;
    uint32_t CurrDevIdx=0;
/****************************************
 * class method
****************************************/

    int  OpenBgCamera(uint32_t DevIdx=0);
    void closeBgCamera( );
    uint32_t  GetDeviceNum();
    void StartCap( );
    void StopCap( );
    void SetBgCameraFeatures( );
    void GetBgCameraFeatures();
    cxp_std_features_t   GetCamInfo(void);
    bool GetDeviceXml(char *DeviceXml,long *DeviceXmlSize);
    // bool  GetCameraXml(char *CameraXml,long *CameraXmlSize);
    bool  GetCameraXml(void );
    BgVisionCamera(BgCxpCard *ptCxpCard);
    BgVisionCamera( );
    virtual ~BgVisionCamera();

public slots:

signals:
    void signal_ConfigDevice(std::string ConfigDeviceFileName);
    void signal_ConfigCamera(std::string ConfigCameraFileName);

private:
    BgCxpCard  *mgCxpCard ;

};





#endif//_MV_CAMERA_H_
