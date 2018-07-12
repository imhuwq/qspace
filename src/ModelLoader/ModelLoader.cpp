#include "ModelLoader/ModelLoader.h"

#define ADAPT_FBXVECTOR4_TO_QVECTOR_DOUBLE(val) (val)[0], (val)[1], (val)[2]
#define ADAPT_FBXVECTOR4_TO_QVECTOR_FLOAT(val) (float)(val)[0], (float)(val)[1], (float)(val)[2]

bool ModelLoader::isMeshNode(FbxNode *fbxNode) {
    FbxNodeAttribute *fbxNodeAttribute = fbxNode->GetNodeAttribute();
    if (!fbxNodeAttribute) return false;
    return fbxNodeAttribute
           and fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh
           and fbxNode->Visibility.Get();
}

inline bool operator==(const ModelLoader::SingleVertex &lhs, const ModelLoader::SingleVertex &rhs) {
    return lhs.m_positions == rhs.m_positions and
           lhs.m_normals == rhs.m_normals and
           lhs.m_uv0s == rhs.m_uv0s and
           lhs.m_uv1s == rhs.m_uv1s and
           lhs.m_colors == rhs.m_colors;
}

inline bool operator!=(const ModelLoader::SingleVertex &lhs, const ModelLoader::SingleVertex &rhs) {
    return !(lhs == rhs);
}

bool ModelLoader::VertexTable::insert(SingleVertexPtr &singleVertex, QSharedPointer<VertexContext> &vertexContext) {
    bool insertSuccess = false;
    bool foundEqual = false;

    // 先根据 gControlPointIndexInMesh 找到当前 control point 对应的 vertex vector
    vector<SingleVertexPtr> &vertices = mTable[vertexContext->m_controlPointIndex2Mesh];

    // 如果 vertex vector 是空的, 说明这个 vertex 肯定是第一次插入, 它就是 control point, 它的 index 就是 control point 的 index
    if (vertices.empty()) {
        vertexContext->m_vertexIndex2Mesh = vertexContext->m_controlPointIndex2Mesh;
        singleVertex->m_index = vertexContext->m_vertexIndex2Mesh;
        vertices.push_back(singleVertex);
        insertSuccess = true;
    } else {
        // 如果 vertex vector 不是空的, 把 vertex 和 vertex vector 里面的每一个 vertex 逐个比较, 看看是否有相等的.
        // 如果有相等的, 说明这个 vertex 已经不是第一次插入了, 那它的 index 就是之前的 index
        for (SingleVertexPtr &vertex: vertices) {
            if (*vertex == *singleVertex) {
                vertexContext->m_vertexIndex2Mesh = vertex->m_index;
                foundEqual = true;
                break;
            }
        }

        // 如果 vertex vector 不是空的, 并且里面没有相等的, 说明这个 vertex 是第一次插入
        // 它的 index 是 mSize, 也就是除了 control point 以外新插入的 vertex 的个数
        if (!foundEqual) {
            vertexContext->m_vertexIndex2Mesh = m_size++;
            singleVertex->m_index = vertexContext->m_vertexIndex2Mesh;
            vertices.push_back(singleVertex);
            insertSuccess = true;
        }
    }

    if (insertSuccess) {
        vertexContext->m_vertexBuffer->extendPositions(singleVertex->m_positions);
        vertexContext->m_vertexBuffer->extendNormals(singleVertex->m_normals);
        vertexContext->m_vertexBuffer->extendUV0s(singleVertex->m_uv0s);
        vertexContext->m_vertexBuffer->extendUV1s(singleVertex->m_uv1s);
        vertexContext->m_vertexBuffer->extendColors(singleVertex->m_colors);
    }

    return insertSuccess;
}

bool ModelLoader::checkFileFormatIsSupported(const QString &filePath) {
    QFileInfo fileInfo(filePath);
    if (fileInfo.exists() && !fileInfo.isHidden()) {
        QString format = fileInfo.suffix().toLower();
        return fileFormat().contains(format);
    }
    return false;
}

