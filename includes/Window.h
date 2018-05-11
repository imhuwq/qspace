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

#include "Vertex.h"
#include "Transform3D.h"

class Window : public QOpenGLWindow, protected QOpenGLFunctions {
Q_OBJECT
public:
    Window();

    ~Window() {
        makeCurrent();  // http://doc.qt.io/qt-5/qopenglwindow.html#dtor.QOpenGLWindow
        teardownGL();
    }

protected:
    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

    void teardownGL();

protected slots:

    void update();

private:
    QOpenGLBuffer m_buffer;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLShaderProgram *m_program;
    int u_modelToWorldID;
    int u_worldToViewID;
    QMatrix4x4 m_projection;
    Transform3D m_transform;

    void printContextInformation();
};

#endif //QSPACE_QTRIANGLEWINDOW_H
