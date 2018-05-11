#include "Camera3D.h"

const QVector3D Camera3D::LocalForward(0.0f, 0.0f, -1.0f);
const QVector3D Camera3D::LocalUp(0.0f, 1.0f, 0.0f);
const QVector3D Camera3D::LocalRight(1.0f, 0.0f, 0.0f);

QDebug operator<<(QDebug dbg, const Camera3D &transform) {
    dbg << "Camera3D\n{\n";
    dbg << "Position: <" << transform.translation().x() << ", " << transform.translation().y() << ", " << transform.translation().z() << ">\n";
    dbg << "Rotation: <" << transform.rotation().x() << ", " << transform.rotation().y() << ", " << transform.rotation().z() << " | "
        << transform.rotation().scalar() << ">\n}";
    return dbg;
}

QDataStream &operator<<(QDataStream &out, const Camera3D &transform) {
    out << transform.m_translation;
    out << transform.m_rotation;
    return out;
}

QDataStream &operator>>(QDataStream &in, Camera3D &transform) {
    in >> transform.m_translation;
    in >> transform.m_rotation;
    transform.m_dirty = true;
    return in;
}