bool ModelLoader::load(const QString &pathToFile, QSharedPointer<Scene> &scene) {
    if (!initializeFbxSdkObjects()) return false;

    if (!checkFileFormatIsSupported(pathToFile)) return false;
    m_modelFile = QFileInfo(pathToFile);
    m_modelFile.makeAbsolute();
    m_modelFile.refresh();
    m_modelDir = QDir(m_modelFile.absoluteDir());
    m_scene = scene;

    if (!importModel()) return false;

    processTextureFiles();

    processNodes();

    destroyFbxSdkObjects();
    return true;
}

bool ModelLoader::importModel() {
    FbxImporter *fbxImporter = FbxImporter::Create(m_fbxManager, "");
    bool importStatus = fbxImporter->Initialize(m_modelFile.filePath().toStdString().c_str(), -1,
                                                m_fbxManager->GetIOSettings());
    fbxImporter->Import(m_fbxScene);
    fbxImporter->Destroy();
    return importStatus;
}

bool ModelLoader::initializeFbxSdkObjects() {
    m_fbxManager = FbxManager::Create();
    FbxIOSettings *ios = FbxIOSettings::Create(m_fbxManager, IOSROOT);
    m_fbxManager->SetIOSettings(ios);

    m_fbxScene = FbxScene::Create(m_fbxManager, "Scene");

    return true;
}

bool ModelLoader::destroyFbxSdkObjects() {
    m_fbxScene->Destroy();
    m_fbxManager->Destroy();
}

void ModelLoader::processTextureFiles() {
    QDir fbmDir = m_modelDir.filePath(m_modelFile.baseName() + QString(".fbm"));
    if (fbmDir.exists()) {
        for (const QFileInfo &textureFileInfo:fbmDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
            QString texturePath = fbmDir.filePath(textureFileInfo.fileName());
            QSharedPointer<TextureFile> texture_file(new TextureFile(texturePath));
            m_scene->addTextureFile(texture_file);
        }
    }
}

void ModelLoader::collectMaterialData(FbxSurfaceMaterial *fbxMaterial, QSharedPointer<Material> &material) {
    FbxDouble3 ambient = ((FbxSurfacePhong *) fbxMaterial)->Ambient.Get();
    material->setAmbient({ADAPT_FBXVECTOR4_TO_QVECTOR_DOUBLE(ambient)});

    FbxDouble3 diffuse = ((FbxSurfacePhong *) fbxMaterial)->Diffuse.Get();
    material->setDiffuse({ADAPT_FBXVECTOR4_TO_QVECTOR_DOUBLE(diffuse)});

    FbxDouble3 emissive = ((FbxSurfacePhong *) fbxMaterial)->Emissive.Get();
    material->setEmissive({ADAPT_FBXVECTOR4_TO_QVECTOR_DOUBLE(emissive)});

    FbxDouble opacity = ((FbxSurfacePhong *) fbxMaterial)->TransparencyFactor.Get();

    if (fbxMaterial->GetClassId().Is(FbxSurfacePhong::ClassId)) {
        material->setShading(Material::Shading::Phone);
        material->setOpacity((float) opacity);

        FbxDouble3 specular = ((FbxSurfacePhong *) fbxMaterial)->Specular.Get();
        material->setSpecular({ADAPT_FBXVECTOR4_TO_QVECTOR_DOUBLE(specular)});

        FbxDouble shininess = ((FbxSurfacePhong *) fbxMaterial)->Shininess.Get();
        material->setShininess((float) shininess);

        FbxDouble reflectivity = ((FbxSurfacePhong *) fbxMaterial)->ReflectionFactor.Get();
        material->setReflectivity(1.0f - (float) reflectivity);

    } else if (fbxMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
        material->setShading(Material::Shading::Lambert);
        material->setOpacity(1.0f - (float) opacity);
    }
}

