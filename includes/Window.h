#ifndef QSPACE_QTRIANGLEWINDOW_H
#define QSPACE_QTRIANGLEWINDOW_H

#include <QDebug>
#include <QString>
#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLPaintDevice>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

class Window : public QOpenGLWindow, protected QOpenGLFunctions {
Q_OBJECT
public:
    Window() {
        QSurfaceFormat format;
        format.setRenderableType(QSurfaceFormat::OpenGL);
        format.setProfile(QSurfaceFormat::CoreProfile);
        format.setVersion(3, 3);
        format.setSamples(16);
        format.setDepthBufferSize(24);
        format.setStencilBufferSize(8);
        setFormat(format);
    }

    ~Window() {
        makeCurrent();
        teardownGL();
    }

protected:
    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

    void teardownGL();

private:
    QOpenGLBuffer m_buffer;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLShaderProgram *m_program;

    void printContextInformation();
};

#endif //QSPACE_QTRIANGLEWINDOW_H
