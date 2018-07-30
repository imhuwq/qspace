#ifndef QSPACE_VERTEXBUFFER_H
#define QSPACE_VERTEXBUFFER_H

#include <QtCore/QVector>

template<typename T>
void insertIntoVector(const QVector<T> &values, QVector<T> &receiver, int start, int count) {
    int minSize = start + count;
    if (receiver.size() < minSize) receiver.resize(minSize);
    move(values.begin(), values.begin() + count, receiver.begin() + start);
}

struct VertexBuffer {
    template<typename T>
    friend void insertIntoVector(const QVector<T> &values, QVector<T> &receiver, int start, int count);

    QVector<unsigned int> m_indices = {};
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

    const QVector<unsigned int> &indices() const { return m_indices; }

    void extendIndices(const QVector<int> &indices) {
        int offset = m_indices.size();
        for (const int index:indices) {
            m_indices.push_back(index + offset);
        }
    }

    void insertPositions(const QVector<double> &positions, int startIndex, int count) {
        insertIntoVector(positions, m_positions, startIndex, count);
    }

    void insertNormals(const QVector<double> &normals, int startIndex, int count) {
        insertIntoVector(normals, m_normals, startIndex, count);
    }

    void insertUV0s(QVector<double> &uv0s, int startIndex, int count) {
        insertIntoVector(uv0s, m_uv0s, startIndex, count);
    }

    void insertUV1s(QVector<double> &uv1s, int startIndex, int count) {
        insertIntoVector(uv1s, m_uv1s, startIndex, count);
    }

    void insertColors(QVector<int> &colors, int startIndex, int count) {
        insertIntoVector(colors, m_colors, startIndex, count);
    }

    int getIndicesSize() { return m_indices.size(); }
};

#endif //QSPACE_VERTEXBUFFER_H
