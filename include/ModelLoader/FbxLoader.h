#ifndef QSPACE_MODELLOADER_H
#define QSPACE_MODELLOADER_H

#include <QDir>
#include <QDebug>
#include <QString>
#include <QVector>
#include <QFileInfo>
#include <QSharedPointer>
#include <fbxsdk.h>

#include "3dObject/Node.h"
#include "3dObject/Mesh.h"
#include "Scene/Scene.h"
#include "Scene/Material.h"
#include "Scene/Texture.h"

struct Vertex {
    QVector<double> positions = {};
    QVector<double> normals = {};
    QVector<double> uv0 = {};
    QVector<double> uv1 = {};
    QVector<int> colors = {};
    int index;
};

inline bool operator==(const Vertex &lhs, const Vertex &rhs) {
    return lhs.positions == rhs.positions and
           lhs.normals == rhs.normals and
           lhs.uv0 == rhs.uv0 and
           lhs.uv1 == rhs.uv1 and
           lhs.colors == rhs.colors;
}

inline bool operator!=(const Vertex &lhs, const Vertex &rhs) {
    return !(lhs == rhs);
}

struct VertexContext {
    int polygon_index_in_mesh = 0;                  // 多边形在 mesh 上的 index
    int control_point_index_in_polygon = 0;         // control point 在多边形上的 index
    int control_point_index_in_mesh = 0;            // control point 在 mesh 上的 index
    int vertex_index_in_mesh = 0;                   // vertex 在 mesh 上的 index
    int vertex_id_in_mesh = 0;                      // vertex 在 mesh 上的 id
    QVector<unsigned int> vertex_indices_of_polygon = {};    // 一个多边形的所有 vertex 的 index
};

typedef QSharedPointer<VertexContext> VertexContextPtr;

struct TextureContext {
    TextureContext(QString channel, int layer, TextureBlendMode blend) : texture_channel_(move(channel)), layer_index_(layer), blend_mode_(blend) {}

    QString texture_channel_ = "";
    int layer_index_ = 0;
    TextureBlendMode blend_mode_ = TextureBlendMode::kNone;
};

typedef QSharedPointer<TextureContext> TextureContextPtr;

typedef QSharedPointer<Vertex> VertexPtr;

struct VertexTable {
    int size = 0;

    vector<vector<VertexPtr >> table;

    explicit VertexTable(const int vertex_count) : size(vertex_count) { table.assign(vertex_count, {}); }

    bool Insert(VertexPtr &vertex, VertexContextPtr &vertex_context) {
        vector<VertexPtr> &vertices = table[vertex_context->control_point_index_in_mesh];

        if (vertices.empty()) {
            vertex_context->vertex_index_in_mesh = vertex_context->control_point_index_in_mesh;
            vertex->index = vertex_context->vertex_index_in_mesh;
            vertices.push_back(vertex);
            return true;
        }

        for (VertexPtr &v: vertices) {
            if (*v == *vertex) {
                vertex_context->vertex_index_in_mesh = v->index;
                return false;
            }
        }

        vertex_context->vertex_index_in_mesh = size++;
        vertex->index = vertex_context->vertex_index_in_mesh;
        vertices.push_back(vertex);
        return true;
    }
};

class FbxLoader {
public:
    QVector<QString> GetFileFormat() { return {"fbx",}; }

    bool CheckFormatIsSupported(const QString &file_path);

    bool Load(const QString &model_file, ScenePtr scene);

    NodePtr CreateNode(FbxNode *const &fbx_node);

private:
    QFileInfo model_file_;

    QDir model_dir_;

    FbxManager *fbx_manager_;

    FbxScene *fbx_scene_;

    ScenePtr scene_;

    VertexBufferPtr vertex_buffer_;

    bool ImportModel();

    bool InitializeFbxSdkObjects();

    bool DestroyFbxSdkObjects();

    static bool IsMeshNode(FbxNode *fbx_node);

    void ProcessTextureFiles();

    void CollectMaterialData(FbxSurfaceMaterial *fbx_material, MaterialPtr material);

    void CollectTextureData(FbxTexture *fbx_texture, TextureContextPtr texture_context, MaterialPtr material);

    void CollectTextureData(FbxSurfaceMaterial *fbx_material, MaterialPtr material);

    void ProcessNodeMaterials(FbxNode *fbx_node, MeshPtr node);

    void ProcessNodeTRS(FbxNode *fbx_node, NodePtr node);

    void PrepareFbxMesh(FbxMesh *&fbx_mesh);

    void CollectVertexPosition(FbxMesh *fbx_mesh, VertexPtr vertex, VertexContextPtr vertex_context);

    void CollectVertexNormal(FbxMesh *fbx_mesh, VertexPtr vertex, VertexContextPtr vertexContext);

    void CollectVertexTexCoords(FbxMesh *fbx_mesh, VertexPtr vertex, VertexContextPtr vertexContext);

    void CollectVertexColor(FbxMesh *fbx_mesh, VertexPtr vertex, VertexContextPtr vertexContext);

    void CollectVertexBufferData(FbxMesh *fbx_mesh, VertexTable vertex_table, VertexContextPtr vertexContext, VertexBufferPtr vertex_buffer);

    void CollectGeometryData(FbxMesh *fbx_mesh, VertexContextPtr vertexContext, MeshPtr node);

    void ProcessNodeGeometry(FbxNode *fbx_node, MeshPtr node);

    void ProcessNodeAllData(FbxNode *fbx_node, NodePtr node);

    void ProcessNode(FbxNode *fbx_node, NodePtr node);

    void ProcessNodes();

    double GetScaleRatio();
};

typedef QSharedPointer<FbxLoader> FbxLoaderPtr;

typedef QSharedPointer<const FbxLoader> kFbxLoaderPtr;

#endif //QSPACE_MODELLOADER_H
