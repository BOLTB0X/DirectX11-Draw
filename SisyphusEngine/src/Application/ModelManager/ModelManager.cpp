// Application/ModelManager/ModelManager.cpp
#include "ModelManager.h"
#include "TexturesManager/TexturesManager.h"
// Common
#include "Common/EngineHelper.h"
#include "Common/EngineSettings.h"
// Graphics
#include "Model/MeshModel.h"
#include "Model/TerrainModel.h"
#include "Model/Mesh/Mesh.h"
#include "Model/Mesh/VertexTypes.h"
#include "Model/Texture/Material.h"
#include "Model/Texture/Texture.h"


#include <filesystem>
#include <vector>

/* default */
///////////////////////////////////////////////////////////////////////////

ModelManager::ModelManager() { };

ModelManager::~ModelManager()
{
    Shutdown();
};


bool ModelManager::Init(ID3D11Device* device,
    ID3D11DeviceContext* context,
    TexturesManager* texManager)
{ 

    if (GetMeshModel(device, context, texManager, EngineSettings::STONE_PATH))
        EngineHelper::SuccessCheck(true, "모델 로드 성공");
    else
    {
        EngineHelper::SuccessCheck(false, "Stone 모델 로드 실패: " + EngineSettings::STONE_PATH);
        return false;
    }

    if (GetTerrainModel(device, context, texManager, EngineSettings::TERRAIN_PATH))
        EngineHelper::SuccessCheck(true, "TERRAIN 모델 로드 성공");
    else
    {
        EngineHelper::SuccessCheck(false, "TERRAIN 모델 로드 실패");
        return false;
    }

	return true;
} // Init


void ModelManager::Shutdown()
{
	m_modelLibrary.clear();
    m_terrainLibrary.clear();
} // Shutdown

/////////////////////////////////////////////////////////////////////////

/* public */
//////////////////////////////////////////////////////////////////////////

MeshModel* ModelManager::GetMeshModel(
    ID3D11Device* device,
    ID3D11DeviceContext* context,
    TexturesManager* texManager,
    const std::string& path)
{
    if (m_modelLibrary.find(path) != m_modelLibrary.end())
        return m_modelLibrary[path].get();

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_CalcTangentSpace
    | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality
    | aiProcess_ConvertToLeftHanded);

    if (scene == nullptr)
    {
        EngineHelper::SuccessCheck(false, "Assimp Scene 로드 실패: " + path);
        return nullptr;
    }

    if (scene->mRootNode == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
    {
        EngineHelper::SuccessCheck(false, "Assimp 데이터 불완전: " + path);
        return nullptr;
    }

    auto newModel = std::make_unique<MeshModel>();

    if (EngineHelper::SuccessCheck(newModel->InitConstantBuffer(device), 
        "ModelManager: MeshModel->InitConstantBuffer 실패")
        == false) return nullptr;

    size_t lastSlash = path.find_last_of("\\/");
    std::string directory = (lastSlash == std::string::npos) ? "." : path.substr(0, lastSlash);

    ProcessMaterials(scene, device, context, texManager, directory, newModel.get());
    ProcessNode(scene->mRootNode, scene, device, newModel.get());

    m_modelLibrary[path] = std::move(newModel);
    return m_modelLibrary[path].get();
} // GetMeshModel


TerrainModel* ModelManager::GetTerrainModel(
    ID3D11Device* device,
    ID3D11DeviceContext* context,
    TexturesManager* texManager,
    const std::string& path)
{
    if (m_terrainLibrary.find(path) != m_terrainLibrary.end())
        return m_terrainLibrary[path].get();

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_ConvertToLeftHanded);
    if (scene == nullptr)
    {
        EngineHelper::SuccessCheck(false, "Assimp Scene 로드 실패: " + path);
        return nullptr;
    }

    if (scene->mRootNode == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
    {
        EngineHelper::SuccessCheck(false, "Assimp 데이터 불완전: " + path);
        return nullptr;
    }

    auto newTerrainModel = std::make_unique<TerrainModel>();

    size_t lastSlash = path.find_last_of("\\/");
    std::string directory = (lastSlash == std::string::npos) ? "." : path.substr(0, lastSlash);

    ProcessMaterialsForTerrain(scene, device, context, texManager, directory, newTerrainModel.get());

    ExtractAllMeshData(scene->mRootNode, scene, newTerrainModel.get());
    // 데이터 수집이 끝났으니 셀 분할 작업 시작
    if (newTerrainModel->BuildTerrainCells(device) == false)
    {
        EngineHelper::DebugPrint("ModelMananger: newTerrainModel->BuildTerrainCells(device)  실패");
        return nullptr;
    }
    m_terrainLibrary[path] = std::move(newTerrainModel);
    return m_terrainLibrary[path].get();
}

//////////////////////////////////////////////////////////////////////////

/* private */
//////////////////////////////////////////////////////////////////////////