void ModelLoader::collectTextureData(FbxTexture *fbxTexture, TextureContextPtr textureContext,
                                     QSharedPointer<Material> &material) {
    QString textureChannelName = QString(textureContext->m_textureChannel);
    textureChannelName = textureChannelName + "_" + QString::number(textureContext->m_layerIndex);
    QSharedPointer<TextureData> textureData(new TextureData(textureChannelName));

    QString textureName = fbxTexture->GetName();
    textureData->setName(textureName);

    auto fbxFileTexture = FbxCast<FbxFileTexture>(fbxTexture);
    QString textureFilePath = fbxFileTexture->GetFileName();
    QSharedPointer<TextureFile> texture = m_scene->getTextureFile(textureFilePath);
    textureData->setTextureFile(texture);

    textureData->setScaleU((float) fbxTexture->GetScaleU());
    textureData->setScaleV((float) fbxTexture->GetScaleV());
    textureData->setTranslationU((float) fbxTexture->GetTranslationU());
    textureData->setTranslationV((float) fbxTexture->GetTranslationV());
    textureData->setSwapUV(fbxTexture->GetSwapUV());
    textureData->setRotationU((float) fbxTexture->GetRotationU());
    textureData->setRotationV((float) fbxTexture->GetRotationV());
    textureData->setRotationW((float) fbxTexture->GetRotationW());
    textureData->setCroppingLeft(fbxTexture->GetCroppingLeft());
    textureData->setCroppingRight(fbxTexture->GetCroppingRight());
    textureData->setCroppingTop(fbxTexture->GetCroppingTop());
    textureData->setCroppingBottom(fbxTexture->GetCroppingBottom());

    if (textureContext->m_blendMode >= 0) {
        textureData->setBlendMode(TextureData::BlendMode(textureContext->m_blendMode));
    } else textureData->setBlendMode(TextureData::BlendMode::None);

    material->addTexture(textureData);
}

void ModelLoader::collectTextureData(FbxSurfaceMaterial *fbxMaterial, QSharedPointer<Material> &material) {
    for (int textureChannelIndex = 0;
         textureChannelIndex < FbxLayerElement::sTypeNonTextureCount; textureChannelIndex++) {
        const char *textureChannelName = FbxLayerElement::sTextureChannelNames[textureChannelIndex];
        FbxProperty fbxProperty = fbxMaterial->FindProperty(textureChannelName);
        if (!fbxProperty.IsValid()) continue;

        int textureCount = fbxProperty.GetSrcObjectCount<FbxTexture>();
        for (int textureIndex = 0; textureIndex < textureCount; textureIndex++) {
            auto fbxLayeredTexture = fbxProperty.GetSrcObject<FbxLayeredTexture>(textureIndex);
            // 当前通道有多张贴图
            if (fbxLayeredTexture) {
                int layeredTextureCount = fbxLayeredTexture->GetSrcObjectCount<FbxTexture>();
                for (int layerIndex = 0; layerIndex < layeredTextureCount; layerIndex++) {
                    auto fbxTexture = fbxLayeredTexture->GetSrcObject<FbxTexture>(layerIndex);
                    FbxLayeredTexture::EBlendMode blendMode;
                    fbxLayeredTexture->GetTextureBlendMode(layerIndex, blendMode);
                    TextureContextPtr textureContext(new TextureContext(textureChannelName, layerIndex, blendMode));
                    collectTextureData(fbxTexture, textureContext, material);
                }
            } else {
                auto fbxTexture = fbxProperty.GetSrcObject<FbxTexture>(textureIndex);
                TextureContextPtr textureContext(new TextureContext(textureChannelName, 0, -1));
                collectTextureData(fbxTexture, textureContext, material);
            }
        }
    }
}

void ModelLoader::processMaterialsForNode(FbxNode *fbxNode) {
    if (isMeshNode(fbxNode)) {
        auto fbxMesh = (FbxMesh *) fbxNode->GetNodeAttribute();
        for (int layerIndex = 0; layerIndex < fbxMesh->GetLayerCount(); layerIndex++) {
            FbxGeometryElementMaterial *layerMaterials = fbxMesh->GetLayer(layerIndex)->GetMaterials();

            if (!layerMaterials) continue;
            if (layerMaterials->GetReferenceMode() == FbxLayerElement::eIndex) continue;

            for (int materialIndex = 0; materialIndex < fbxNode->GetMaterialCount(); materialIndex++) {
                FbxSurfaceMaterial *fbxMaterial = fbxNode->GetMaterial(materialIndex);
                QSharedPointer<Material> material(new Material(fbxMaterial->GetName()));
                collectMaterialData(fbxMaterial, material);
                collectTextureData(fbxMaterial, material);
                m_scene->addMaterial(material);
            }
        }
    }
}

