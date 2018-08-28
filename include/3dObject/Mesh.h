#ifndef QSPACE_MESH_H
#define QSPACE_MESH_H

#include <QString>
#include <QSharedPointer>

#include "Node.h"
#include "Scene/Material.h"
#include "Scene/Geometry.h"

class Mesh;

typedef QSharedPointer<Mesh> MeshPtr;

typedef QSharedPointer<const Mesh> kMeshPtr;

class FbxLoader;

class Mesh : public Node {
    friend FbxLoader;

public:
    explicit Mesh(const QString &name, NodeType type = NodeType::kMesh) : Node(name, type) {}

    void AddGeometry(const GeometryPtr &geometry) { geometries_[geometry->GetMaterialName()] = geometry; }

    bool HasGeometry(const QString &material_name) { return geometries_.contains(material_name); }

    kGeometryPtr GetGeometry(const QString &material_name) const { return geometries_[material_name]; }

    QVector<kGeometryPtr> GetGeometries() const {
        QVector<kGeometryPtr> geometries;
        for (kGeometryPtr geometry: geometries_.values()) geometries.push_back(geometry);
        return geometries;
    }

private:
    GeometryPtr GetGeometry(const QString &material_name) { return geometries_[material_name]; }

    GeometryPtr GetOrCreateGeometry(const QString &material_name) {
        GeometryPtr geometry;
        if (!HasGeometry(material_name)) {
            geometry = GeometryPtr(new Geometry(material_name));
            AddGeometry(geometry);
        } else {
            geometry = GetGeometry(material_name);
        }
        return geometry;
    }

    QVector<GeometryPtr> GetGeometries() {
        QVector<GeometryPtr> geometries;
        for (GeometryPtr geometry: geometries_.values()) geometries.push_back(geometry);
        return geometries;
    }

    QMap<QString, GeometryPtr> geometries_;
};

#endif //QSPACE_MESH_H
