#ifndef QSPACE_MESH_H
#define QSPACE_MESH_H

#include <QString>
#include <QSharedPointer>
#include "Material.h"

class Mesh {
public:
    const QString &name() const { return m_name; }

    void setName(const QString &name) { m_name = name; }

    unsigned int indexCount() const { return m_indexCount; }

    unsigned int indexOffset() const { return m_indexOffset; }

    const QSharedPointer<Material> &material() const { return m_material; };

private:
    QString m_name;
    unsigned int m_indexCount;
    unsigned int m_indexOffset;
    QSharedPointer<Material> m_material;
};

#endif //QSPACE_MESH_H
