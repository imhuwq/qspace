#ifndef QSPACE_MESH_H
#define QSPACE_MESH_H

#include <QString>
#include <QSharedPointer>
#include "Material.h"

class Mesh {
public:
    void extendTmpIndices(const QVector<unsigned int> &indices) {
        m_tmpIndices.append(indices);
        m_indexCount += indices.size();
    }

    const QVector<unsigned int> &tmpIndices() { return m_tmpIndices; }

    void clearTmpIndices() { m_tmpIndices.clear(); }

    unsigned int indexCount() const { return m_indexCount; }

    void setIndexCount(unsigned int count) { m_indexCount = count; }

    unsigned int indexOffset() const { return m_indexOffset; }

    void setIndexOffset(unsigned int index) { m_indexOffset = index; }

private:
    QVector<unsigned int> m_tmpIndices;  // 合并到 node 的 vertexBuffer 后成为空的
    unsigned int m_indexCount;
    unsigned int m_indexOffset;
};

#endif //QSPACE_MESH_H
