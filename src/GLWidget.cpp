#include "GLWidget.h"
#include <QPair>
#include <QtMath>
#include <iostream>

using namespace std;

GLWidget::GLWidget() : gl_shader_program_(nullptr), gl_position_buffer_(QOpenGLBuffer::VertexBuffer), gl_index_buffer_(QOpenGLBuffer::IndexBuffer) {
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);
    format.setSamples(16);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    setFormat(format);

    transform_ = NodePtr(new Node("transform"));
    camera_ = CameraPtr(new Camera("default camera"));
}

void GLWidget::CreateShaders() {
    gl_shader_program_ = new QOpenGLShaderProgram();
    gl_shader_program_->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/simple.vert");
    gl_shader_program_->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/simple.frag");
    gl_shader_program_->link();
    gl_shader_program_->bind();
};

void GLWidget::CreateBuffers() {
    gl_vertex_array_object_.create();
    gl_vertex_array_object_.bind();

    kScenePtr kScene = scene_;
    kVertexBufferPtr vertex_buffer = kScene->GetVertexBuffer();

    gl_position_buffer_.create();
    gl_position_buffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    gl_position_buffer_.bind();
    gl_position_buffer_.allocate(vertex_buffer->GetPositions().constData(), vertex_buffer->GetPositions().size() * sizeof(double));
    gl_shader_program_->enableAttributeArray(0);
    gl_shader_program_->setAttributeBuffer(0, GL_DOUBLE, 0, 3);

    gl_normal_buffer_.create();
    gl_normal_buffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    gl_normal_buffer_.bind();
    gl_normal_buffer_.allocate(vertex_buffer->GetNormals().constData(), vertex_buffer->GetPositions().size() * sizeof(double));
    gl_shader_program_->enableAttributeArray(1);
    gl_shader_program_->setAttributeBuffer(1, GL_DOUBLE, 0, 3);

    gl_index_buffer_.create();
    gl_index_buffer_.setUsagePattern(QOpenGLBuffer::StaticDraw);
    gl_index_buffer_.bind();
    gl_index_buffer_.allocate(vertex_buffer->GetIndices().constData(), vertex_buffer->GetIndices().size() * sizeof(unsigned int));
}

void GLWidget::LoadModelFile(QString filePath) {
    filePath = filePath.length() == 0 ? "models/cube.fbx" : filePath;

    if (!loader_->CheckFormatIsSupported(filePath)) {
        qDebug() << "Unsupported file format: " << filePath << "\n";
        return;
    }

    scene_ = ScenePtr(new Scene);
    loader_ = FbxLoaderPtr(new FbxLoader);
    if (!loader_->Load(filePath, scene_)) {
        qDebug() << "Failed to Load file: " << filePath << "\n";
        exit(1);
    }
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
    PrintContextInformation();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

    LoadModelFile();
    CreateShaders();
    CreateBuffers();
}

void GLWidget::resizeGL(int w, int h) {
    projection_.setToIdentity();
    projection_.perspective(45.0f, w / float(h), 0.01f, 100.0f);
}

void GLWidget::DrawNode(kNodePtr node, QMatrix4x4 objectMatrix) {
    objectMatrix *= node->GetTransformation();
    gl_shader_program_->setUniformValue("modelToWorld", objectMatrix);

    if (node->GetType() == NodeType::kMesh) {
        kMeshPtr mesh = qSharedPointerDynamicCast<const Mesh>(node);
        for (auto geometry: mesh->GetGeometries()) {
            glDrawElements(GL_TRIANGLES, geometry->GetIndicesSize(), GL_UNSIGNED_INT, (void *) (geometry->GetIndexOffset() * sizeof(unsigned int)));
        }
    }

    for (auto &childNode:node->GetNodes()) DrawNode(childNode, objectMatrix);
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gl_shader_program_->setUniformValue("modelMatrix", transform_->GetTransformation());
    gl_shader_program_->setUniformValue("viewMatrix", camera_->GetTransformation());
    gl_shader_program_->setUniformValue("projectionMatrix", projection_);
    DrawNode(scene_->GetModel(), QMatrix4x4());
}

void GLWidget::TeardownGL() {
    gl_vertex_array_object_.destroy();
    gl_position_buffer_.destroy();
    delete gl_shader_program_;
}

void GLWidget::PrintContextInformation() {
    QString glType;
    QString glVersion;

    glType = context()->isOpenGLES() ? "OpenGL ES" : "OpenGL";
    glVersion = reinterpret_cast<const char *>(glGetString(GL_VERSION));

    qDebug() << qPrintable(glType) << qPrintable(glVersion);
}

void GLWidget::update() {
    Input::Update();
    if (Input::ButtonPressed(Qt::LeftButton)) { camera_->OrbitAround(scene_->GetModel()); }
    QOpenGLWidget::update();
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) event->ignore();
}

void GLWidget::keyReleaseEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) event->ignore();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    Input::RegisterMousePress(event->button());
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    Input::RegisterMouseRelease(event->button());
}
