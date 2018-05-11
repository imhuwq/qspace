#ifndef QSPACE_MODELLOADER_H
#define QSPACE_MODELLOADER_H

#include <QString>
#include <QVector>
#include <QSharedPointer>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "Node.h"

class ModelLoader {
public:
    ModelLoader();

    bool Load(const QString &pathToFile);

    void getBufferData(QVector<float> **vertices, QVector<float> **normals, QVector<unsigned int> **indices);

    const QSharedPointer<Node> getNodeData() { return m_rootNode; };

private:
    QVector<float> m_vertices;
    QVector<float> m_normals;
    QVector<unsigned int> m_indices;
    QVector<QSharedPointer<Material>> m_materials;
    QVector<QSharedPointer<Mesh>> m_meshes;
    QSharedPointer<Node> m_rootNode;

    QSharedPointer<Material> processMaterial(aiMaterial *ai_material);

    QSharedPointer<Mesh> processMesh(aiMesh *ai_mesh);

    void ProcessNode(const aiScene *ai_scene, aiNode *ai_node, Node *parentNode, Node &newNode);

    void transformToUnitCoordinates();

    void findObjectDimensions(Node *node, QMatrix4x4 transformation, QVector3D &minDimension, QVector3D &maxDimension);
};

#endif //QSPACE_MODELLOADER_H
