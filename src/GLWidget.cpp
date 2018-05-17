#include "GLWidget.h"

GLWidget::GLWidget() : m_glInitialized(false), m_shd(nullptr), m_vbo(QOpenGLBuffer::VertexBuffer), m_ibo(QOpenGLBuffer::IndexBuffer) {

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

void GLWidget::createShaders() {
    if (m_glInitialized) m_shd->release();
    else m_shd = new QOpenGLShaderProgram();

    m_shd->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/simple.vert");
    m_shd->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/simple.frag");
    m_shd->link();
    m_shd->bind();
};

void GLWidget::createBuffers() {
#define CREATE_OR_RELEASE(x) if (m_glInitialized) (x).release();\
                             else (x).create();

    u_modelToWorldID = m_shd->uniformLocation("modelToWorld");
    u_worldToCameraID = m_shd->uniformLocation("worldToCamera");
    u_cameraToViewID = m_shd->uniformLocation("cameraToView");

    CREATE_OR_RELEASE(m_vao);
    m_vao.bind();

    CREATE_OR_RELEASE(m_vbo);
    m_vbo.bind();
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vbo.allocate(m_scene->vertices().constData(), m_scene->vertices().size() * sizeof(float));
    m_shd->enableAttributeArray(0);
    m_shd->setAttributeBuffer(0, GL_FLOAT, 0, 3);

    CREATE_OR_RELEASE(m_ibo);
    m_ibo.bind();
    m_ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_ibo.allocate(m_scene->indices().constData(), m_scene->indices().size() * sizeof(unsigned int));

    m_vao.release();
    m_vbo.release();

#undef CREATE_OR_RELEASE
}

void GLWidget::loadModelFile(const QString &filePath) {
    m_scene = QSharedPointer<Scene>(new Scene);
    m_loader = QSharedPointer<ModelLoader>(new ModelLoader);

    if (filePath.length() == 0) m_loader->Load("test_files/cube.obj", m_scene);
    else m_loader->Load(filePath, m_scene);

    createShaders();
    createBuffers();
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
    printContextInformation();

    glEnable(GL_CULL_FACE);
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

    loadModelFile();
    m_glInitialized = true;
}

void GLWidget::resizeGL(int w, int h) {
    m_projection.setToIdentity();
    m_projection.perspective(45.0f, w / float(h), 0.0f, 1000.0f);
}

void GLWidget::paintGL() {
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

void GLWidget::teardownGL() {
    m_vao.destroy();
    m_vbo.destroy();
    delete m_shd;
}

void GLWidget::printContextInformation() {
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

void GLWidget::update() {
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
    QOpenGLWidget::update();
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) event->ignore();
}

void GLWidget::keyReleaseEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) event->ignore();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
//    Input::registerMousePress(event->button());
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
//    Input::registerMouseRelease(event->button());
}
