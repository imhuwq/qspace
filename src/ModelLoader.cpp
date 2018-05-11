#include "ModelLoader.h"

bool ModelLoader::Load(const QString &pathToFile) {
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
            m_materials.push_back(processMaterial(ai_scene->mMaterials[index]));
        }
    }

    if (ai_scene->HasMeshes()) {
        for (unsigned int index = 0; index < ai_scene->mNumMeshes; index++) {
            m_meshes.push_back(processMesh(ai_scene->mMeshes[index]));
        }
    } else {
        qDebug() << "Error: No meshes found";
        return false;
    }
}

QSharedPointer<Material> ModelLoader::processMaterial(aiMaterial *ai_material) {
    QSharedPointer<Material> material(new Material);
    aiString ai_mat_name;
    ai_material->Get(AI_MATKEY_NAME, ai_mat_name);
    if (ai_mat_name.length > 0) {
        material->setName(ai_mat_name.C_Str());
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
    }

    return material;
}

QSharedPointer<Mesh> ModelLoader::processMesh(aiMesh *ai_mesh) {
    QSharedPointer<Mesh> mesh(new Mesh);
    QString meshName = ai_mesh->mName.length > 0 ? ai_mesh->mName : "";
    mesh->setName(meshName);
    mesh->setIndexOffset(m_indices.size());
}