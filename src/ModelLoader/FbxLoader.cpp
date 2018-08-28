#include "ModelLoader/FbxLoader.h"

#define ADAPT_FBXVECTOR4_TO_QVECTOR_DOUBLE(val) (val)[0], (val)[1], (val)[2]
#define ADAPT_FBXVECTOR4_TO_QVECTOR_FLOAT(val) (float)(val)[0], (float)(val)[1], (float)(val)[2]

bool FbxLoader::IsMeshNode(FbxNode *fbx_node) {
    FbxNodeAttribute *fbx_node_attribute = fbx_node->GetNodeAttribute();
    if (!fbx_node_attribute) return false;
    return fbx_node_attribute
           and fbx_node_attribute->GetAttributeType() == FbxNodeAttribute::eMesh
           and fbx_node->Visibility.Get();
}

bool FbxLoader::CheckFormatIsSupported(const QString &file_path) {
    QFileInfo file_info(file_path);
    if (file_info.exists() && !file_info.isHidden()) {
        QString format = file_info.suffix().toLower();
        return GetFileFormat().contains(format);
    }
    return false;
}

bool FbxLoader::Load(const QString &model_file, ScenePtr scene) {
    if (!InitializeFbxSdkObjects()) return false;

    if (!CheckFormatIsSupported(model_file)) return false;
    model_file_ = QFileInfo(model_file);
    model_file_.makeAbsolute();
    model_file_.refresh();
    model_dir_ = QDir(model_file_.absoluteDir());
    scene_ = scene;

    if (!ImportModel()) return false;

    ProcessTextureFiles();

    ProcessNodes();

    DestroyFbxSdkObjects();
    return true;
}

NodePtr FbxLoader::CreateNode(FbxNode *const &fbx_node) {
    if (IsMeshNode(fbx_node)) return MeshPtr(new Mesh(fbx_node->GetName()));
    else return NodePtr(new Node(fbx_node->GetName()));
}

bool FbxLoader::ImportModel() {
    FbxImporter *fbx_importer = FbxImporter::Create(fbx_manager_, "");
    bool import_status = fbx_importer->Initialize(model_file_.filePath().toStdString().c_str(), -1, fbx_manager_->GetIOSettings());
    fbx_importer->Import(fbx_scene_);
    fbx_importer->Destroy();
    return import_status;
}

bool FbxLoader::InitializeFbxSdkObjects() {
    fbx_manager_ = FbxManager::Create();
    FbxIOSettings *ios = FbxIOSettings::Create(fbx_manager_, IOSROOT);
    fbx_manager_->SetIOSettings(ios);
    fbx_scene_ = FbxScene::Create(fbx_manager_, "3dObject");
    return true;
}

bool FbxLoader::DestroyFbxSdkObjects() {
    fbx_scene_->Destroy();
    fbx_manager_->Destroy();
}

void FbxLoader::ProcessTextureFiles() {
    QDir fbm_dir = model_dir_.filePath(model_file_.baseName() + QString(".fbm"));
    if (fbm_dir.exists()) {
        for (const QFileInfo &texture_file_info:fbm_dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
            QString texture_path = fbm_dir.filePath(texture_file_info.fileName());
            scene_->AddTextureFile(texture_path);
        }
    }
}

void FbxLoader::CollectMaterialData(FbxSurfaceMaterial *fbx_material, MaterialPtr material) {
    FbxDouble3 ambient = ((FbxSurfacePhong *) fbx_material)->Ambient.Get();
    material->SetAmbient({ADAPT_FBXVECTOR4_TO_QVECTOR_DOUBLE(ambient)});

    FbxDouble3 diffuse = ((FbxSurfacePhong *) fbx_material)->Diffuse.Get();
    material->SetDiffuse({ADAPT_FBXVECTOR4_TO_QVECTOR_DOUBLE(diffuse)});

    FbxDouble3 emissive = ((FbxSurfacePhong *) fbx_material)->Emissive.Get();
    material->SetEmissive({ADAPT_FBXVECTOR4_TO_QVECTOR_DOUBLE(emissive)});

    FbxDouble opacity = ((FbxSurfacePhong *) fbx_material)->TransparencyFactor.Get();

    if (fbx_material->GetClassId().Is(FbxSurfacePhong::ClassId)) {
        material->SetShading(Material::Shading::kPhone);
        material->SetOpacity((float) opacity);

        FbxDouble3 specular = ((FbxSurfacePhong *) fbx_material)->Specular.Get();
        material->SetSpecular({ADAPT_FBXVECTOR4_TO_QVECTOR_DOUBLE(specular)});

        FbxDouble shininess = ((FbxSurfacePhong *) fbx_material)->Shininess.Get();
        material->SetShininess((float) shininess);

        FbxDouble reflectivity = ((FbxSurfacePhong *) fbx_material)->ReflectionFactor.Get();
        material->SetReflectivity(1.0f - (float) reflectivity);

    } else if (fbx_material->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
        material->SetShading(Material::Shading::kLambert);
        material->SetOpacity(1.0f - (float) opacity);
    }
}

