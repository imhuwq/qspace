#include "Window.h"

// Front Verticies
#define VERTEX_FTR Vertex( QVector3D( 0.5f,  0.5f,  0.5f), QVector3D( 1.0f, 0.0f, 0.0f ) )
#define VERTEX_FTL Vertex( QVector3D(-0.5f,  0.5f,  0.5f), QVector3D( 0.0f, 1.0f, 0.0f ) )
#define VERTEX_FBL Vertex( QVector3D(-0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 1.0f ) )
#define VERTEX_FBR Vertex( QVector3D( 0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 0.0f ) )

// Back Verticies
#define VERTEX_BTR Vertex( QVector3D( 0.5f,  0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 0.0f ) )
#define VERTEX_BTL Vertex( QVector3D(-0.5f,  0.5f, -0.5f), QVector3D( 0.0f, 1.0f, 1.0f ) )
#define VERTEX_BBL Vertex( QVector3D(-0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 0.0f, 1.0f ) )
#define VERTEX_BBR Vertex( QVector3D( 0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 1.0f ) )

// Create a colored cube
static const Vertex sg_vertices[] = {
        // Face 1 (Front)
        VERTEX_FTR, VERTEX_FTL, VERTEX_FBL,
        VERTEX_FBL, VERTEX_FBR, VERTEX_FTR,
        // Face 2 (Back)
        VERTEX_BBR, VERTEX_BTL, VERTEX_BTR,
        VERTEX_BTL, VERTEX_BBR, VERTEX_BBL,
        // Face 3 (Top)
        VERTEX_FTR, VERTEX_BTR, VERTEX_BTL,
        VERTEX_BTL, VERTEX_FTL, VERTEX_FTR,
        // Face 4 (Bottom)
        VERTEX_FBR, VERTEX_FBL, VERTEX_BBL,
        VERTEX_BBL, VERTEX_BBR, VERTEX_FBR,
        // Face 5 (Left)
        VERTEX_FBL, VERTEX_FTL, VERTEX_BTL,
        VERTEX_FBL, VERTEX_BTL, VERTEX_BBL,
        // Face 6 (Right)
        VERTEX_FTR, VERTEX_FBR, VERTEX_BBR,
        VERTEX_BBR, VERTEX_BTR, VERTEX_FTR
};

#undef VERTEX_BBR
#undef VERTEX_BBL
#undef VERTEX_BTL
#undef VERTEX_BTR

#undef VERTEX_FBR
#undef VERTEX_FBL
#undef VERTEX_FTL
#undef VERTEX_FTR

Window::Window() {
    {
        QSurfaceFormat format;
        format.setRenderableType(QSurfaceFormat::OpenGL);
        format.setProfile(QSurfaceFormat::CoreProfile);
        format.setVersion(3, 3);
        format.setSamples(16);
        format.setDepthBufferSize(24);
        format.setStencilBufferSize(8);
        setFormat(format);
    }
    {
        m_transform.translate(0.0f, 0.0f, -5.0f);
    }
}

void Window::initializeGL() {
    initializeOpenGLFunctions();
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
    printContextInformation();

    glEnable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    {
        m_program = new QOpenGLShaderProgram();
        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/simple.vert");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/simple.frag");
        m_program->link();
        m_program->bind();

        u_modelToWorldID = m_program->uniformLocation("modelToWorld");
        u_worldToCameraID = m_program->uniformLocation("worldToCamera");
        u_cameraToViewID = m_program->uniformLocation("cameraToView");

        m_buffer.create();
        m_buffer.bind();
        m_buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
        m_buffer.allocate(sg_vertices, sizeof(sg_vertices));  // http://doc.qt.io/qt-5/qopenglbuffer.html#allocate

        m_vao.create();
        m_vao.bind();
        m_program->enableAttributeArray(0);
        m_program->enableAttributeArray(1);
        m_program->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
        m_program->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize,
                                      Vertex::stride());  // http://doc.qt.io/qt-5/qopenglshaderprogram.html#setAttributeBuffer

        m_vao.release();
        m_buffer.release();
        m_program->release();
    }
}

void Window::resizeGL(int w, int h) {
    m_projection.setToIdentity();
    m_projection.perspective(45.0f, w / float(h), 0.0f, 1000.0f);
}

void Window::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();
    m_program->setUniformValue(u_worldToCameraID, m_camera.toMatrix());
    m_program->setUniformValue(u_cameraToViewID, m_projection);

    {
        m_vao.bind();
        m_program->setUniformValue(u_modelToWorldID, m_transform.toMatrix());
        glDrawArrays(GL_TRIANGLES, 0, sizeof(sg_vertices) / sizeof(sg_vertices[0]));
        m_vao.release();
    }

    m_program->release();
}

void Window::teardownGL() {
    m_vao.destroy();
    m_buffer.destroy();
    delete m_program;
}

void Window::printContextInformation() {
    QString glType;
    QString glVersion;
    QString glProfile;

    glType = context()->isOpenGLES() ? "OpenGL ES" : "OpenGL";
    glVersion = reinterpret_cast<const char *>(glGetString(GL_VERSION));

#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
    switch (format().profile()) {
        CASE(NoProfile);
        CASE(CoreProfile);
        CASE(CompatibilityProfile);
    }
#undef CASE
    qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}

void Window::update() {
    Input::update();

    if (Input::buttonPressed(Qt::LeftButton)) {
        static const float transSpeed = 0.3f;
        static const float rotSpeed = 0.3f;

        m_camera.rotate(-rotSpeed * Input::mouseDelta().x(), Camera3D::LocalUp);
        m_camera.rotate(-rotSpeed * Input::mouseDelta().y(), m_camera.right());

        QVector3D translation;
        m_camera.translate(transSpeed * translation);
    }

    m_transform.rotate(1.0f, QVector3D(0.4f, 0.3f, 0.3f));
    QOpenGLWindow::update();
}

void Window::keyPressEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) {
        event->ignore();
    } else {
        Input::registerKeyPress(event->key());
    }
}

void Window::keyReleaseEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) {
        event->ignore();
    } else {
        Input::registerKeyRelease(event->key());
    }
}

void Window::mousePressEvent(QMouseEvent *event) {
    Input::registerMousePress(event->button());
}

void Window::mouseReleaseEvent(QMouseEvent *event) {
    Input::registerMouseRelease(event->button());
}
