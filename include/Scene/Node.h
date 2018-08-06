#ifndef QSPACE_NODE_H
#define QSPACE_NODE_H

#include <QMap>
#include <QString>
#include <QVector>
#include <QMatrix4x4>
#include <QSharedPointer>

#include "Mesh.h"

using namespace std;

class Node {
public:
    Node(const QString &name) : m_name(name) {}

    const QString &name() const { return m_name; }

    const QMatrix4x4 &getTransformation() {
        if (m_dirty) {
            m_dirty = false;
            m_transformation.setToIdentity();
            m_transformation.translate(m_position[0], m_position[1], m_position[2]);
            QQuaternion quaternion = QQuaternion::fromEulerAngles(m_rotation[0], m_rotation[1], m_rotation[2]);
            m_transformation.rotate(quaternion);
            m_transformation.scale(m_scale[0], m_scale[1], m_scale[2]);
        }
        return m_transformation;
    }

    void setPosition(const QVector3D &position) {
        m_dirty = true;
        m_position = position;
    }

    QVector3D getPosition() { return m_position; }

    void setRotation(const QVector3D &rotation) {
        m_dirty = true;
        m_rotation = rotation;
    }

    QVector3D getRotation() { return m_rotation; }


    void setScale(const QVector3D &scale) {
        m_dirty = true;
        m_scale = scale;
    }

    QVector3D getScale() { return m_scale; }

    QVector<QSharedPointer<Mesh> > meshes() { return m_meshes.values().toVector(); }

    bool hasMesh(const QString &materialName) { return m_meshes.contains(materialName); }

    QSharedPointer<Mesh> getMesh(const QString &materialName) { return m_meshes[materialName]; }

    void setMesh(QSharedPointer<Mesh> mesh, const QString &materialName) { m_meshes[materialName] = mesh; }

    QVector<QSharedPointer<Node>> &nodes() { return m_nodes; }

    void addNode(const QSharedPointer<Node> &node) { m_nodes.push_back(node); }

    QSharedPointer<Node> &getNodeAt(unsigned int index) { return m_nodes[index]; }

    void scaleByRatio(double ratio) { m_scale *= ratio; }

private:
    QString m_name;
    bool m_dirty = false;
    QVector3D m_position;
    QVector3D m_rotation;
    QVector3D m_scale;
    QMatrix4x4 m_transformation;
    QMap<QString, QSharedPointer<Mesh>> m_meshes;  // material_name --> mesh
    QVector<QSharedPointer<Node>> m_nodes;
};

typedef QSharedPointer<Node> NodePtr;
#endif //QSPACE_NODE_H
