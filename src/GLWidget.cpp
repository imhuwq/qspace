#include "GLWidget.h"
#include <QPair>
#include <iostream>

using namespace std;

GLWidget::GLWidget() : m_shaderProgram(nullptr), m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
                       m_indexBuffer(QOpenGLBuffer::IndexBuffer) {

    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);
    format.setSamples(16);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    setFormat(format);
    m_transform.translate(0.0f, 0.0f, -10.0f);
}

void GLWidget::createShaders() {
    m_shaderProgram = new QOpenGLShaderProgram();
    m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/simple.vert");
    m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/simple.frag");
    m_shaderProgram->link();
    m_shaderProgram->bind();
};

void GLWidget::createBuffers() {
    m_vertexArrayObject.create();
    m_vertexArrayObject.bind();

    QSharedPointer<const VertexBuffer> vertexBuffer = m_scene->getVertexBufferConst();

    m_vertexBuffer.create();
    m_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vertexBuffer->m_positions.constData(), vertexBuffer->m_positions.size() * sizeof(double));
    m_shaderProgram->enableAttributeArray(0);
    m_shaderProgram->setAttributeBuffer(0, GL_DOUBLE, 0, 3);

    m_indexBuffer.create();
    m_indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_indexBuffer.bind();
    m_indexBuffer.allocate(vertexBuffer->m_indices.constData(), vertexBuffer->m_indices.size() * sizeof(int));
}

void GLWidget::loadModelFile(QString filePath) {
    filePath = filePath.length() == 0 ? "test_files/cube.fbx" : filePath;

    if (!m_loader->checkFileFormatIsSupported(filePath)) {
        qDebug() << "Unsupported file format: " << filePath << "\n";
        return;
    }

    m_scene = QSharedPointer<Scene>(new Scene);
    m_loader = QSharedPointer<ModelLoader>(new ModelLoader);
    if (!m_loader->load(filePath, m_scene)) qDebug() << "Failed to load file: " << filePath << "\n";
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
    printContextInformation();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

    loadModelFile();
    createShaders();
    createBuffers();
}

void GLWidget::resizeGL(int w, int h) {
    m_projection.setToIdentity();
    m_projection.perspective(45.0f, w / float(h), 0.001f, 1000.0f);
}

void GLWidget::drawNode(QSharedPointer<Node> node, QMatrix4x4 objectMatrix) {
    objectMatrix *= node->transformation();
    m_shaderProgram->setUniformValue("modelToWorld", objectMatrix);

    for (auto &mesh:node->meshes()) {
        glDrawElements(GL_TRIANGLES, mesh->indexCount(), GL_UNSIGNED_INT, 0);
    }

    for (auto &childNode:node->nodes()) drawNode(childNode, objectMatrix);
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_shaderProgram->setUniformValue("worldToCamera", m_camera.toMatrix());
    m_shaderProgram->setUniformValue("cameraToView", m_projection);
    m_shaderProgram->setUniformValue("modelToWorld", m_transform.toMatrix());
    drawNode(m_scene->rootNode(), m_transform.toMatrix());
}

void GLWidget::teardownGL() {
    m_vertexArrayObject.destroy();
    m_vertexBuffer.destroy();
    delete m_shaderProgram;
}

void GLWidget::printContextInformation() {
    QString glType;
    QString glVersion;

    glType = context()->isOpenGLES() ? "OpenGL ES" : "OpenGL";
    glVersion = reinterpret_cast<const char *>(glGetString(GL_VERSION));

    qDebug() << qPrintable(glType) << qPrintable(glVersion);
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
    Input::registerMousePress(event->button());
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    Input::registerMouseRelease(event->button());
}
