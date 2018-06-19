#include "ModelLoader.h"

QVector<QString> ModelLoader::fileFormat = {"obj", "fbx"};

bool ModelLoader::checkFileFormatIsOk(const QString &filePath) {
    QFileInfo fileInfo(filePath);
    if (fileInfo.exists() && !fileInfo.isHidden()) {
        QString format = fileInfo.suffix().toLower();
        return fileFormat.contains(format);
    }
    return false;
}

bool ModelLoader::Load(const QString &pathToFile, QSharedPointer<Scene> &scene) {
    if (!checkFileFormatIsOk(pathToFile)) return false;
    m_modelFile = QFileInfo(pathToFile);
    m_modelDir = QDir(m_modelFile.absoluteDir());
    m_scene = scene;

    Assimp::Importer importer;
    const aiScene *ai_scene = importer.ReadFile(pathToFile.toStdString(),
                                                aiProcess_GenSmoothNormals |
                                                aiProcess_CalcTangentSpace |
                                                aiProcess_Triangulate |
                                                aiProcess_JoinIdenticalVertices |
                                                aiProcess_SortByPType);
    if (!ai_scene) {
        qDebug() << "Error loading file: (assimp:) " << importer.GetErrorString();
        return false;
    }

    if (ai_scene->HasMaterials()) {
        for (unsigned int index = 0; index < ai_scene->mNumMaterials; index++) {
            QSharedPointer<Material> material = processMaterial(ai_scene->mMaterials[index]);
            m_scene->addToMaterials(material);
        }
    }

    if (ai_scene->HasMeshes()) {
        for (unsigned int index = 0; index < ai_scene->mNumMeshes; index++) {
            QSharedPointer<Mesh> mesh = processMesh(ai_scene->mMeshes[index]);
            m_scene->addToMeshes(mesh);
        }
    } else {
        qDebug() << "Error: No meshes found";
        return false;
    }

    if (ai_scene->mRootNode != nullptr) {
        QSharedPointer<Node> rootNode(new Node);
        processNode(ai_scene, ai_scene->mRootNode, QSharedPointer<Node>(), rootNode);
        m_scene->setRootNode(rootNode);
    }
    return true;
}

QSharedPointer<Material> ModelLoader::processMaterial(aiMaterial *ai_material) {
    QSharedPointer<Material> material(new Material);
    aiString ai_mat_name;
    ai_material->Get(AI_MATKEY_NAME, ai_mat_name);
    if (ai_mat_name.length > 0) {
        material->setName(QString(ai_mat_name.C_Str()));
    }

    int shadingModel;
    ai_material->Get(AI_MATKEY_SHADING_MODEL, shadingModel);
    if (shadingModel != aiShadingMode_Phong && shadingModel != aiShadingMode_Gouraud) {
        qDebug() << "This mesh's shading model is not implemented in this loader, setting to default material";
        material->setName("DefaultMaterial");
    } else {
        aiColor3D dif(0.f, 0.f, 0.f);
        aiColor3D amb(0.f, 0.f, 0.f);
        aiColor3D spc(0.f, 0.f, 0.f);
        float shine = 0.0;

        material->setAmbient(QVector3D(amb.r * 0.2d, amb.g * 0.2d, amb.b * 0.2d));
        material->setDiffuse(QVector3D(dif.r, dif.g, dif.b));
        material->setSpecular(QVector3D(spc.r, spc.g, spc.b));
        shine == 0.0 ? material->setShininess(30.0f) : material->setShininess(shine);

        if (ai_material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString texturePath;
            if (ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS) {
                QFileInfo textPath = QFileInfo(texturePath.data);
                if (textPath.isRelative()) {
                    material->setDiffusePath(m_modelDir.filePath(texturePath.data));
                } else {
                    material->setDiffusePath(textPath.absolutePath());
                }
            } else {
                qDebug() << "Failed to get texture path for material";
            }
        }
    }

    return material;
}

QSharedPointer<Mesh> ModelLoader::processMesh(aiMesh *ai_mesh) {
    QSharedPointer<Mesh> mesh(new Mesh);
    aiString meshName = ai_mesh->mName.length > 0 ? ai_mesh->mName : aiString("");
    mesh->setName(meshName.C_Str());
    mesh->setIndexOffset((unsigned int) m_scene->indices().size());
    unsigned int indexCountBefore = (unsigned int) m_scene->indices().size();
    unsigned int vertexIndexOffset = (unsigned int) m_scene->indices().size() / 3;

    for (unsigned int index = 0; index < ai_mesh->mNumVertices; index++) {
        // position
        aiVector3D &vertVec = ai_mesh->mVertices[index];
        m_scene->addToVertices(vertVec.x);
        m_scene->addToVertices(vertVec.y);
        m_scene->addToVertices(vertVec.z);

        // normal
        aiVector3D &normVec = ai_mesh->mNormals[index];
        m_scene->addToNormals(normVec.x);
        m_scene->addToNormals(normVec.y);
        m_scene->addToNormals(normVec.z);

        // uv
        if (ai_mesh->HasTextureCoords(0)) {
            aiVector3D &textVec = ai_mesh->mTextureCoords[0][index];
            m_scene->addToUVs(textVec.x);
            m_scene->addToUVs(textVec.y);
        }
    }

    for (unsigned int index = 0; index < ai_mesh->mNumFaces; index++) {
        aiFace *ai_face = &ai_mesh->mFaces[index];
        if (ai_face->mNumIndices != 3) {
            qDebug() << "Warning: Mesh face with not exactly 3 indices, ignoring this primitive.";
            continue;
        }

        m_scene->addToIndices(ai_face->mIndices[0] + vertexIndexOffset);
        m_scene->addToIndices(ai_face->mIndices[1] + vertexIndexOffset);
        m_scene->addToIndices(ai_face->mIndices[2] + vertexIndexOffset);
    }

    mesh->setIndexCount(m_scene->indices().size() - indexCountBefore);

    mesh->setMaterial(m_scene->getMaterialAt(ai_mesh->mMaterialIndex));

    return mesh;
}

void ModelLoader::processNode(const aiScene *ai_scene, aiNode *ai_node, QSharedPointer<Node> parentNode, QSharedPointer<Node> node) {
    QString name = ai_node->mName.length != 0 ? ai_node->mName.C_Str() : "";
    node->setName(name);
    node->setTransformation(QMatrix4x4(ai_node->mTransformation[0]));
    node->resizeMeshes(ai_node->mNumMeshes);

    for (unsigned int index = 0; index < ai_node->mNumMeshes; index++) {
        QSharedPointer<Mesh> mesh = m_scene->getMeshAt(ai_node->mMeshes[index]);
        node->setMeshAt(mesh, index);
    }

    for (unsigned int index = 0; index < ai_node->mNumChildren; index++) {
        QSharedPointer<Node> childNode(new Node);
        node->addNode(childNode);
        processNode(ai_scene, ai_node->mChildren[index], node, childNode);
    }
}
