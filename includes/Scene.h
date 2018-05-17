#ifndef QSPACE_SCENE_H
#define QSPACE_SCENE_H

#include <QtCore/QVector>

#include "Node.h"

class Scene {
public:
    Scene() = default;

    const QVector<float> &vertices() const { return m_vertices; }

    void addToVertices(float val) { m_vertices.push_back(val); }

    const QVector<float> &uvs() const { return m_uvs; }

    void addToUVs(float val) { m_uvs.push_back(val); }

    const QVector<float> &normals() const { return m_normals; }

    void addToNormals(float val) { m_normals.push_back(val); }

    const QVector<unsigned int> &indices() const { return m_indices; }

    void addToIndices(unsigned int val) { m_indices.push_back(val); }

    const QVector<QSharedPointer<Material>> &materials() const { return m_materials; }

    QSharedPointer<Material> getMaterialAt(unsigned int index) { return m_materials[index]; }

    const QSharedPointer<Material> getMaterialAt(unsigned int index) const { return m_materials[index]; }

    void addToMaterials(QSharedPointer<Material> &material) { m_materials.push_back(material); }

    const QVector<QSharedPointer<Mesh>> &meshes() const { return m_meshes; }

    QSharedPointer<Mesh> getMeshAt(unsigned int index) { return m_meshes[index]; }

    const QSharedPointer<Mesh> getMeshAt(unsigned int index) const { return m_meshes[index]; }

    void addToMeshes(QSharedPointer<Mesh> &mesh) { m_meshes.push_back(mesh); }

    const QSharedPointer<Node> &rootNode() const { return m_rootNode; }

    void setRootNode(QSharedPointer<Node> &rootNode) { m_rootNode = rootNode; }

private:
    QVector<float> m_vertices;
    QVector<float> m_normals;
    QVector<float> m_uvs;
    QVector<unsigned int> m_indices;
    QVector<QSharedPointer<Material>> m_materials;
    QVector<QSharedPointer<Mesh>> m_meshes;
    QSharedPointer<Node> m_rootNode;
};

#endif //QSPACE_SCENE_H