void ModelLoader::processTRSforNode(FbxNode *fbxNode, QSharedPointer<Node> &node) {
    FbxAMatrix fbxNodeLocalTransform = fbxNode->EvaluateLocalTransform();

    FbxVector4 translation = fbxNodeLocalTransform.GetT();
    FbxVector4 rotation = fbxNodeLocalTransform.GetR();
    FbxVector4 scale = fbxNodeLocalTransform.GetS();

    QMatrix4x4 nodeTransformation;
    nodeTransformation.setToIdentity();
    nodeTransformation.translate(ADAPT_FBXVECTOR4_TO_QVECTOR_FLOAT(translation));
    nodeTransformation.rotate(ADAPT_FBXVECTOR4_TO_QVECTOR_FLOAT(rotation));
    nodeTransformation.scale(ADAPT_FBXVECTOR4_TO_QVECTOR_FLOAT(scale));
    node->setTransformation(nodeTransformation);
}

void ModelLoader::prepareFbxMesh(FbxMesh *&fbxMesh) {
    fbxMesh->RemoveBadPolygons();
    fbxMesh = (FbxMesh *) FbxGeometryConverter(m_fbxManager).Triangulate(fbxMesh, true);
    fbxMesh->GenerateNormals(false, true, false);
}

void ModelLoader::collectVertexPosition(FbxMesh *fbxMesh, SingleVertexPtr &singleVertex,
                                        QSharedPointer<VertexContext> &vertexContext) {
    FbxVector4 *controlPoints = fbxMesh->GetControlPoints();
    FbxVector4 controlPoint = controlPoints[vertexContext->m_controlPointIndex2Mesh];
    singleVertex->m_positions = {ADAPT_FBXVECTOR4_TO_QVECTOR_DOUBLE(controlPoint)};
}

void ModelLoader::collectVertexNormal(FbxMesh *fbxMesh, SingleVertexPtr &singleVertex,
                                      QSharedPointer<VertexContext> &vertexContext) {
    for (int normalIndex = 0; normalIndex < 1; normalIndex++) {
        FbxVector4 normal;
        bool normalFound = false;

        FbxGeometryElementNormal *fbxNormal = fbxMesh->GetElementNormal(normalIndex);
        if (!fbxNormal) continue;

        FbxGeometryElement::EMappingMode mapMode = fbxNormal->GetMappingMode();
        int identifier;
        if (mapMode == FbxGeometryElement::eByPolygonVertex) identifier = vertexContext->m_vertexId2Mesh;
        else if (mapMode == FbxGeometryElement::eByControlPoint) identifier = vertexContext->m_controlPointIndex2Mesh;
        else continue;

        FbxLayerElement::EReferenceMode refMode = fbxNormal->GetReferenceMode();
        if (refMode == FbxGeometryElement::eDirect) {
            normal = fbxNormal->GetDirectArray().GetAt(identifier);
            normalFound = true;
        } else if (refMode == FbxGeometryElement::eIndexToDirect) {
            int id = fbxNormal->GetIndexArray().GetAt(identifier);
            normal = fbxNormal->GetDirectArray().GetAt(id);
            normalFound = true;
        }

        if (normalFound) {
            singleVertex->m_normals = {ADAPT_FBXVECTOR4_TO_QVECTOR_DOUBLE(normal)};
        }
    }
}

