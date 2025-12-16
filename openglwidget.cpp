#include "openglwidget.h"
#include <QDebug>
#include <QFileInfo>

// 顶点着色器源码
const char *vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec2 aPos;
    layout(location = 1) in vec2 aTexCoord;

    out vec2 TexCoord;

    uniform mat4 transform;

    void main()
    {
        gl_Position = transform * vec4(aPos, 0.0, 1.0);
        TexCoord = aTexCoord;
    }
)";

// 片段着色器源码
const char *fragmentShaderSource = R"(
    #version 330 core
    in vec2 TexCoord;
    out vec4 FragColor;

    uniform sampler2D ourTexture;

    void main()
    {
        FragColor = texture(ourTexture, TexCoord);
    }
)";

// 顶点数据 (位置坐标 + 纹理坐标)
float vertices[] = {
    // 位置          // 纹理坐标
    1.0f,  1.0f,   1.0f, 1.0f,  // 右上
    1.0f, -1.0f,   1.0f, 0.0f,  // 右下
    -1.0f, -1.0f,   0.0f, 0.0f,  // 左下


    -1.0f,  1.0f,   0.0f, 1.0f   // 左上
};

// 索引数据
unsigned int indices[] = {
    0, 1, 3,  // 第一个三角形
    1, 2, 3   // 第二个三角形
};

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_texture(nullptr)
    , m_shaderProgram(nullptr)
    , m_zoom(1.0f)
    , m_panX(0.0f)
    , m_panY(0.0f)
    , m_imageAspectRatio(1.0f)

    , m_dragging(false)
    , m_matrixLoc(-1)
    , m_textureLoc(-1)

{
    m_vertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
}

OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();
    delete m_texture;
    delete m_shaderProgram;
    m_vertexBuffer.destroy();
    doneCurrent();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // 设置清屏颜色
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // 启用混合（用于透明纹理）
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 设置着色器
    setupShaders();
    setupGeometry();
}

void OpenGLWidget::setupShaders()
{
    m_shaderProgram = new QOpenGLShaderProgram(this);

    // 编译顶点着色器
    if (!m_shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource)) {
        qWarning() << "顶点着色器编译错误:" << m_shaderProgram->log();
        return;
    }

    // 编译片段着色器
    if (!m_shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource)) {
        qWarning() << "片段着色器编译错误:" << m_shaderProgram->log();
        return;
    }

    // 链接着色器程序
    if (!m_shaderProgram->link()) {
        qWarning() << "着色器程序链接错误:" << m_shaderProgram->log();
        return;
    }

    // 绑定着色器程序
    if (!m_shaderProgram->bind()) {
        qWarning() << "着色器程序绑定失败";
        return;
    }

    // 获取uniform位置
    m_matrixLoc = m_shaderProgram->uniformLocation("transform");
    m_textureLoc = m_shaderProgram->uniformLocation("ourTexture");
}

void OpenGLWidget::setupGeometry()
{
    // 创建VAO
    m_vao.create();
    m_vao.bind();

    // 创建VBO
    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vertices, sizeof(vertices));

    // 设置顶点属性指针
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 创建EBO
    QOpenGLBuffer ebo(QOpenGLBuffer::IndexBuffer);
    ebo.create();
    ebo.bind();
    ebo.allocate(indices, sizeof(indices));

    m_vao.release();

}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    update();
}


void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (!m_shaderProgram || !m_texture) {
        return;
    }


    m_shaderProgram->bind();
    m_vao.bind();
    m_texture->bind(0);

    // 设置纹理单元
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_texture->setMagnificationFilter(QOpenGLTexture::Nearest);
    m_shaderProgram->setUniformValue(m_textureLoc, 0);

    // 计算变换矩阵
    updateTransform();

    // 绘制三角形
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    m_vao.release();
    m_shaderProgram->release();

}

void OpenGLWidget::updateTransform()
{
    QMatrix4x4 transform;

    // 应用平移
    transform.translate(m_panX, m_panY);

    // 应用缩放
    transform.scale(m_zoom, m_zoom);

    // 应用宽高比修正
    float widgetAspect = static_cast<float>(width()) / height();
    if (widgetAspect > m_imageAspectRatio) {

        transform.scale(m_imageAspectRatio / widgetAspect, 1.0f);
    } else {

        transform.scale(1.0f, widgetAspect / m_imageAspectRatio);
    }

    m_shaderProgram->setUniformValue(m_matrixLoc, transform);
}