void FbxLoader::CollectTextureData(FbxTexture *fbx_texture, TextureContextPtr texture_context, MaterialPtr material) {
    QString texture_channel_name = QString(texture_context->texture_channel_);
    texture_channel_name = texture_channel_name + "_" + QString::number(texture_context->layer_index_);
    TexturePtr texture(new Texture(texture_channel_name));

    QString textureName = fbx_texture->GetName();
    texture->SetName(textureName);

    auto fbx_file_texture = FbxCast<FbxFileTexture>(fbx_texture);
    QString fbx_texture_path = fbx_file_texture->GetFileName();
    QString texture_path = scene_->GetTextureFile(fbx_texture_path);
    texture->SetTextureFile(texture_path);

    texture->SetScaleU((float) fbx_texture->GetScaleU());
    texture->SetScaleV((float) fbx_texture->GetScaleV());
    texture->SetTranslationU((float) fbx_texture->GetTranslationU());
    texture->SetTranslationV((float) fbx_texture->GetTranslationV());
    texture->SetSwapUV(fbx_texture->GetSwapUV());
    texture->SetRotationU((float) fbx_texture->GetRotationU());
    texture->SetRotationV((float) fbx_texture->GetRotationV());
    texture->SetRotationW((float) fbx_texture->GetRotationW());
    texture->SetCroppingLeft(fbx_texture->GetCroppingLeft());
    texture->SetCroppingRight(fbx_texture->GetCroppingRight());
    texture->SetCroppingTop(fbx_texture->GetCroppingTop());
    texture->SetCroppingBottom(fbx_texture->GetCroppingBottom());

    if (texture_context->blend_mode_ >= 0) texture->SetBlendMode(TextureBlendMode(texture_context->blend_mode_));
    else texture->SetBlendMode(TextureBlendMode::kNone);

    material->AddTexture(texture);
}

void FbxLoader::CollectTextureData(FbxSurfaceMaterial *fbx_material, MaterialPtr material) {
    for (int texture_channel_index = 0; texture_channel_index < FbxLayerElement::sTypeNonTextureCount; texture_channel_index++) {
        const char *texture_channel_name = FbxLayerElement::sTextureChannelNames[texture_channel_index];
        FbxProperty fbx_property = fbx_material->FindProperty(texture_channel_name);
        if (!fbx_property.IsValid()) continue;

        int texture_count = fbx_property.GetSrcObjectCount<FbxTexture>();
        for (int texture_index = 0; texture_index < texture_count; texture_index++) {
            auto fbx_layered_texture = fbx_property.GetSrcObject<FbxLayeredTexture>(texture_index);
            // 当前通道有多张贴图
            if (fbx_layered_texture) {
                int layered_texture_count = fbx_layered_texture->GetSrcObjectCount<FbxTexture>();
                for (int layer_index = 0; layer_index < layered_texture_count; layer_index++) {
                    auto fbx_texture = fbx_layered_texture->GetSrcObject<FbxTexture>(layer_index);
                    FbxLayeredTexture::EBlendMode blend_mode;
                    fbx_layered_texture->GetTextureBlendMode(layer_index, blend_mode);
                    TextureContextPtr texture_context(new TextureContext(texture_channel_name, layer_index, TextureBlendMode(blend_mode)));
                    CollectTextureData(fbx_texture, texture_context, material);
                }
            } else {
                auto fbxTexture = fbx_property.GetSrcObject<FbxTexture>(texture_index);
                TextureContextPtr texture_context(new TextureContext(texture_channel_name, 0, TextureBlendMode::kNone));
                CollectTextureData(fbxTexture, texture_context, material);
            }
        }
    }
}

