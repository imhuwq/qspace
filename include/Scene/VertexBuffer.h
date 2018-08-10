#ifndef QSPACE_VERTEXBUFFER_H
#define QSPACE_VERTEXBUFFER_H

#include <QVector>
#include <QSharedPointer>

using namespace std;

template<typename T>
void InsertIntoVector(const QVector<T> &values, QVector<T> &receiver, int start, int count) {
    int minSize = start + count;
    if (receiver.size() < minSize) receiver.resize(minSize);
    move(values.begin(), values.begin() + count, receiver.begin() + start);
}

class VertexBuffer;

typedef QSharedPointer<VertexBuffer> VertexBufferPtr;

typedef QSharedPointer<const VertexBuffer> kVertexBufferPtr;

class VertexBuffer {
public:
    int GetIndicesSize() { return indices_.size(); }

    void ExtendIndices(const QVector<unsigned int> &indices) {
        unsigned int offset = (unsigned int) indices_.size();
        for (unsigned int index:indices) {
            indices_.push_back(index + offset);
        }
    }

    void InsertPositions(const QVector<double> &positions, int startIndex, int count) {
        InsertIntoVector(positions, positions_, startIndex, count);
    }

    void InsertNormals(const QVector<double> &normals, int startIndex, int count) {
        InsertIntoVector(normals, normals_, startIndex, count);
    }

    void InsertUV0(QVector<double> &uv0s, int startIndex, int count) {
        InsertIntoVector(uv0s, uv0s, startIndex, count);
    }

    void InsertUV1(QVector<double> &uv1s, int startIndex, int count) {
        InsertIntoVector(uv1s, uv1_, startIndex, count);
    }

    void InsertColors(QVector<int> &colors, int startIndex, int count) {
        InsertIntoVector(colors, colors_, startIndex, count);
    }

    const QVector<unsigned int> &GetIndices() const { return indices_; }

    const QVector<double> &GetPositions() const { return positions_; }

    const QVector<double> &GetNormals() const { return normals_; }

    const QVector<double> &GetUV0() const { return uv0_; }

    const QVector<double> &GetUV1() const { return uv1_; }

    const QVector<int> &GetColors() const { return colors_; }

private:
    QVector<unsigned int> indices_ = {};
    QVector<double> positions_ = {};
    QVector<double> normals_ = {};
    QVector<double> uv0_ = {};
    QVector<double> uv1_ = {};
    QVector<int> colors_ = {};
};

#endif //QSPACE_VERTEXBUFFER_H
