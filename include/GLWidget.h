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
#include "Camera3D.h"
#include "Input.h"
#include "ModelLoader/ModelLoader.h"
#include "Scene/Scene.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
Q_OBJECT
public:
    GLWidget();

    ~GLWidget() {
        makeCurrent();  // http://doc.qt.io/qt-5/qopenglwindow.html#dtor.QOpenGLWindow
        teardownGL();
    }

    void loadModelFile(QString filePath = "");

protected:
    void createBuffers();

    void createShaders();

    void createTextures();

    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

    void teardownGL();

    void keyPressEvent(QKeyEvent *event);

    void keyReleaseEvent(QKeyEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

protected slots:

    void update();

private:
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
    QOpenGLVertexArrayObject m_vertexArrayObject;
    QOpenGLShaderProgram *m_shaderProgram;
    QMap<QString, QSharedPointer<QOpenGLTexture>> m_textures;

    Camera3D m_camera;
    QMatrix4x4 m_projection;
    Transform3D m_transform;

    QSharedPointer<ModelLoader> m_loader;
    QSharedPointer<Scene> m_scene;

    void printContextInformation();

    void createTexturesForNode(const QSharedPointer<Node> &node);

    void drawNode(QSharedPointer<Node> node, QMatrix4x4 objectMatrix);
};

#endif //QSPACE_GLWIDGET_H
