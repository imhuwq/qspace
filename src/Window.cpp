#include "Window.h"

Window::Window() : m_vbo(QOpenGLBuffer::VertexBuffer), m_ibo(QOpenGLBuffer::IndexBuffer) {
    m_scene = QSharedPointer<Scene>(new Scene);

    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);
    format.setSamples(16);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    setFormat(format);
    m_transform.translate(0.0f, 0.0f, -5.0f);
}

void Window::initializeGL() {
    initializeOpenGLFunctions();
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
    printContextInformation();

    glEnable(GL_CULL_FACE);
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

    m_loader.Load("test_files/cube.obj", m_scene);

    m_shd = new QOpenGLShaderProgram();
    m_shd->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/simple.vert");
    m_shd->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/simple.frag");
    m_shd->link();
    m_shd->bind();

    u_modelToWorldID = m_shd->uniformLocation("modelToWorld");
    u_worldToCameraID = m_shd->uniformLocation("worldToCamera");
    u_cameraToViewID = m_shd->uniformLocation("cameraToView");

    m_vao.create();
    m_vao.bind();

    m_vbo.create();
    m_vbo.bind();
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vbo.allocate(m_scene->vertices().constData(), m_scene->vertices().size() * sizeof(float));
    m_shd->enableAttributeArray(0);
    m_shd->setAttributeBuffer(0, GL_FLOAT, 0, 3);

    m_ibo.create();
    m_ibo.bind();
    m_ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_ibo.allocate(m_scene->indices().constData(), m_scene->indices().size() * sizeof(unsigned int));

    m_vao.release();
    m_vbo.release();
    m_shd->release();
}

void Window::resizeGL(int w, int h) {
    m_projection.setToIdentity();
    m_projection.perspective(45.0f, w / float(h), 0.0f, 1000.0f);
}

void Window::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    m_shd->bind();
    m_shd->setUniformValue(u_worldToCameraID, m_camera.toMatrix());
    m_shd->setUniformValue(u_cameraToViewID, m_projection);

    m_vao.bind();
    m_shd->setUniformValue(u_modelToWorldID, m_transform.toMatrix());
    glDrawElements(GL_TRIANGLES, m_scene->indices().size(), GL_UNSIGNED_INT, (void *) 0);
    m_vao.release();

    m_shd->release();
}

void Window::teardownGL() {
    m_vao.destroy();
    m_vbo.destroy();
    delete m_shd;
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
