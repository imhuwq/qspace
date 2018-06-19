#include "GLWidget.h"
#include <QPair>
#include <iostream>
using namespace std;

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
    if (m_glInitialized) {
        m_shd->release();
        delete m_shd;
    }

    m_shd = new QOpenGLShaderProgram();
    m_shd->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/simple.vert");
    m_shd->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/simple.frag");
    m_shd->link();
    m_shd->bind();
    m_shd->setUniformValue("texture", 0);
};

void GLWidget::createBuffers() {
#define CREATE_OR_RELEASE(x) if (m_glInitialized) (x).release();\
                             else (x).create();

    CREATE_OR_RELEASE(m_vao);
    m_vao.bind();

    CREATE_OR_RELEASE(m_vbo);
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vbo.bind();
    m_vbo.allocate(m_scene->vertices().constData(), m_scene->vertices().size() * sizeof(float));
    m_shd->enableAttributeArray(0);
    m_shd->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    m_vbo.release();

    CREATE_OR_RELEASE(m_tbo);
    m_tbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_tbo.bind();
    m_tbo.allocate(m_scene->uvs().constData(), m_scene->uvs().size() * sizeof(float));
    m_shd->enableAttributeArray(1);
    m_shd->setAttributeBuffer(1, GL_FLOAT, 0, 2);
    m_tbo.release();

    CREATE_OR_RELEASE(m_ibo);
    m_ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_ibo.bind();
    m_ibo.allocate(m_scene->indices().constData(), m_scene->indices().size() * sizeof(unsigned int));

#undef CREATE_OR_RELEASE
}

void GLWidget::createTexturesForNode(const QSharedPointer<Node> &node) {
    for (const auto &mesh:node->meshes()) {
        const auto &material = mesh->material();
        for (const auto &textureInfo:material->texturePaths().toStdMap()) {
            QString key = textureInfo.first + "_" + textureInfo.second;
            QSharedPointer<QOpenGLTexture> texture(new QOpenGLTexture(QImage(textureInfo.second).mirrored()));
            m_textures[key] = texture;
        }
    }

    for (const auto& childNode:node->nodes()) {
        createTexturesForNode(childNode);
    }
}

void GLWidget::createTextures() {
    m_textures.clear();
    QSharedPointer<Node> rootNode = m_scene->rootNode();
    createTexturesForNode(rootNode);
}

void GLWidget::loadModelFile(QString filePath) {
    filePath = filePath.length() == 0 ? "test_files/cube.obj" : filePath;

    if (!m_loader->checkFileFormatIsOk(filePath)) {
        qDebug() << "Unsupported file format: " << filePath << "\n";
        return;
    }

    m_scene = QSharedPointer<Scene>(new Scene);
    m_loader = QSharedPointer<ModelLoader>(new ModelLoader);
    if (!m_loader->Load(filePath, m_scene)) qDebug() << "Failed to load file: " << filePath << "\n";

    createShaders();
    createBuffers();
    createTextures();
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
    printContextInformation();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

    loadModelFile();
    m_glInitialized = true;
}

void GLWidget::resizeGL(int w, int h) {
    m_projection.setToIdentity();
    m_projection.perspective(45.0f, w / float(h), 0.001f, 1000.0f);
}

void GLWidget::drawNode(QSharedPointer<Node> node, QMatrix4x4 objectMatrix) {
    objectMatrix *= node->transformation();
    m_shd->setUniformValue("modelToWorld", objectMatrix);

    for (auto &mesh:node->meshes()) {
        const auto &material = mesh->material();
        for (const auto &textureInfo:material->texturePaths().toStdMap()) {
            QString key = textureInfo.first + "_" + textureInfo.second;
            m_textures[key]->bind();
        }
        glDrawElements(GL_TRIANGLES, mesh->indexCount(), GL_UNSIGNED_INT, (void *) (mesh->indexOffset() * sizeof(unsigned int)));
    }

    for (auto &childNode:node->nodes()) {
        drawNode(childNode, objectMatrix);
    }
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shd->setUniformValue("worldToCamera", m_camera.toMatrix());
    m_shd->setUniformValue("cameraToView", m_projection);

    drawNode(m_scene->rootNode(), m_transform.toMatrix());
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
