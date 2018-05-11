#include "Transform3D.h"

const QVector3D Transform3D::LocalForward(0.0f, 0.0f, 1.0f);
const QVector3D Transform3D::LocalUp(0.0f, 1.0f, 0.0f);
const QVector3D Transform3D::LocalRight(1.0f, 0.0f, 0.0f);

QDebug operator<<(QDebug dbg, const Transform3D &transform) {
    dbg << "Transform3D\n{\n";
    dbg << "Position: <" << transform.translation().x() << ", " << transform.translation().y() << ", " << transform.translation().z() << ">\n";
    dbg << "Scale: <" << transform.scale().x() << ", " << transform.scale().y() << ", " << transform.scale().z() << ">\n";
    dbg << "Rotation: <" << transform.rotation().x() << ", " << transform.rotation().y() << ", " << transform.rotation().z() << " | "
        << transform.rotation().scalar() << ">\n}";
    return dbg;
}

QDataStream &operator<<(QDataStream &out, const Transform3D &transform3d) {
    out << transform3d.m_translation;
    out << transform3d.m_scale;
    out << transform3d.m_rotation;
    return out;
}

QDataStream &operator>>(QDataStream &in, Transform3D &transform3d) {
    in >> transform3d.m_translation;
    in >> transform3d.m_scale;
    in >> transform3d.m_rotation;
    transform3d.m_dirty = true;
    return in;
}