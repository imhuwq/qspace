#ifndef QSPACE_MESH_H
#define QSPACE_MESH_H

#include <QString>
#include <QSharedPointer>
#include "Material.h"

class Mesh {
public:
    void extendTmpIndices(const QVector<int> &indices) { m_indices.append(indices); }

    const QVector<int> &indices() { return m_indices; }

    void clearIndices() { m_indices.clear(); }

    int indexCount() const { return m_indexCount; }

    void setIndexCount(int count) { m_indexCount = count; }

    int indexOffset() const { return m_indexOffset; }

    void setIndexOffset(int index) { m_indexOffset = index; }

private:
    QVector<int> m_indices;  // 合并到 node 的 vertexBuffer 后成为空的
    int m_indexCount;
    int m_indexOffset;
};

#endif //QSPACE_MESH_H