void FbxLoader::ProcessNodeMaterials(FbxNode *fbx_node, MeshPtr node) {
    if (IsMeshNode(fbx_node)) {
        auto fbx_mesh = (FbxMesh *) fbx_node->GetNodeAttribute();
        for (int layer_index = 0; layer_index < fbx_mesh->GetLayerCount(); layer_index++) {
            FbxGeometryElementMaterial *layer_material = fbx_mesh->GetLayer(layer_index)->GetMaterials();

            if (!layer_material) continue;
            if (layer_material->GetReferenceMode() == FbxLayerElement::eIndex) continue;

            for (int material_index = 0; material_index < fbx_node->GetMaterialCount(); material_index++) {
                FbxSurfaceMaterial *fbx_material = fbx_node->GetMaterial(material_index);
                MaterialPtr material(new Material(fbx_material->GetName()));
                CollectMaterialData(fbx_material, material);
                CollectTextureData(fbx_material, material);
                scene_->AddMaterial(material);
            }
        }
    }
}

void FbxLoader::ProcessNodeTRS(FbxNode *fbx_node, NodePtr node) {
    FbxAMatrix fbx_node_local_transform = fbx_node->EvaluateLocalTransform();

    FbxVector4 translation = fbx_node_local_transform.GetT();
    FbxVector4 rotation = fbx_node_local_transform.GetR();
    FbxVector4 scale = fbx_node_local_transform.GetS();

    node->Translate({ADAPT_FBXVECTOR4_TO_QVECTOR_FLOAT(translation)});
    node->Rotate({ADAPT_FBXVECTOR4_TO_QVECTOR_FLOAT(rotation)});
    node->SetScale({ADAPT_FBXVECTOR4_TO_QVECTOR_FLOAT(scale)});
}

void FbxLoader::PrepareFbxMesh(FbxMesh *&fbx_mesh) {
    fbx_mesh->RemoveBadPolygons();
    fbx_mesh = (FbxMesh *) FbxGeometryConverter(fbx_manager_).Triangulate(fbx_mesh, true);
    fbx_mesh->GenerateNormals(false, true, false);
}

void FbxLoader::CollectVertexPosition(FbxMesh *fbx_mesh, VertexPtr vertex, VertexContextPtr vertex_context) {
    FbxVector4 *controlPoints = fbx_mesh->GetControlPoints();
    FbxVector4 controlPoint = controlPoints[vertex_context->control_point_index_in_mesh];
    vertex->positions = {ADAPT_FBXVECTOR4_TO_QVECTOR_DOUBLE(controlPoint)};
}

void FbxLoader::CollectVertexNormal(FbxMesh *fbx_mesh, VertexPtr vertex, VertexContextPtr vertex_context) {
    for (int normal_index = 0; normal_index < 1; normal_index++) {
        FbxVector4 normal;
        bool normal_found = false;

        FbxGeometryElementNormal *fbx_normal = fbx_mesh->GetElementNormal(normal_index);
        if (!fbx_normal) continue;

        FbxGeometryElement::EMappingMode map_mode = fbx_normal->GetMappingMode();
        int identifier;
        if (map_mode == FbxGeometryElement::eByPolygonVertex) identifier = vertex_context->vertex_id_in_mesh;
        else if (map_mode == FbxGeometryElement::eByControlPoint) identifier = vertex_context->control_point_index_in_mesh;
        else continue;

        FbxLayerElement::EReferenceMode reference_mode = fbx_normal->GetReferenceMode();
        if (reference_mode == FbxGeometryElement::eDirect) {
            normal = fbx_normal->GetDirectArray().GetAt(identifier);
            normal_found = true;
        } else if (reference_mode == FbxGeometryElement::eIndexToDirect) {
            int id = fbx_normal->GetIndexArray().GetAt(identifier);
            normal = fbx_normal->GetDirectArray().GetAt(id);
            normal_found = true;
        }

        if (normal_found) {
            vertex->normals = {ADAPT_FBXVECTOR4_TO_QVECTOR_DOUBLE(normal)};
        }
    }
}

