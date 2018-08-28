#ifndef QSPACE_CAMERA3D_H
#define QSPACE_CAMERA3D_H

#include "Control/Input.h"
#include "Transform3D.h"
#include "3dObject/Node.h"

class Camera;

typedef QSharedPointer<Camera> CameraPtr;
typedef QSharedPointer<const Camera> kCameraPtr;

class Camera : public Node {
public:
    explicit Camera(const QString &name, NodeType type = NodeType::kCamera) : Node(name, type) {
        Translate(QVector3D(0, 0, -10));
        ComputeTransform();
    }

    static const QVector3D LocalUp;

    void OrbitAround(kNodePtr target) {
        dirty_ = false;
        QVector3D offset = target->GetTranslation() - translation_;
        float horizontalMovement = -Input::MouseDelta().x() * orbit_speed_;
        float verticalMovement = Input::MouseDelta().y() * orbit_speed_ * 1.5f;
        QQuaternion rotation = QQuaternion::fromEulerAngles(verticalMovement, horizontalMovement, 0);
        SetTranslation(target->GetTranslation() - rotation * offset);
        LookAt(target);
    }

    void LookAt(kNodePtr target) {
        transformation_.setToIdentity();
        transformation_.lookAt(translation_, target->GetTranslation(), LocalUp);
    }

private:
    float orbit_speed_ = 0.1f;
};

#endif //QSPACE_CAMERA3D_H
