#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMenuBar>
#include <QGraphicsEllipseItem>
#include <QLabel>
#include <QStatusBar>
#include <QMouseEvent>
#include "bgvision.h"
#include "camthread.h"
#include <QTreeWidgetItem>
#include <qfile.h>
#include <QTimer>
#include <QRadioButton>
#include <qtreeview.h>
#include <QDomElement>
#include <QTreeWidgetItem>
#include <QComboBox>
#include  "openglwidget.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:

    uint32_t  DeviceNumber=0;
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

   // void initScene();            			//初始化场景
    void createActions();       			//创建主窗体的所有动作
    void createMenus();         			//创建主窗体的菜单栏
    void resizeEvent(QResizeEvent *event);

signals:
    void signal_SavePic(bool savePic);
public slots:
    void slotNew();              			//新建一个显示窗体
    void slotClear();           			//清除场景中所有的图元
    void slotOpenCard( );
    void slotOpenCam( );        		//在场景中加入一个透明蝴蝶图片
    void slotCloseCam();
    void slotStartCap( );         		//
    void slotStopCap( );
    void slotPauseCap( );
    void slotContinusCap();
    void slotOpenCam1( );

    void  updateframerate(  );

    void slotOpenFile() ;
    void slotOpenConfigDeviceFile(std::string *DeviceFlie);
    void slotOpenConfigCameraFile(std::string *CameraFlie);

    void slotShowImage(QImage CurImage) ;
    void slotSetImageVale( );
    void slotSaveImage( );
    void slotSaveButton( );
    void onZoomChanged(float zoom);
    // void do_mouseMovePoint(QPoint point);    //鼠标移动
    // void do_mouseClicked(QPoint point);      //鼠标单击
    // void do_mouseDoubleClick(QPoint point);  //鼠标双击
    // void do_keyPress(QKeyEvent *event);      //按键
    // void do_mouseWheel(int angleDetla);      //滚轮
    // void do_mouseRightClicked(QPoint point); //鼠标右键
    // virtual void contextMenuEvent(QContextMenuEvent *event);

private:
    Ui::MainWindow  *ui;

    QTreeWidget *CameraFeture;

    //QList<QTreeWidgetItem *> CameraFeture;
    QAction *newAct;
    QAction *openFileAct;
    QAction *clearAct;
    QAction *exitAct;
    QAction *addOpenCardAct;
    QAction *addOpenCamAct;
    QAction *addOpenCam1Act;
    QAction *addCloseCamAct;
    QAction *addStartCapAct;
    QAction *addStopCapAct;
    QAction *addPauseCapAct;
    QAction *add1SecCapAct;
    QAction *savepicAct;
    QAction *setbreakAct;
    QAction *addGetXmlAct;

    QAction *addOpenCamAct1;
    QAction *addCloseCamAct1;
    QAction *addStartCapAct1;
    QAction *addStopCapAct1;
    QAction *addPauseCapAct1;
    QAction *add1SecCapAct1;
    QAction *savepicAct1;
    QAction *setbreakAct1;

    // QAction *addOpenCamAct2;
    // QAction *addCloseCamAct2;
    // QAction *addStartCapAct2;
    // QAction *addStopCapAct2;
    // QAction *addPauseCapAct2;
    // QAction *add1SecCapAct2;
    // QAction *savepicAct2;
    // QAction *setbreakAct2;

    // QAction *addOpenCamAct3;
    // QAction *addCloseCamAct3;
    // QAction *addStartCapAct3;
    // QAction *addStopCapAct3;
    // QAction *addPauseCapAct3;
    // QAction *add1SecCapAct3;
    // QAction *savepicAct3;
    // QAction *setbreakAct3;
    OpenGLWidget *m_openGLWidget;

    QLabel  *labViewCord;       //用于状态栏
    QLabel  *labSceneCord;
    QLabel  *labItemCord;
    QLabel  *labItemInfo;
    QTimer  *tim;
    QGraphicsPixmapItem *CameraItem;
    QString  AppName;
    // image
    QImage  *ShowImage=new QImage();
    Mat     *myImage = NULL;
    int     ImageValue=8;
    bool    CamIsOn=false;
    bool    TimerOn=false;
    bool    StreamIsOn=false;
    bool    CamItemIsMove=false;

    bool    SavePic=false;
    BgCxpCard   *ptCxpCard = NULL ;

    BgVisionCamera   *m_pcMyCamera = NULL ;
    BgVisionCamera   *m_pcMyCamera1 = NULL ;
    CameraThread    *mCamThread = NULL;
};

#endif // MAINWINDOW_H