void FbxLoader::CollectVertexTexCoords(FbxMesh *fbx_mesh, VertexPtr vertex, VertexContextPtr vertex_context) {
    for (int texcoord_index = 0; texcoord_index < 2; texcoord_index++) {
        FbxVector2 uv;
        bool uv_found = false;

        FbxGeometryElementUV *fbx_uv = fbx_mesh->GetElementUV(texcoord_index);
        if (!fbx_uv) continue;

        FbxGeometryElement::EMappingMode map_mode = fbx_uv->GetMappingMode();
        int identifier;
        if (map_mode == FbxGeometryElement::eByPolygonVertex)
            identifier = fbx_mesh->GetTextureUVIndex(vertex_context->polygon_index_in_mesh,
                                                     vertex_context->control_point_index_in_polygon);
        else if (map_mode == FbxGeometryElement::eByControlPoint) identifier = vertex_context->control_point_index_in_mesh;
        else continue;

        FbxGeometryElement::EReferenceMode reference_mode = fbx_uv->GetReferenceMode();
        if (reference_mode == FbxGeometryElement::eDirect or map_mode == FbxGeometryElement::eByPolygonVertex) {
            uv = fbx_uv->GetDirectArray().GetAt(identifier);
            uv_found = true;
        } else if (reference_mode == FbxGeometryElement::eIndexToDirect) {
            int id = fbx_uv->GetIndexArray().GetAt(identifier);
            uv = fbx_uv->GetDirectArray().GetAt(id);
            uv_found = true;
        }

        if (uv_found) {
            if (texcoord_index == 0) vertex->uv0 = {ADAPT_FBXVECTOR4_TO_QVECTOR_DOUBLE(uv)};
            else if (texcoord_index == 1) vertex->uv1 = {ADAPT_FBXVECTOR4_TO_QVECTOR_DOUBLE(uv)};
        }
    }
}

void FbxLoader::CollectVertexColor(FbxMesh *fbx_mesh, VertexPtr vertex, VertexContextPtr vertex_context) {
    for (int color_index = 0; color_index < 1; color_index++) {
        FbxColor color;
        bool color_found = false;

        FbxGeometryElementVertexColor *fbx_color = fbx_mesh->GetElementVertexColor(color_index);
        if (!fbx_color) continue;

        FbxGeometryElement::EMappingMode map_mode = fbx_color->GetMappingMode();
        int identifier;
        if (map_mode == FbxGeometryElement::eByPolygonVertex) identifier = vertex_context->vertex_id_in_mesh;
        else if (map_mode == FbxGeometryElement::eByControlPoint) identifier = vertex_context->control_point_index_in_mesh;
        else continue;

        FbxGeometryElement::EReferenceMode reference_mode = fbx_color->GetReferenceMode();
        if (reference_mode == FbxGeometryElement::eDirect) {
            color = fbx_color->GetDirectArray().GetAt(identifier);
            color_found = true;
        } else if (reference_mode == FbxGeometryElement::eIndexToDirect) {
            int id = fbx_color->GetIndexArray().GetAt(identifier);
            color = fbx_color->GetDirectArray().GetAt(id);
            color_found = true;
        }

        if (color_found) {
            vertex->colors = {(int) color.mRed * 255, (int) color.mGreen * 255, (int) color.mBlue * 255,
                              (int) color.mAlpha * 255};
        }
    }
}

void FbxLoader::CollectVertexBufferData(FbxMesh *fbx_mesh, VertexTable vertex_table, VertexContextPtr vertex_context, VertexBufferPtr vertex_buffer) {
    VertexPtr vertex(new Vertex());
    CollectVertexPosition(fbx_mesh, vertex, vertex_context);
    CollectVertexNormal(fbx_mesh, vertex, vertex_context);
    CollectVertexTexCoords(fbx_mesh, vertex, vertex_context);
    CollectVertexColor(fbx_mesh, vertex, vertex_context);

    if (vertex_table.Insert(vertex, vertex_context)) {
        int insert_index = vertex_context->vertex_index_in_mesh;
        vertex_buffer->InsertPositions(vertex->positions, insert_index * 3, 3);
        vertex_buffer->InsertNormals(vertex->normals, insert_index * 3, 3);
        vertex_buffer->InsertUV0(vertex->uv0, insert_index * 2, 2);
        vertex_buffer->InsertUV1(vertex->uv1, insert_index * 2, 2);
        vertex_buffer->InsertColors(vertex->colors, insert_index * 3, 3);
    }
}

void FbxLoader::CollectGeometryData(FbxMesh *fbx_mesh, VertexContextPtr vertex_context, MeshPtr node) {
    int material_count = fbx_mesh->GetElementMaterialCount();

    QString material_name = "default";
    if (material_count > 0) {
        for (int material_index = 0; material_index < material_count; material_index++) {
            FbxGeometryElementMaterial *fbx_material_element = fbx_mesh->GetElementMaterial(material_index);
            int material_id = fbx_material_element->GetIndexArray().GetAt(vertex_context->polygon_index_in_mesh);
            if (material_id >= 0) material_name = fbx_mesh->GetNode()->GetMaterial(material_id)->GetName();
            GeometryPtr geometry = node->GetOrCreateGeometry(material_name);
            geometry->ExtendIndices(vertex_context->vertex_indices_of_polygon);
        }
    } else {
        GeometryPtr geometry = node->GetOrCreateGeometry(material_name);
        geometry->ExtendIndices(vertex_context->vertex_indices_of_polygon);
    }
}

