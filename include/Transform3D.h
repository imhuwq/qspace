#ifndef QSPACE_TRANSFORM3D_H
#define QSPACE_TRANSFORM3D_H

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

class Transform3D {
public:
    static const QVector3D LocalForward;
    static const QVector3D LocalUp;
    static const QVector3D LocalRight;

    Transform3D() : m_dirty(true), m_scale(1.0f, 1.0f, 1.0f) {}

    // transform by
    void translate(const QVector3D &dt) {
        m_dirty = true;
        m_translation += dt;
    }

    void translate(float dx, float dy, float dz) {
        m_dirty = true;
        m_translation += QVector3D(dx, dy, dz);
    }

    void scale(const QVector3D &ds) {
        m_dirty = true;
        m_scale *= ds;
    }

    void scale(float dx, float dy, float dz) {
        m_dirty = true;
        m_scale *= QVector3D(dx, dy, dz);
    }

    void rotate(const QQuaternion &dr) {
        m_dirty = true;
        m_rotation = dr * m_rotation;
    }

    void rotate(float angle, const QVector3D &axis) {
        m_dirty = true;
        QQuaternion tmp_q = QQuaternion::fromAxisAndAngle(axis, angle);
        m_rotation = tmp_q * m_rotation;
    }

    void rotate(float angle, float ax, float ay, float az) {
        m_dirty = true;
        QQuaternion tmp_q = QQuaternion::fromAxisAndAngle(ax, ay, az, angle);
        m_rotation = tmp_q * m_rotation;
    }

    void grow(const QVector3D &ds) {
        m_dirty = true;
        m_scale += ds;
    }

    void grow(float dx, float dy, float dz) {
        m_dirty = true;
        m_scale += QVector3D(dx, dy, dz);
    }

    void grow(float factor) {
        m_dirty = true;
        m_scale += QVector3D(factor, factor, factor);
    }

    // transform to
    void setTranslation(const QVector3D &t) {
        m_dirty = true;
        m_translation = t;
    }

    void setTranslation(float x, float y, float z) {
        m_dirty = true;
        m_translation = QVector3D(x, y, z);
    }

    void setScale(const QVector3D &s) {
        m_dirty = true;
        m_scale = s;
    }

    void setScale(float x, float y, float z) {
        m_dirty = true;
        m_scale = QVector3D(x, y, z);
    }

    void setScale(float k) {
        m_dirty = true;
        m_scale = QVector3D(k, k, k);
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

    const QVector3D &translation() const {
        return m_translation;
    }

    const QVector3D &scale() const {
        return m_scale;
    }

    const QQuaternion &rotation() const {
        return m_rotation;
    }

    const QMatrix4x4 &toMatrix() {
        if (m_dirty) {
            m_dirty = false;
            m_world.setToIdentity();
            m_world.translate(m_translation);
            m_world.rotate(m_rotation);
            m_world.scale(m_scale);
        }
        return m_world;
    }

    QVector3D forward() const {
        return m_rotation.rotatedVector(LocalForward);
    }

    QVector3D up() const {
        return m_rotation.rotatedVector(LocalUp);
    }

    QVector3D right() const {
        return m_rotation.rotatedVector(LocalRight);
    }

private:
    bool m_dirty;
    QVector3D m_translation;
    QVector3D m_scale;
    QQuaternion m_rotation;
    QMatrix4x4 m_world;

#ifndef QT_NO_DATASTREAM

    friend QDataStream &operator<<(QDataStream &out, const Transform3D &transform3d);

    friend QDataStream &operator>>(QDataStream &in, Transform3D &transform3d);

#endif
};

Q_DECLARE_TYPEINFO(Transform3D, Q_MOVABLE_TYPE);

#endif //QSPACE_TRANSFORM3D_H
