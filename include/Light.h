#ifndef QSPACE_LIGHT_H
#define QSPACE_LIGHT_H

#include <QString>
#include <QVector3D>
#include <QVector4D>

class Light {
public:
    const QVector4D &position() {
        return m_position;
    }

    const QVector3D &intensity() {
        return m_intensity;
    }

private:
    QVector4D m_position;
    QVector3D m_intensity;
};

#endif //QSPACE_LIGHT_H