void FbxLoader::ProcessNodeGeometry(FbxNode *fbx_node, MeshPtr node) {
    if (!IsMeshNode(fbx_node)) return;
    auto *fbx_mesh = (FbxMesh *) fbx_node->GetNodeAttribute();
    PrepareFbxMesh(fbx_mesh);

    int control_point_count = fbx_mesh->GetControlPointsCount();
    VertexTable vertex_table(control_point_count);
    VertexContextPtr vertex_context(new VertexContext());
    VertexBufferPtr vertex_buffer = scene_->GetVertexBuffer();
    int mesh_index_offset = vertex_buffer->GetIndicesSize();

    for (int polygon_index = 0; polygon_index < fbx_mesh->GetPolygonCount(); polygon_index++) {
        vertex_context->polygon_index_in_mesh = polygon_index;
        vertex_context->vertex_indices_of_polygon = {};
        for (int control_point_index = 0; control_point_index < fbx_mesh->GetPolygonSize(polygon_index); control_point_index++) {
            vertex_context->control_point_index_in_polygon = control_point_index;
            vertex_context->control_point_index_in_mesh = fbx_mesh->GetPolygonVertex(polygon_index, control_point_index);
            CollectVertexBufferData(fbx_mesh, vertex_table, vertex_context, vertex_buffer);
            vertex_context->vertex_indices_of_polygon.append((unsigned int) vertex_context->vertex_index_in_mesh);
            vertex_context->vertex_id_in_mesh++;
        }
        CollectGeometryData(fbx_mesh, vertex_context, node);
        vertex_context->vertex_indices_of_polygon.clear();
    }

    for (auto geometry:node->GetGeometries()) {
        geometry->SetIndexOffset(mesh_index_offset);
        vertex_buffer->ExtendIndices(geometry->GetIndices());
    }
}

void FbxLoader::ProcessNodeAllData(FbxNode *fbx_node, NodePtr node) {
    ProcessNodeTRS(fbx_node, node);
    if (node->GetType() == NodeType::kMesh) {
        MeshPtr mesh_node = qSharedPointerDynamicCast<Mesh>(node);
        ProcessNodeMaterials(fbx_node, mesh_node);
        ProcessNodeGeometry(fbx_node, mesh_node);
    }
}

void FbxLoader::ProcessNode(FbxNode *fbx_node, NodePtr node) {
    ProcessNodeAllData(fbx_node, node);
    for (int index = 0; index < fbx_node->GetChildCount(); index++) {
        FbxNode *child_fbx_node = fbx_node->GetChild(index);
        NodePtr child_node = CreateNode(child_fbx_node);
        ProcessNode(child_fbx_node, child_node);
        node->AddNode(child_node);
    }
    node->ComputeTransform();
}

double FbxLoader::GetScaleRatio() {
    FbxSystemUnit lSysUnit = fbx_scene_->GetGlobalSettings().GetSystemUnit();

    if (lSysUnit == FbxSystemUnit::mm) return 0.001;
    if (lSysUnit == FbxSystemUnit::dm) return 0.1;
    if (lSysUnit == FbxSystemUnit::cm) return 0.01;
    if (lSysUnit == FbxSystemUnit::m) return 1.0;
    if (lSysUnit == FbxSystemUnit::km) return 1000.0;
    if (lSysUnit == FbxSystemUnit::Inch) return 0.0254;
    if (lSysUnit == FbxSystemUnit::Foot) return 0.3048;
    if (lSysUnit == FbxSystemUnit::Mile) return 1609.344;
    if (lSysUnit == FbxSystemUnit::Yard) return 0.9144;
    return 1.0;
}

void FbxLoader::ProcessNodes() {
    FbxNode *root_fbx_node = fbx_scene_->GetRootNode();
    NodePtr root_node = CreateNode(root_fbx_node);
    ProcessNode(root_fbx_node, root_node);
    double scale_ratio = GetScaleRatio();
    root_node->Scale(scale_ratio);
    scene_->SetModel(root_node);
}
