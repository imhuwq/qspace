#ifndef QSPACE_NODE_H
#define QSPACE_NODE_H

#include <QString>
#include <QMatrix4x4>
#include <QVector>

#include "Mesh.h"

class Node {
public:
    const QString &name() const {
        return m_name;
    }

    const QMatrix4x4 &transformation() const {
        return m_transformation;
    }

    const QVector<QSharedPointer<Mesh> > &meshes() const {
        return m_meshes;
    }

    const QVector<Node> &nodes() const {
        return m_nodes;
    }

private:
    QString m_name;
    QMatrix4x4 m_transformation;
    QVector<QSharedPointer<Mesh> > m_meshes;
    QVector<Node> m_nodes;
};

#endif //QSPACE_NODE_H
