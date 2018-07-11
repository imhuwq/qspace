#ifndef QSPACE_VERTEXBUFFER_H
#define QSPACE_VERTEXBUFFER_H

#include <QtCore/QVector>

struct VertexBuffer {
    QVector<int> m_indices = {};
    QVector<double> m_positions = {};
    QVector<double> m_normals = {};
    QVector<double> m_uv0s = {};
    QVector<double> m_uv1s = {};
    QVector<int> m_colors = {};

    VertexBuffer() {
        m_indices.resize(0);
        m_positions.resize(0);
        m_normals.resize(0);
        m_uv0s.resize(0);
        m_uv1s.resize(0);
        m_colors.resize(0);
    }

    const QVector<int> &indices() const { return m_indices; }

    void extendIndices(const QVector<int> &indices) {
        int offset = m_indices.size();
        for (const int index:indices) {
            m_indices.push_back(index + offset);
        }
    }

    void extendPositions(const QVector<double> &positions) {
        m_positions.append(positions);
    }

    void extendNormals(const QVector<double> &normals) { m_normals.append(normals); }

    void extendUV0s(QVector<double> &uv0s) { m_uv0s.append(uv0s); }

    void extendUV1s(QVector<double> &uv1s) { m_uv0s.append(uv1s); }

    void extendColors(QVector<int> &colors) { colors.append(colors); }

    int getIndicesSize() { return m_indices.size(); }
};

#endif //QSPACE_VERTEXBUFFER_H
