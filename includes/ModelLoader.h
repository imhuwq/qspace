#ifndef QSPACE_MODELLOADER_H
#define QSPACE_MODELLOADER_H

#include <QDir>
#include <QString>
#include <QVector>
#include <QFileInfo>
#include <QSharedPointer>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "Node.h"
#include "Scene.h"

class ModelLoader {
public:
    static QVector<QString> fileFormat;

    bool checkFileFormatIsOk(const QString &filePath);

    bool Load(const QString &pathToFile, QSharedPointer<Scene> &scene);

private:
    QFileInfo m_modelFile;
    QDir m_modelDir;

    QSharedPointer<Scene> m_scene;

    QSharedPointer<Material> processMaterial(aiMaterial *ai_material);

    QSharedPointer<Mesh> processMesh(aiMesh *ai_mesh);

    void processNode(const aiScene *ai_scene, aiNode *ai_node, QSharedPointer<Node> parentNode, QSharedPointer<Node> newNode);
};


#endif //QSPACE_MODELLOADER_H
