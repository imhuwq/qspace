#ifndef QSPACE_CAMERA3D_H
#define QSPACE_CAMERA3D_H

#include "Transform3D.h"

class Camera3D {
public:
    static const QVector3D LocalForward;
    static const QVector3D LocalUp;
    static const QVector3D LocalRight;

    Camera3D() : m_dirty(true) {}

    void translate(const QVector3D &dt) {
        m_dirty = true;
        m_translation += dt;
    }

    void translate(float dx, float dy, float dz) {
        m_dirty = true;
        m_translation += QVector3D(dx, dy, dz);
    }

    void rotate(const QQuaternion &dr) {
        m_dirty = true;
        m_rotation = dr * m_rotation;
    }

    void rotate(float angle, const QVector3D &axis) {
        m_dirty = true;
        m_rotation = QQuaternion::fromAxisAndAngle(axis, angle) * m_rotation;
    }

    void rotate(float angle, float ax, float ay, float az) {
        m_dirty = true;
        m_rotation = QQuaternion::fromAxisAndAngle(ax, ay, az, angle) * m_rotation;
    }

    void setTranslation(const QVector3D &t) {
        m_dirty = true;
        m_translation = t;
    }

    void setTranslation(float x, float y, float z) {
        m_dirty = true;
        m_translation = QVector3D(x, y, z);
    }

    void setRotation(const QQuaternion &r) {
        m_dirty = true;
        m_rotation = r;
    }

    void setRotation(float angle, const QVector3D &axis) {
        m_dirty = true;
        m_rotation = QQuaternion::fromAxisAndAngle(axis, angle);
    }

    void setRotation(float angle, float ax, float ay, float az) {
        m_dirty = true;
        m_rotation = QQuaternion::fromAxisAndAngle(ax, ay, az, angle);
    }

    const QVector3D &translation() const { return m_translation; }

    const QQuaternion &rotation() const { return m_rotation; }

    const QMatrix4x4 &toMatrix() {
        if (m_dirty) {
            m_dirty = false;
            m_world.setToIdentity();
            m_world.rotate(m_rotation.conjugate());
            m_world.translate(-m_translation);
        }
        return m_world;
    }

    QVector3D forward() const {
        return m_rotation.rotatedVector(LocalForward);
    }

    QVector3D right() const {
        return m_rotation.rotatedVector(LocalRight);
    }

    QVector3D up() const {
        return m_rotation.rotatedVector(LocalUp);
    }

private:
    bool m_dirty;
    QVector3D m_translation;
    QQuaternion m_rotation;
    QMatrix4x4 m_world;

#ifndef QT_NO_DATASTREAM

    friend QDataStream &operator<<(QDataStream &out, const Camera3D &transform);

    friend QDataStream &operator>>(QDataStream &in, Camera3D &transform);

#endif
};

Q_DECLARE_TYPEINFO(Camera3D, Q_MOVABLE_TYPE);

#endif //QSPACE_CAMERA3D_H
