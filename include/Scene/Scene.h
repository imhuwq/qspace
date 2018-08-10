#ifndef QSPACE_SCENE_H
#define QSPACE_SCENE_H

#include <QMap>
#include <QString>
#include <QVector>
#include <QSharedPointer>

#include "Node.h"
#include "Material.h"
#include "VertexBuffer.h"

class Scene;

class FbxLoader;

typedef QSharedPointer<Scene> ScenePtr;

typedef QSharedPointer<const Scene> kScenePtr;

class Scene {
    friend FbxLoader;
public:
    Scene() { vertex_buffer_ = VertexBufferPtr(new VertexBuffer()); }

    kNodePtr GetRootNode() const { return root_node_; }

    void SetRootNode(const NodePtr &rootNode) { root_node_ = rootNode; }

    void AddTextureFile(const QString &texture_path) { texture_files_[texture_path] = texture_path; }

    QString GetTextureFile(const QString &texture_path) { return texture_files_[texture_path]; }

    void AddMaterial(const MaterialPtr &material) { materials_[material->GetName()] = material; }

    kMaterialPtr GetMaterial(const QString &material_name) { return materials_[material_name]; }

    kVertexBufferPtr GetVertexBuffer() const { return vertex_buffer_; }

private:
    NodePtr root_node_;

    QMap<QString, QString> texture_files_;

    QMap<QString, MaterialPtr> materials_;

    VertexBufferPtr vertex_buffer_ = nullptr;

    VertexBufferPtr GetVertexBuffer() { return vertex_buffer_; }
};

#endif //QSPACE_SCENE_H