void ModelLoader::collectVertexTexCoords(FbxMesh *fbxMesh, SingleVertexPtr &singleVertex,
                                         QSharedPointer<VertexContext> &vertexContext) {
    for (int texCoordIndex = 0; texCoordIndex < 2; texCoordIndex++) {
        FbxVector2 uv;
        bool uvFound = false;

        FbxGeometryElementUV *fbxUV = fbxMesh->GetElementUV(texCoordIndex);
        if (!fbxUV) continue;

        FbxGeometryElement::EMappingMode fMapMode = fbxUV->GetMappingMode();
        int identifier;
        if (fMapMode == FbxGeometryElement::eByPolygonVertex)
            identifier = fbxMesh->GetTextureUVIndex(vertexContext->m_polygonIndex2Mesh,
                                                    vertexContext->m_controlPointIndex2Polygon);
        else if (fMapMode == FbxGeometryElement::eByControlPoint) identifier = vertexContext->m_controlPointIndex2Mesh;
        else continue;

        FbxGeometryElement::EReferenceMode refMode = fbxUV->GetReferenceMode();
        // NOTICE map mode of eByPolygonVertex
        if (refMode == FbxGeometryElement::eDirect or fMapMode == FbxGeometryElement::eByPolygonVertex) {
            uv = fbxUV->GetDirectArray().GetAt(identifier);
            uvFound = true;
        } else if (refMode == FbxGeometryElement::eIndexToDirect) {
            int id = fbxUV->GetIndexArray().GetAt(identifier);
            uv = fbxUV->GetDirectArray().GetAt(id);
            uvFound = true;
        }

        if (uvFound) {
            if (texCoordIndex == 0) singleVertex->m_uv0s = {ADAPT_FBXVECTOR4_TO_QVECTOR_DOUBLE(uv)};
            else if (texCoordIndex == 1) singleVertex->m_uv1s = {ADAPT_FBXVECTOR4_TO_QVECTOR_DOUBLE(uv)};
        }
    }
}

void ModelLoader::collectVertexColor(FbxMesh *fbxMesh, SingleVertexPtr &singleVertex,
                                     QSharedPointer<VertexContext> &vertexContext) {
    for (int colorIndex = 0; colorIndex < 1; colorIndex++) {
        FbxColor color;
        bool colorFound = false;

        FbxGeometryElementVertexColor *fbxColor = fbxMesh->GetElementVertexColor(colorIndex);
        if (!fbxColor) continue;

        FbxGeometryElement::EMappingMode mapMode = fbxColor->GetMappingMode();
        int identifier;
        if (mapMode == FbxGeometryElement::eByPolygonVertex) identifier = vertexContext->m_vertexId2Mesh;
        else if (mapMode == FbxGeometryElement::eByControlPoint) identifier = vertexContext->m_controlPointIndex2Mesh;
        else continue;

        FbxGeometryElement::EReferenceMode refMod = fbxColor->GetReferenceMode();
        if (refMod == FbxGeometryElement::eDirect) {
            color = fbxColor->GetDirectArray().GetAt(identifier);
            colorFound = true;
        } else if (refMod == FbxGeometryElement::eIndexToDirect) {
            int id = fbxColor->GetIndexArray().GetAt(identifier);
            color = fbxColor->GetDirectArray().GetAt(id);
            colorFound = true;
        }

        if (colorFound) {
            singleVertex->m_colors = {(int) color.mRed * 255, (int) color.mGreen * 255, (int) color.mBlue * 255,
                                      (int) color.mAlpha * 255};
        }
    }
}

void ModelLoader::collectVertexData(FbxMesh *fbxMesh, VertexTable &vertexTable,
                                    QSharedPointer<VertexContext> &vertexContext) {
    SingleVertexPtr singleVertex(new SingleVertex());
    collectVertexPosition(fbxMesh, singleVertex, vertexContext);
    collectVertexNormal(fbxMesh, singleVertex, vertexContext);
    collectVertexTexCoords(fbxMesh, singleVertex, vertexContext);
    collectVertexColor(fbxMesh, singleVertex, vertexContext);

    vertexTable.insert(singleVertex, vertexContext);
}

