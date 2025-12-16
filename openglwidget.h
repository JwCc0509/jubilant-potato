#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QImage>
#include <QMouseEvent>
#include <QWheelEvent>
//#include <QMatrix4x4>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget();

    bool loadImage(const QString &imagePath);
    void fitToWindow();
    void actualSize();

    // 图片操作
    void setZoom(float zoom);
    void setPan(float dx, float dy);
    void  updateTransform();
    bool  ShowImage(QImage &imagedata);

public slots:
    void zoomIn();
    void zoomOut();

signals:
    void imageLoaded(bool success);
    void zoomChanged(float zoom);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // 鼠标事件处理
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mouseDoubleClickEvent(class QMouseEvent *event) override;


private:
    void setupShaders();
    void setupGeometry();

    QOpenGLTexture *m_texture;
    QOpenGLShaderProgram *m_shaderProgram;
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLVertexArrayObject m_vao;

    QImage m_image;

    // 变换参数
    float m_zoom;
    float m_panX, m_panY;
    float m_movespeed;

    float m_imageAspectRatio;

    // 鼠标交互
    bool m_dragging;
    QPoint m_lastMousePos;

    // 着色器变量位置
    int m_matrixLoc;
    int m_textureLoc;

    bool m_initialized;
};

#endif // OPENGLWIDGET_H
