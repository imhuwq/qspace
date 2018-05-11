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
#include <QKeyEvent>

#include "Vertex.h"
#include "Transform3D.h"
#include "Camera3D.h"
#include "Input.h"

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

protected:
protected:
    void keyPressEvent(QKeyEvent *event);

    void keyReleaseEvent(QKeyEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

private:
    QOpenGLBuffer m_buffer;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLShaderProgram *m_program;
    int u_modelToWorldID;
    int u_worldToCameraID;
    int u_cameraToViewID;
    Camera3D m_camera;
    QMatrix4x4 m_projection;
    Transform3D m_transform;

    void printContextInformation();
};

#endif //QSPACE_QTRIANGLEWINDOW_H
