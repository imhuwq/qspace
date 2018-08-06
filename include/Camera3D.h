#ifndef QSPACE_CAMERA3D_H
#define QSPACE_CAMERA3D_H

#include "Input.h"
#include "Transform3D.h"
#include "Scene/Node.h"

class Camera3D {
public:
    Camera3D() : m_translation(0, 0, -10) {
        m_matrix.setToIdentity();
        m_matrix.translate(m_translation);
    }

    static const QVector3D LocalUp;

    const QMatrix4x4 &toMatrix() { return m_matrix; }

    void orbitAround(const NodePtr &target) {
        QVector3D offset = target->getPosition() - m_translation;
        float horizontalMovement = -Input::mouseDelta().x() * m_orbitSpeed;
        float verticalMovement = Input::mouseDelta().y() * m_orbitSpeed;
        QQuaternion rotation = QQuaternion::fromEulerAngles(verticalMovement, horizontalMovement, 0);
        m_translation = target->getPosition() - rotation * offset;
        m_matrix.setToIdentity();
        m_matrix.lookAt(m_translation, target->getPosition(), LocalUp);
        qDebug() << m_matrix;
    }

private:
    float m_orbitSpeed = 0.1f;
    QVector3D m_translation;
    QMatrix4x4 m_matrix;
};

Q_DECLARE_TYPEINFO(Camera3D, Q_MOVABLE_TYPE);

#endif //QSPACE_CAMERA3D_H