void ModelManager::ProcessNode(
    aiNode* node, const aiScene* scene, 
    ID3D11Device* device, MeshModel* outModel)
{
    // 현재 노드의 모든 메쉬 처리
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        outModel->AddMesh(ProcessMesh(mesh, scene, device));
    }

    // 자식 노드들도 처리
    for (unsigned int i = 0; i < node->mNumChildren; i++)
        ProcessNode(node->mChildren[i], scene, device, outModel);

    return;
} // ProcessNode


std::unique_ptr<Mesh> ModelManager::ProcessMesh(
    aiMesh* mesh, const aiScene* scene,
    ID3D11Device* device)
{
    std::vector<ModelVertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        ModelVertex vertex;
        vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

        if (mesh->mTextureCoords[0])
        {
            vertex.texture.x = (float)mesh->mTextureCoords[0][i].x;
            vertex.texture.y = (float)mesh->mTextureCoords[0][i].y;
        }
        else
            vertex.texture = DirectX::XMFLOAT2(0.0f, 0.0f);

        if (mesh->HasNormals())
            vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

        if (mesh->HasTangentsAndBitangents())
        {
            vertex.tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
            vertex.binormal = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
        }
        else
        {
            vertex.tangent = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
            vertex.binormal = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    MeshData meshData;
    meshData.vertices = vertices;
    meshData.indices = indices;

    auto newMesh = std::make_unique<Mesh>();
    if (!newMesh->Init(device, meshData, mesh->mMaterialIndex))
        return nullptr;
    return newMesh;
} // ProcessMesh


void ModelManager::ProcessMaterials(
    const aiScene* scene, ID3D11Device* device,
    ID3D11DeviceContext* context, TexturesManager* texManager,
    const std::string& directory, MeshModel* outModel)
{
    std::string pbrDir = directory + "/textures/";

    size_t lastSlash = directory.find_last_of("\\/");
    std::string parentDir = directory.substr(0, lastSlash); // "assets/Stone"
    size_t modelSlash = parentDir.find_last_of("\\/");
    std::string modelName = (modelSlash == std::string::npos) ? parentDir : parentDir.substr(modelSlash + 1);

    for (unsigned int i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial* aiMat = scene->mMaterials[i];
        Material myMaterial;
        myMaterial.name = aiMat->GetName().C_Str();

        //// Assimp 로더
        //auto loadTex = [&](aiTextureType type) -> std::shared_ptr<Texture> {
        //    aiString path;
        //    if (aiMat->GetTexture(type, 0, &path) == AI_SUCCESS) {
        //        std::string fullPath = directory + "/" + path.C_Str();
        //        auto tex = texManager->GetTexture(device, context, fullPath);

        //        EngineHelper::SuccessCheck(tex != nullptr, "일반 텍스처 로드 실패: " + fullPath);
        //        return tex;
        //    }
        //    return nullptr;
        //};

        //myMaterial.diffuse = loadTex(aiTextureType_DIFFUSE);
        //myMaterial.ambient = loadTex(aiTextureType_AMBIENT);
        //myMaterial.specular = loadTex(aiTextureType_SPECULAR);
        //myMaterial.normal = loadTex(aiTextureType_NORMALS);

        std::string matName = myMaterial.name;
        std::transform(matName.begin(), matName.end(), matName.begin(), ::tolower); // 소문자 변환

        if (matName.find("cloud") != std::string::npos)
            myMaterial.type = MaterialType::CLOUD;
        else if (matName.find("cliff") != std::string::npos || matName.find("ciff") != std::string::npos)
            myMaterial.type = MaterialType::CLIFF;
        else if (matName.find("stone") != std::string::npos)
            myMaterial.type = MaterialType::STONE;
        else
            myMaterial.type = MaterialType::BASE;

        auto getPBR = [&](const std::string& suffix) -> std::shared_ptr<Texture> {
            std::string fullPath = pbrDir + modelName + suffix;
            auto tex = texManager->GetTexture(device, context, fullPath);

            EngineHelper::SuccessCheck(tex != nullptr, "PBR 로드 실패: " + fullPath);
            return tex;
        };

        //myMaterial.albedo = getPBR("_BaseColor.png");
        //myMaterial.normal = getPBR("_normal.png");
        //myMaterial.metallic = getPBR("_Metallic.png");
        //myMaterial.roughness = getPBR("_Roughness.png");
        //myMaterial.ao = getPBR("_ao.png");
        myMaterial.albedo = LoadPBRTexture(device, context, texManager, pbrDir, modelName, "_BaseColor");
        myMaterial.normal = LoadPBRTexture(device, context, texManager, pbrDir, modelName, "_normal");
        myMaterial.metallic = LoadPBRTexture(device, context, texManager, pbrDir, modelName, "_Metallic");
        myMaterial.roughness = LoadPBRTexture(device, context, texManager, pbrDir, modelName, "_Roughness");
        myMaterial.ao = LoadPBRTexture(device, context, texManager, pbrDir, modelName, "_ao");

        outModel->AddMaterial(myMaterial);

    } // for
} // ProcessMaterials


void ModelManager::ProcessMaterialsForTerrain(
    const aiScene* scene, ID3D11Device* device,
    ID3D11DeviceContext* context, TexturesManager* texManager,
    const std::string& directory, TerrainModel* outModel)
{
    std::string pbrDir = directory + "/textures/";

    size_t lastSlash = directory.find_last_of("\\/");
    std::string parentDir = directory.substr(0, lastSlash);
    size_t modelSlash = parentDir.find_last_of("\\/");
    std::string modelName = (modelSlash == std::string::npos) ? parentDir : parentDir.substr(modelSlash + 1);

    for (unsigned int i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial* aiMat = scene->mMaterials[i];
        Material myMaterial;
        myMaterial.name = aiMat->GetName().C_Str();
        auto getPBR = [&](const std::string& suffix) -> std::shared_ptr<Texture> {
            std::string fullPath = pbrDir + modelName + suffix;
            auto tex = texManager->GetTexture(device, context, fullPath);

            EngineHelper::SuccessCheck(tex != nullptr, "PBR 로드 실패: " + fullPath);
            return tex;
        };

        myMaterial.albedo = LoadPBRTexture(device, context, texManager, pbrDir, modelName, "_BaseColor");
        myMaterial.normal = LoadPBRTexture(device, context, texManager, pbrDir, modelName, "_normal");
        myMaterial.metallic = LoadPBRTexture(device, context, texManager, pbrDir, modelName, "_Metallic");
        myMaterial.roughness = LoadPBRTexture(device, context, texManager, pbrDir, modelName, "_Roughness");
        myMaterial.ao = LoadPBRTexture(device, context, texManager, pbrDir, modelName, "_ao");

        outModel->AddMaterial(myMaterial);

    } // for
} // ProcessMaterialsForTerrain


void ModelManager::ExtractAllMeshData(aiNode* node, const aiScene* scene, TerrainModel* outModel)
{
    // 1. 현재 노드에 포함된 메쉬들 처리
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        std::vector<ModelVertex> vertices;
        std::vector<unsigned int> indices;

        // --- 정점 데이터 추출 ---
        for (unsigned int vIdx = 0; vIdx < mesh->mNumVertices; vIdx++)
        {
            ModelVertex vertex;
            // 위치
            vertex.position = { mesh->mVertices[vIdx].x, mesh->mVertices[vIdx].y, mesh->mVertices[vIdx].z };

            // 텍스처 좌표
            if (mesh->mTextureCoords[0])
                vertex.texture = { (float)mesh->mTextureCoords[0][vIdx].x, (float)mesh->mTextureCoords[0][vIdx].y };
            else
                vertex.texture = { 0.0f, 0.0f };

            // 법선(Normal)
            if (mesh->HasNormals())
                vertex.normal = { mesh->mNormals[vIdx].x, mesh->mNormals[vIdx].y, mesh->mNormals[vIdx].z };

            // 탄젠트 및 바이노멀 (PBR용)
            if (mesh->HasTangentsAndBitangents())
            {
                vertex.tangent = { mesh->mTangents[vIdx].x, mesh->mTangents[vIdx].y, mesh->mTangents[vIdx].z };
                vertex.binormal = { mesh->mBitangents[vIdx].x, mesh->mBitangents[vIdx].y, mesh->mBitangents[vIdx].z };
            }

            vertices.push_back(vertex);
        }

        // --- 인덱스 데이터 추출 ---
        for (unsigned int fIdx = 0; fIdx < mesh->mNumFaces; fIdx++)
        {
            aiFace face = mesh->mFaces[fIdx];
            for (unsigned int idx = 0; idx < face.mNumIndices; idx++)
            {
                indices.push_back(face.mIndices[idx]);
            }
        }

        outModel->AddTerrainData(vertices, indices);
    }

    // 2. 자식 노드들도 재귀적으로 처리
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ExtractAllMeshData(node->mChildren[i], scene, outModel);
    }

    return;
} // ExtractAllMeshData


std::shared_ptr<Texture> ModelManager::LoadPBRTexture(
    ID3D11Device* device,
    ID3D11DeviceContext* context,
    TexturesManager* texManager,
    const std::string& pbrDir,
    const std::string& modelName,
    const std::string& suffix)
{
    static const std::vector<std::string> extensions = { ".png", ".jpg", ".jpeg", ".tga", ".dds" };

    std::string basePath = pbrDir + modelName + suffix;

    for (const auto& ext : extensions)
    {
        std::string fullPath = basePath + ext;

        if (std::filesystem::exists(fullPath))
        {
            auto tex = texManager->GetTexture(device, context, fullPath);
            if (tex) return tex;
        }
    }

    return nullptr;
} // LoadPBRTexture
