#-------------------------------------------------
#
# Project created by QtCreator 2013-05-30T14:48:51
#
#-------------------------------------------------

QT       += core gui openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = CameraDemo
TEMPLATE = app
QT       += xml
CONFIG += c++17
CONFIG += console

QMAKE_PROJECT_DEPTH = 0

SOURCES += main.cpp\
            cvMat2Qimage.cpp\
            bgvision.cpp\
            camthread.cpp \
            openglwidget.cpp\
            logout.cpp\
            cxpCard.cpp\
          #  load_gentl_cti.cpp\
            mainwindow.cpp

HEADERS  += mainwindow.h\
            cvMat2Qimage.h\
            camthread.h \
            openglwidget.h\
            bgvision.h\
            logout.h\
            cxpCard.h\
   #         load_gentl_cti.h\
   #         load_gentl_cti.h\

            mythread.h


FORMS += \
    mainwindow.ui


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/OpenCV/Lib/release/ -lopencv_world452
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/OpenCV/Lib/debug/ -lopencv_world452d
else:unix: LIBS += -L$$PWD/OpenCV/Lib/ -lopencv_world452d

INCLUDEPATH += $$PWD/OpenCV/Includes
DEPENDPATH += $$PWD/OpenCV/Includes



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/magewelsdk/lib/ -lCEasyCapS  -lmw_base_converter
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/magewelsdk/lib/ -lCEasyCapS  -lmw_base_converter

INCLUDEPATH += $$PWD/magewelsdk/include
DEPENDPATH += $$PWD/magewelsdk/include
