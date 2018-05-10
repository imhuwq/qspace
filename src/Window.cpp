#include "Window.h"
#include "Vertex.h"

static const Vertex sg_vertices[] = {
        Vertex(QVector3D(0.00f, 0.75f, 1.00f), QVector3D(1.00f, 0.00f, 0.00f)),
        Vertex(QVector3D(0.75f, -0.75f, 1.00f), QVector3D(0.00f, 1.00f, 0.00f)),
        Vertex(QVector3D(-0.75f, -0.75f, 1.00f), QVector3D(0.00f, 0.00f, 1.00f))
};

void Window::initializeGL() {
    initializeOpenGLFunctions();
    printContextInformation();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    {
        m_program = new QOpenGLShaderProgram();
        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/simple.vert");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/simple.frag");
        m_program->link();
        m_program->bind();

        m_buffer.create();
        m_buffer.bind();
        m_buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
        m_buffer.allocate(sg_vertices, sizeof(sg_vertices));

        m_vao.create();
        m_vao.bind();
        m_program->enableAttributeArray(0);
        m_program->enableAttributeArray(1);
        m_program->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize,
                                      Vertex::stride());
        m_program->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

        m_vao.release();
        m_buffer.release();
        m_program->release();
    }
}

void Window::resizeGL(int w, int h) {
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void Window::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();
    {
        m_vao.bind();
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