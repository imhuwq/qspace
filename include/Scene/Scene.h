#ifndef QSPACE_SCENE_H
#define QSPACE_SCENE_H

#include <QMap>
#include <QString>
#include <QVector>

#include "Node.h"
#include "Material.h"
#include "TextureFile.h"
#include "VertexBuffer.h"

class ModelLoader;

class Scene {
    friend ModelLoader;
public:
    Scene() {
        m_vertexBuffer = QSharedPointer<VertexBuffer>(new VertexBuffer());
    }

    void addMaterial(const QSharedPointer<Material> &material) { m_materials[material->name()] = material; }

    QSharedPointer<Material> getMaterial(const QString &material_name) { return m_materials[material_name]; }

    void addTextureFile(QSharedPointer<TextureFile> &texture) { m_texture_files[texture->path()] = texture; }

    QSharedPointer<TextureFile> getTextureFile(const QString &texturePath) { return m_texture_files[texturePath]; }

    const QSharedPointer<VertexBuffer> getVertexBuffer() const { return m_vertexBuffer; }

    void setVertexBuffer(QSharedPointer<VertexBuffer> &vertexBuffer) { m_vertexBuffer = vertexBuffer; }

    const QSharedPointer<Node> rootNode() const { return m_rootNode; }

    void setRootNode(QSharedPointer<Node> &rootNode) { m_rootNode = rootNode; }

    unsigned int getIndicesSize() { return m_vertexBuffer->getIndicesSize(); }

    QSharedPointer<const VertexBuffer> getVertexBufferConst() { return m_vertexBuffer; }

private:
    QMap<QString, QSharedPointer<Material>> m_materials;  // material_name -> material_obj
    QMap<QString, QSharedPointer<TextureFile>> m_texture_files;    // texture_file_path -> texture_file_obj
    QSharedPointer<VertexBuffer> m_vertexBuffer;
    QSharedPointer<Node> m_rootNode;

    QSharedPointer<VertexBuffer> getVertexBuffer() { return m_vertexBuffer; }
};

#endif //QSPACE_SCENE_H
