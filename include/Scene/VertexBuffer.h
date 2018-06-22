#ifndef QSPACE_VERTEXBUFFER_H
#define QSPACE_VERTEXBUFFER_H

#include <QtCore/QVector>

class VertexBuffer {
public:
    const QVector<unsigned int> &indices() const { return m_indices; }

    void addToIndices(unsigned int index) { m_indices.push_back(index); }

    const QVector<double> &positions() const { return m_positions; }

    void addToPositions(double position) { m_positions.push_back(position); }

    void extendPositions(const QVector<double> &positions) { m_positions.append(positions); }

    const QVector<double> &normals() const { return m_normals; }

    void addToNormals(double normal) { m_normals.push_back(normal); }

    void extendNormals(const QVector<double> &normals) { m_normals.append(normals); }

    const QVector<double> &uv0s() const { return m_uv0s; }

    void addToUV0s(double uv0) { m_uv0s.push_back(uv0); }

    void extendUV0s(QVector<double> &uv0s) { m_uv0s.append(uv0s); }

    const QVector<double> &uv1s() const { return m_uv1s; }

    void addToUV10s(double uv1) { m_uv1s.push_back(uv1); }

    void extendUV1s(QVector<double> &uv1s) { m_uv0s.append(uv1s); }

    const QVector<int> &colors() const { return m_colors; }

    void addToUV10s(int color) { m_uv1s.push_back(color); }

    void extendColors(QVector<int> &colors) { colors.append(colors); }

private:
    QVector<unsigned int> m_indices = {};
    QVector<double> m_positions = {};
    QVector<double> m_normals = {};
    QVector<double> m_uv0s = {};
    QVector<double> m_uv1s = {};
    QVector<int> m_colors = {};
};

#endif //QSPACE_VERTEXBUFFER_H
