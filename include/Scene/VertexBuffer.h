#ifndef QSPACE_VERTEXBUFFER_H
#define QSPACE_VERTEXBUFFER_H

#include <QtCore/QVector>

class VertexBuffer {
public:
    const QVector<unsigned int> &indices() const { return m_indices; }

    void extendIndices(const QVector<unsigned int> &indices) {
        unsigned int offset = m_indices.size();
        for (const unsigned int index:indices) {
            m_indices.push_back(index + offset);
        }
    }

    const QVector<double> &positions() const { return m_positions; }

    void extendPositions(const QVector<double> &positions) { m_positions.append(positions); }

    const QVector<double> &normals() const { return m_normals; }

    void extendNormals(const QVector<double> &normals) { m_normals.append(normals); }

    const QVector<double> &uv0s() const { return m_uv0s; }

    void extendUV0s(QVector<double> &uv0s) { m_uv0s.append(uv0s); }

    const QVector<double> &uv1s() const { return m_uv1s; }

    void extendUV1s(QVector<double> &uv1s) { m_uv0s.append(uv1s); }

    const QVector<int> &colors() const { return m_colors; }

    void extendColors(QVector<int> &colors) { colors.append(colors); }

    unsigned int getIndicesSize() { return m_indices.size(); }

private:
    QVector<unsigned int> m_indices = {};
    QVector<double> m_positions = {};
    QVector<double> m_normals = {};
    QVector<double> m_uv0s = {};
    QVector<double> m_uv1s = {};
    QVector<int> m_colors = {};
};

#endif //QSPACE_VERTEXBUFFER_H
