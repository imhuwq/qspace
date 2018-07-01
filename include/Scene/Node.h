#ifndef QSPACE_NODE_H
#define QSPACE_NODE_H

#include <QMap>
#include <QString>
#include <QVector>
#include <QMatrix4x4>

#include "Mesh.h"

using namespace std;

class Node {
public:
    Node(const QString &name) : m_name(name) {}

    const QString &name() const { return m_name; }

    const QMatrix4x4 &transformation() const { return m_transformation; }

    void setTransformation(const QMatrix4x4 &transformation) { m_transformation = transformation; }

    const QVector<QSharedPointer<Mesh> > &meshes() const { return m_meshes.values().toVector(); }

    bool hasMesh(const QString &materialName) { return m_meshes.contains(materialName); }

    QSharedPointer<Mesh> getMesh(const QString &materialName) { return m_meshes[materialName]; }

    void setMesh(QSharedPointer<Mesh> mesh, const QString &materialName) { m_meshes[materialName] = mesh; }

    const QVector<QSharedPointer<Node>> &nodes() const { return m_nodes; }

    void addNode(const QSharedPointer<Node> &node) { m_nodes.push_back(node); }

    QSharedPointer<Node> &getNodeAt(unsigned int index) { return m_nodes[index]; }

private:
    QString m_name;
    QMatrix4x4 m_transformation;
    QMap<QString, QSharedPointer<Mesh>> m_meshes;  // material_name --> mesh
    QVector<QSharedPointer<Node>> m_nodes;
};

#endif //QSPACE_NODE_H
