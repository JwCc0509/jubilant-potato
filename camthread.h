#ifndef CAMTHREAD_H
#define CAMTHREAD_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QDir>
#include <QDateTime>
#include <QDebug>

#include "bgvision.h"

class CameraThread : public QThread
{
    Q_OBJECT

public:
    bool  tSavePic=false;
    explicit CameraThread(QObject *parent = nullptr);
     ~CameraThread();
    bool m_bCalicationFlag=0;
    void run();
    void getCameraPtr(BgVisionCamera *camera);
    void getImagePtr(cv::Mat *image);

signals:
    void signal_messImage(QImage myImage);
public slots:
    void soltSavePic( );

private:
    bool  FrameBuffLock=false;

    BgVisionCamera *cameraPtr = NULL;
    cv::Mat *imagePtr = NULL;
    QImage *myImage = NULL;


};

#endif // MYTHREAD_H
