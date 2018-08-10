#ifndef QSPACE_CAMERA3D_H
#define QSPACE_CAMERA3D_H

#include "Control/Input.h"
#include "Transform3D.h"
#include "Scene/Node.h"

class Camera {
public:
    Camera() : m_translation(0, 0, -10) {
        m_matrix.setToIdentity();
        m_matrix.translate(m_translation);
    }

    static const QVector3D LocalUp;

    const QMatrix4x4 &toMatrix() { return m_matrix; }

    void OrbitAround(kNodePtr target) {
        QVector3D offset = target->GetTranslation() - m_translation;
        float horizontalMovement = -Input::MouseDelta().x() * m_orbitSpeed;
        float verticalMovement = Input::MouseDelta().y() * m_orbitSpeed;
        QQuaternion rotation = QQuaternion::fromEulerAngles(verticalMovement, horizontalMovement, 0);
        m_translation = target->GetTranslation() - rotation * offset;
        m_matrix.setToIdentity();
        m_matrix.lookAt(m_translation, target->GetTranslation(), LocalUp);
    }

private:
    float m_orbitSpeed = 0.1f;
    QVector3D m_translation;
    QMatrix4x4 m_matrix;
};

Q_DECLARE_TYPEINFO(Camera, Q_MOVABLE_TYPE);

#endif //QSPACE_CAMERA3D_H
