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

    void setPosition(const QVector<double> &position) { m_position = position; }

    void setRotation(const QVector<double> &rotation) { m_rotation = rotation; }

    void setScale(const QVector<double> &scale) { m_scale = scale; }

    void setTransformation(const QMatrix4x4 &transformation) {
        m_transformation.setToIdentity();
        m_transformation.translate(m_position[0], m_position[1], m_position[2]);
        m_transformation.rotate(m_rotation[0], m_rotation[1], m_rotation[2]);
        m_transformation.scale(m_scale[0], m_scale[1], m_scale[2]);
    }

    QVector<QSharedPointer<Mesh> > meshes() { return m_meshes.values().toVector(); }

    bool hasMesh(const QString &materialName) { return m_meshes.contains(materialName); }

    QSharedPointer<Mesh> getMesh(const QString &materialName) { return m_meshes[materialName]; }

    void setMesh(QSharedPointer<Mesh> mesh, const QString &materialName) { m_meshes[materialName] = mesh; }

    QVector<QSharedPointer<Node>> &nodes() { return m_nodes; }

    void addNode(const QSharedPointer<Node> &node) { m_nodes.push_back(node); }

    QSharedPointer<Node> &getNodeAt(unsigned int index) { return m_nodes[index]; }

    void scaleByRatio(double ratio) {
        for (auto &v: m_scale) {
            v *= ratio;
        };
    }

private:
    QString m_name;
    QVector<double> m_position;
    QVector<double> m_rotation;
    QVector<double> m_scale;
    QMatrix4x4 m_transformation;
    QMap<QString, QSharedPointer<Mesh>> m_meshes;  // material_name --> mesh
    QVector<QSharedPointer<Node>> m_nodes;
};

#endif //QSPACE_NODE_H
