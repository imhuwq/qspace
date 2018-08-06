#include "Application.h"

int main(int argc, char **argv) {
    Application app(argc, argv);
//    QVector3D position = QVector3D(0, 0, 0);
//    QVector3D rotation = QVector3D(-90, 0, 0);
//    QVector3D scale = QVector3D(100, 100, 100);
//    QMatrix4x4 transformation;
//    transformation.setToIdentity();
//    qDebug() << transformation;
//
//    transformation.translate(position);
//    qDebug() << transformation;
//
//    QQuaternion quaternion = QQuaternion::fromEulerAngles(-90, 0, 0);
//    transformation.rotate(quaternion);
//    qDebug() << transformation;
//
//    transformation.scale(scale);
//    qDebug() << transformation;
    return app.run();
}
