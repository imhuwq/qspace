#ifndef QSPACE_NODE_H
#define QSPACE_NODE_H

#include <QString>
#include <QMatrix4x4>
#include <QVector>
#include <iostream>

#include "Mesh.h"

using namespace std;

class Node {
public:
    const QString &name() const { return m_name; }

    void setName(const QString &name) { m_name = name; }

    const QMatrix4x4 &transformation() const { return m_transformation; }

    void setTransformation(const QMatrix4x4 &transformation) { m_transformation = transformation; }

    const QVector<QSharedPointer<Mesh> > &meshes() const { return m_meshes; }

    const QSharedPointer<Mesh> getMeshAt(unsigned int index) const { return m_meshes[index]; }

    QSharedPointer<Mesh> getMeshAt(unsigned int index) { return m_meshes[index]; }

    void resizeMeshes(unsigned int size) { m_meshes.resize(size); }

    void setMeshAt(QSharedPointer<Mesh> mesh, unsigned int index) { m_meshes[index] = mesh; }

    const QVector<QSharedPointer<Node>> &nodes() const { return m_nodes; }

    void addNode(const QSharedPointer<Node> &node) { m_nodes.push_back(node); }

    QSharedPointer<Node> &getNodeAt(unsigned int index) { return m_nodes[index]; }

private:
    QString m_name;
    QMatrix4x4 m_transformation;
    QVector<QSharedPointer<Mesh>> m_meshes;
    QVector<QSharedPointer<Node>> m_nodes;
};

#endif //QSPACE_NODE_H