bool OpenGLWidget::ShowImage(QImage &currImage)
{
    if(currImage.isNull())
    {

        return false;
    }
   // m_image = currImage.convertToFormat(QImage::Format_RGBA8888);
    qDebug()<<"the image width:"<<currImage.width()<<"  height:"<<currImage.height();
    m_imageAspectRatio = static_cast<float>(currImage.width()) / currImage.height();
    // 删除旧纹理
    delete m_texture;
     m_texture = new QOpenGLTexture(currImage);
    // 设置纹理参数
    m_texture->setMinificationFilter(QOpenGLTexture::Linear);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setWrapMode(QOpenGLTexture::ClampToEdge);


    update();
    return true;
}

bool OpenGLWidget::loadImage(const QString &imagePath)
{

    makeCurrent();

    QImage image(imagePath);
    if (image.isNull()) {
        qWarning() << "无法加载图片:" << imagePath;
        emit imageLoaded(false);
        return false;
    }

    m_image = image.convertToFormat(QImage::Format_RGBA8888);
    m_imageAspectRatio = static_cast<float>(m_image.width()) / m_image.height();
    qDebug()<<"image width:"<<m_image.width()<<"  height:"<<m_image.height();
    // 删除旧纹理
    delete m_texture;
    m_texture = new QOpenGLTexture(m_image.mirrored());

    // 设置纹理参数
    m_texture->setMinificationFilter(QOpenGLTexture::Linear);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setWrapMode(QOpenGLTexture::ClampToEdge);

    // 重置视图
    fitToWindow();

    update();
    emit imageLoaded(true);
    return true;
}

void OpenGLWidget::fitToWindow()
{
    m_zoom = 1.0f;
    m_panX = 0.0f;
    m_panY = 0.0f;
    update();
    emit zoomChanged(m_zoom);
}

void OpenGLWidget::actualSize()
{
    m_zoom = 1.0f;
    m_panX = 0.0f;
    m_panY = 0.0f;
    update();
    emit zoomChanged(m_zoom);
}

void OpenGLWidget::setZoom(float zoom)
{
    m_zoom = qMax(0.02f, qMin(50.0f, zoom));
    update();
    emit zoomChanged(m_zoom);
}

void OpenGLWidget::setPan(float dx, float dy)
{
    m_panX = dx;
    m_panY = dy;
    update();
}

void OpenGLWidget::zoomIn()
{
    setZoom(m_zoom * 1.2f);
}

void OpenGLWidget::zoomOut()
{
    setZoom(m_zoom / 1.2f);
}

// 鼠标事件处理
void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_lastMousePos = event->pos();
    }
    QOpenGLWidget::mousePressEvent(event);
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        QPoint delta = event->pos() - m_lastMousePos;
        if(m_zoom<1.0)
        {
            m_panX += delta.x() * 0.001f / m_zoom;
            m_panY -= delta.y() * 0.001f / m_zoom;
        }
        else if(m_zoom<5.0)
        {
            m_panX += delta.x() * 0.002f / m_zoom;
            m_panY -= delta.y() * 0.002f / m_zoom;
        }else if(m_zoom<10.0)
        {
            m_panX += delta.x() * 0.01f / m_zoom;
            m_panY -= delta.y() * 0.01f / m_zoom;
        }
        else if(m_zoom<30.0)
        {
            m_panX += delta.x() * 0.04f / m_zoom;
            m_panY -= delta.y() * 0.04f / m_zoom;
        }
        else
        {
            m_panX += delta.x() * 0.1f / m_zoom;
            m_panY -= delta.y() * 0.1f / m_zoom;
        }
        m_lastMousePos = event->pos();
        update();

    }




    QOpenGLWidget::mouseMoveEvent(event);
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;

    }
    QOpenGLWidget::mouseReleaseEvent(event);
}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;
    if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;
        float zoomFactor = 1.0f + numSteps.y() * 0.1f;
        setZoom(m_zoom * zoomFactor);
    }
    event->accept();
}

void OpenGLWidget::mouseDoubleClickEvent(class QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_lastMousePos = event->pos();
        setZoom( 1.0);
        qDebug()<<"left button double click";
    }
    if (event->button() == Qt::RightButton) {
        makeCurrent();
        GLubyte pixel[3];
        m_lastMousePos = event->pos();
        pixel[0]=0;pixel[1]=0;pixel[2]=0;
        glReadPixels(m_lastMousePos.x(), m_lastMousePos.y(), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
        qDebug()<<"right button double click";
        qDebug()<<"red :"<<pixel[0]<<"green:"<<pixel[1]<<"blue"<<pixel[2]<<"the x :"<<m_lastMousePos.x()<<"the y:"<<m_lastMousePos.y();

        event->accept();
    }

}
