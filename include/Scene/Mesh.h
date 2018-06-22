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

    void setIndexCount(unsigned int count) { m_indexCount = count; }

    unsigned int indexOffset() const { return m_indexOffset; }

    void setIndexOffset(unsigned int index) { m_indexOffset = index; }

private:
    QString m_name;
    QString m_materialName;
    unsigned int m_indexCount;
    unsigned int m_indexOffset;
};

#endif //QSPACE_MESH_H