void ModelLoader::collectMeshData(FbxMesh *fbxMesh, QSharedPointer<VertexContext> &vertexContext,
                                  QSharedPointer<Node> &node) {
    int materialCount = fbxMesh->GetElementMaterialCount();

    QString materialName = "default";
    if (materialCount > 0) {
        for (int materialIndex = 0; materialIndex < materialCount; materialIndex++) {
            FbxGeometryElementMaterial *fbxMaterialElement = fbxMesh->GetElementMaterial(materialIndex);
            // 根据当前 polygon(面) 的 index 找到其所属的 material, 以获取其名字
            int materialID = fbxMaterialElement->GetIndexArray().GetAt(vertexContext->m_polygonIndex2Mesh);
            if (materialID >= 0) {
                FbxSurfaceMaterial *fbxMaterial = fbxMesh->GetNode()->GetMaterial(materialID);
                materialName = fbxMaterial->GetName();
            }

            // 根据找到的 material 的名字找到(创建) 对应的 mesh instance
            QSharedPointer<Mesh> mesh(new Mesh());
            if (node->hasMesh(materialName)) {
                mesh = node->getMesh(materialName);
            } else {
                node->setMesh(mesh, materialName);
            }

            // 把 vertex index 加入到 mesh model 里面去
            mesh->extendTmpIndices(vertexContext->m_vertexIndicesOfPolygon);
        }
    }
    else {
        // 根据找到的 material 的名字找到(创建) 对应的 mesh instance
        QSharedPointer<Mesh> mesh(new Mesh());
        if (node->hasMesh(materialName)) {
            mesh = node->getMesh(materialName);
        } else {
            node->setMesh(mesh, materialName);
        }

        // 把 vertex index 加入到 mesh model 里面去
        mesh->extendTmpIndices(vertexContext->m_vertexIndicesOfPolygon);
    }
}

void ModelLoader::processMeshesForNode(FbxNode *fbxNode, QSharedPointer<Node> &node) {
    if (!isMeshNode(fbxNode)) return;
    auto *fbxMesh = (FbxMesh *) fbxNode->GetNodeAttribute();
    prepareFbxMesh(fbxMesh);

    int controlPointsCount = fbxMesh->GetControlPointsCount();
    VertexTable vertexTable(controlPointsCount);
    QSharedPointer<VertexContext> vertexContext(new VertexContext());
    vertexContext->m_vertexBuffer = m_scene->getVertexBuffer();

    for (int polygonIndex = 0; polygonIndex < fbxMesh->GetPolygonCount(); polygonIndex++) {
        vertexContext->m_polygonIndex2Mesh = polygonIndex;
        vertexContext->m_vertexIndicesOfPolygon = {};
        for (int controlPointIndex = 0;
             controlPointIndex < fbxMesh->GetPolygonSize(polygonIndex); controlPointIndex++) {
            vertexContext->m_controlPointIndex2Polygon = controlPointIndex;
            vertexContext->m_controlPointIndex2Mesh = fbxMesh->GetPolygonVertex(polygonIndex, controlPointIndex);
            collectVertexData(fbxMesh, vertexTable, vertexContext);
            vertexContext->m_vertexIndicesOfPolygon.append(vertexContext->m_vertexIndex2Mesh);
            vertexContext->m_vertexId2Mesh++;
        }
        collectMeshData(fbxMesh, vertexContext, node);
        vertexContext->m_vertexIndicesOfPolygon.clear();
    }

    QVectorIterator<QSharedPointer<Mesh>> meshes(node->meshes());
    while (meshes.hasNext()) {
        auto mesh = meshes.next();
        mesh->setIndexOffset(vertexContext->m_vertexBuffer->getIndicesSize());
        vertexContext->m_vertexBuffer->extendIndices(mesh->indices());
        mesh->setIndexCount(mesh->indices().size());
        mesh->clearIndices();
    }
}

void ModelLoader::processAllData(FbxNode *fbxNode, QSharedPointer<Node> &node) {
    processMaterialsForNode(fbxNode);
    processTRSforNode(fbxNode, node);
    processMeshesForNode(fbxNode, node);
}

void ModelLoader::processNode(FbxNode *fbxNode, QSharedPointer<Node> &node) {
    processAllData(fbxNode, node);
    for (int index = 0; index < fbxNode->GetChildCount(); index++) {
        FbxNode *childFbxNode = fbxNode->GetChild(index);
        QSharedPointer<Node> childNode(new Node(childFbxNode->GetName()));
        processNode(childFbxNode, childNode);
        node->addNode(childNode);
    }
}

void ModelLoader::processNodes() {
    FbxNode *rootFbxNode = m_fbxScene->GetRootNode();
    QSharedPointer<Node> rootNode(new Node(rootFbxNode->GetName()));
    processNode(rootFbxNode, rootNode);
    m_scene->setRootNode(rootNode);
}