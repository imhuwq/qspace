#ifndef QSPACE_MODELLOADER_H
#define QSPACE_MODELLOADER_H

#include <QDir>
#include <QDebug>
#include <QString>
#include <QVector>
#include <QFileInfo>
#include <QSharedPointer>
#include <fbxsdk.h>

#include "Scene/Node.h"
#include "Scene/Mesh.h"
#include "Scene/Scene.h"


class ModelLoader {
public:
    QVector<QString> fileFormat() { return {"fbx",}; }

    bool checkFileFormatIsSupported(const QString &filePath);

    bool load(const QString &pathToFile, QSharedPointer<Scene> &scene);

protected:
    struct TextureContext {
        TextureContext(const char *channel, int layer, int blend) : m_textureChannel(channel), m_layerIndex(layer),
                                                                    m_blendMode(blend) {}

        const char *m_textureChannel = nullptr;
        int m_layerIndex = 0;
        int m_blendMode = 0;
    };

    typedef QSharedPointer<TextureContext> TextureContextPtr;

    struct VertexContext {
        VertexContext() { m_vertexBuffer = QSharedPointer<VertexBuffer>(new VertexBuffer()); }

        int m_polygonIndex2Mesh = 0;          // 多边形在 mesh 上的 index
        int m_controlPointIndex2Polygon = 0;  // control point 在多边形上的 index
        int m_controlPointIndex2Mesh = 0;     // control point 在 mesh 上的 index
        int m_vertexIndex2Mesh = 0;           // vertex 在 mesh 上的 index
        int m_vertexId2Mesh = 0;              // vertex 在 mesh 上的 id
        QVector<unsigned int> m_vertexIndicesOfPolygon = {};    // 一个多边形的所有 vertex index
        QSharedPointer<VertexBuffer> m_vertexBuffer;   // mesh 搜集到的 vertex buffer
    };

    struct SingleVertex {
        QVector<double> m_positions = {};
        QVector<double> m_normals = {};
        QVector<double> m_uv0s = {};
        QVector<double> m_uv1s = {};
        QVector<int> m_colors = {};
        int m_index;
    };

    friend bool operator==(const SingleVertex &lhs, const SingleVertex &rhs);

    friend bool operator!=(const SingleVertex &lhs, const SingleVertex &rhs);

    typedef QSharedPointer<SingleVertex> SingleVertexPtr;

    /**
     * 在 Fbx 中, 一个点是一个只由 position 区分的 control point. 每个 control point 都是独一无二的.
     * 在我们需要的数据中, 一个点是综合 position, normal, uv, color 等所有这些数据来区分的 vertex. 每个 vertex 也应该独一无二以节省资源.
     * 同一个 control point, 在不同的面中, 如果有了不同的 normal 等这些数据, 就产生了多个 vertex. 一个 control point 可能对应一个 vertex, 可能多个 vertex.
     * 这个 Class 的作用就是根据 position, normal, uv0, uv1 以及 color 来确定一个 vertex 以它的 index
     */
    struct VertexTable {
        // mSize 用来说明该 table 已经插入了多少个除 control point 以外的个数的 vertex, mSize = 已插入的所有 vertex 个数 - control point 个数
        // mSize 会被初始化为 control point 个数的大小
        int m_size = 0;

        // mTable 里面记录了所有已经插入的点.
        // 它的结构类似一个 linked-list 的 hash-table.
        // 已知每个 control point 对应一个或者多个 vertex, 在这里体现为, 每个 control point 对应一个 vertex 的 vector
        // mesh 上 index 为 n 的 control point 对应的 vertex vector 为 table[n]
        vector<vector<SingleVertexPtr >> mTable;

        explicit VertexTable(const int vertexCount) : m_size(vertexCount) {
            mTable.assign(vertexCount, {});
        }

        /**
         * 把一个 Vertex 插入到当前 control point 的 vertex vector 里面去, 并且获得它在 mesh 里面的 index. control point 由 gControlPointIndexInMesh 确定
         * 如果当前 control point 的 vertex vector 是空的, 不妨把这个 vertex 就看做 control point, 它的 index 就是 control point 在 mesh 里面的 index
         * 如果当前 control point 的 vertex vector 不是空的, 并且里面有和 vertex 相等的, 那说明 vertex 是一个重复的 vertex, 它的 index 就是之前的 index
         * 如果当前 control point 的 vertex vector 不是空的, 并且里面没有和 vertex 相等的, 那说明 vertex 是一个新的 vertex, 它的 index 就是 mSize
         * 我们可以确定获得的 index 是正确的. 为什么呢? 上述有三个 "如果" 的情况:
         * 对于情况 1, 每个 control point 必然对应至少一个 vertex, 所以每个 control point 的 index 必然出现一次, 所以前 control point -1 个 index 必然会被使用
         * 对于情况 3, 当 control point 出现了对应的第 2 个及以上个 "新的" vertex 时, index 是 mSize.
         *            mSize 是从 control point 个数开始随着插入非 control point 而递增的, 所以它是不会重复也不会跳过的
         * 对于情况 2, 避免了插入重复的点, 使用了相同的 vertex 的 index
         * @param singleVertex           需要插入的 vertex
         * @return                  如果是新的 vertex, 返回 true, 否则返回 false
         */
        bool insert(SingleVertexPtr &singleVertex, QSharedPointer<VertexContext> &vertexContext);
    };

private:

    QFileInfo m_modelFile;

    QDir m_modelDir;

    FbxManager *m_fbxManager;

    FbxScene *m_fbxScene;

    QSharedPointer<Scene> m_scene;

    bool importModel();

    bool initializeFbxSdkObjects();

    bool destroyFbxSdkObjects();

    static bool isMeshNode(FbxNode *fbxNode);

    void processTextureFiles();

    void collectMaterialData(FbxSurfaceMaterial *fbxMaterial, QSharedPointer<Material> &material);

    void
    collectTextureData(FbxTexture *fbxTexture, TextureContextPtr textureContext, QSharedPointer<Material> &material);

    void collectTextureData(FbxSurfaceMaterial *fbxMaterial, QSharedPointer<Material> &material);

    void processMaterialsForNode(FbxNode *fbxNode);

    void processTRSforNode(FbxNode *fbxNode, QSharedPointer<Node> &node);

    void prepareFbxMesh(FbxMesh *&fbxMesh);

    void collectVertexPosition(FbxMesh *fbxMesh, SingleVertexPtr &singleVertex,
                               QSharedPointer<VertexContext> &vertexContext);

    void
    collectVertexNormal(FbxMesh *fbxMesh, SingleVertexPtr &singleVertex, QSharedPointer<VertexContext> &vertexContext);

    void collectVertexTexCoords(FbxMesh *fbxMesh, SingleVertexPtr &singleVertex,
                                QSharedPointer<VertexContext> &vertexContext);

    void
    collectVertexColor(FbxMesh *fbxMesh, SingleVertexPtr &singleVertex, QSharedPointer<VertexContext> &vertexContext);

    void collectVertexData(FbxMesh *fbxMesh, VertexTable &vertexTable, QSharedPointer<VertexContext> &vertexContext);

    void collectMeshData(FbxMesh *fbxMesh, QSharedPointer<VertexContext> &vertexContext, QSharedPointer<Node> &node);

    void processMeshesForNode(FbxNode *fbxNode, QSharedPointer<Node> &node);

    void processAllData(FbxNode *fbxNode, QSharedPointer<Node> &node);

    void processNode(FbxNode *fbxNode, QSharedPointer<Node> &node);

    void processNodes();
};

#endif //QSPACE_MODELLOADER_H
