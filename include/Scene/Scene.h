#ifndef QSPACE_MODEL_H
#define QSPACE_MODEL_H

#include <QMap>
#include <QString>
#include <QVector>
#include <QSharedPointer>

#include "Scene/Material.h"
#include "3dObject/Node.h"
#include "3dObject/Camera.h"
#include "Scene/VertexBuffer.h"

class Scene;

class FbxLoader;

typedef QSharedPointer<Scene> ScenePtr;

typedef QSharedPointer<const Scene> kScenePtr;

class Scene {
    friend FbxLoader;
public:
    Scene() {
        vertex_buffer_ = VertexBufferPtr(new VertexBuffer());
    }

    NodePtr GetModel() { return model_; }

    void SetModel(const NodePtr &model) { model_ = model; }

    void AddTextureFile(const QString &texture_path) { texture_files_[texture_path] = texture_path; }

    QString GetTextureFile(const QString &texture_path) { return texture_files_[texture_path]; }

    void AddMaterial(const MaterialPtr &material) { materials_[material->GetName()] = material; }

    kMaterialPtr GetMaterial(const QString &material_name) { return materials_[material_name]; }

    kVertexBufferPtr GetVertexBuffer() const { return vertex_buffer_; }

private:
    NodePtr model_ = nullptr;

    QMap<QString, QString> texture_files_;

    QMap<QString, MaterialPtr> materials_;

    VertexBufferPtr vertex_buffer_ = nullptr;

    VertexBufferPtr GetVertexBuffer() { return vertex_buffer_; }
};

#endif //QSPACE_MODEL_H
