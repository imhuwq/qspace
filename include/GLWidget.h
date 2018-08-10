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
#include <QImage>
#include <QOpenGLTexture>

#include "Transform3D.h"
#include "Scene/Camera.h"
#include "Control/Input.h"
#include "ModelLoader/FbxLoader.h"
#include "Scene/Scene.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
Q_OBJECT
public:
    GLWidget();

    ~GLWidget() {
        makeCurrent();  // http://doc.qt.io/qt-5/qopenglwindow.html#dtor.QOpenGLWindow
        TeardownGL();
    }

    void LoadModelFile(QString filePath = "");

protected:
    void CreateBuffers();

    void CreateShaders();

    void createTextures();

    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

    void TeardownGL();

    void keyPressEvent(QKeyEvent *event);

    void keyReleaseEvent(QKeyEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

protected slots:

    void update();

private:
    QOpenGLBuffer gl_vertex_buffer_;
    QOpenGLBuffer gl_index_buffer_;
    QOpenGLVertexArrayObject gl_vertex_array_object_;
    QOpenGLShaderProgram *gl_shader_programe_;

    Camera camera_;
    QMatrix4x4 projection_;
    Transform3D transform_;

    FbxLoaderPtr loader_;
    ScenePtr scene_;

    void PrintContextInformation();

    void DrawNode(kNodePtr node, QMatrix4x4 objectMatrix);
};

#endif //QSPACE_GLWIDGET_H
