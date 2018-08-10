#ifndef QSPACE_GEOMETRY_H
#define QSPACE_GEOMETRY_H

#include <QVector>
#include <QString>
#include <QSharedPointer>

using namespace std;

class Geometry;

typedef QSharedPointer<Geometry> GeometryPtr;

typedef QSharedPointer<const Geometry> kGeometryPtr;

class Geometry {
public:
    explicit Geometry(QString material_name) : material_name_(move(material_name)) {}

    QString GetMaterialName() { return material_name_; }

    void ExtendIndices(QVector<unsigned int> indices) { indices_.append(indices); }

    void SetIndexOffset(int offset) { index_offset_ = offset; }

    int GetIndexOffset() const { return index_offset_; }

    int GetIndicesSize() const { return indices_.size(); }

    const QVector<unsigned int> &GetIndices() const { return indices_; }

private:
    QString material_name_;
    QVector<unsigned int> indices_;
    int index_offset_ = 0;
};

#endif //QSPACE_GEOMETRY_H
