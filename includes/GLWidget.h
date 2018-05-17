#ifndef QSPACE_GLWIDGET_H
#define QSPACE_GLWIDGET_H

#include <QDebug>
#include <QString>
#include <QOpenGLWidget>
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
#include "ModelLoader.h"
#include "Scene.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
Q_OBJECT
public:
    GLWidget();

    ~GLWidget() {
        makeCurrent();  // http://doc.qt.io/qt-5/qopenglwindow.html#dtor.QOpenGLWindow
        teardownGL();
    }

protected:
    void loadModelFile(const QString &filePath = "");

    void createBuffers();

    void createShaders();

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
    bool m_glInitialized;
    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ibo;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLShaderProgram *m_shd;
    int u_modelToWorldID;
    int u_worldToCameraID;
    int u_cameraToViewID;
    Camera3D m_camera;
    QMatrix4x4 m_projection;
    Transform3D m_transform;

    QSharedPointer<ModelLoader> m_loader;
    QSharedPointer<Scene> m_scene;

    void printContextInformation();
};

#endif //QSPACE_GLWIDGET_H
