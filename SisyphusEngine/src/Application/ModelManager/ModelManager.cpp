// Application/ModelManager/ModelManager.cpp
#include "ModelManager.h"

/* default */
///////////////////////////////////////////////////////////////////////////

ModelManager::ModelManager() { };

ModelManager::~ModelManager() { Shutdown(); };

bool ModelManager::Init()
{ 
	return true;
}


void ModelManager::Shutdown()
{
	m_modelLibrary.clear();
}

/////////////////////////////////////////////////////////////////////////


/* public */
//////////////////////////////////////////////////////////////////////////

Model* ModelManager::GetModel(
    ID3D11Device* device,
    ID3D11DeviceContext* context,
    TexturesManager* texManager,
    const std::string& path)
{
    if (m_modelLibrary.find(path) != m_modelLibrary.end())
        return m_modelLibrary[path].get();

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace);

    if (!scene || !scene->mRootNode) return nullptr;

    auto newModel = std::make_unique<Model>();
    std::string directory = path.substr(0, path.find_last_of("\\/"));

    // 머테리어 처리
    ProcessMaterials(scene, device, context, texManager, directory, newModel.get());

    // 노드 순회하며 메쉬 처리
    ProcessNode(scene->mRootNode, scene, device, newModel.get());

    m_modelLibrary[path] = std::move(newModel);
    return m_modelLibrary[path].get();
} // GetModel
//////////////////////////////////////////////////////////////////////////

/* private */
//////////////////////////////////////////////////////////////////////////

void ModelManager::ProcessNode(
    aiNode* node, const aiScene* scene, 
    ID3D11Device* device, Model* outModel)
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
    MeshData data;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        ModelVertex vertex;
        // 위치
        vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

        // UV 좌표
        if (mesh->mTextureCoords[0])
            vertex.texture = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
        else
            vertex.texture = { 0.0f, 0.0f };

        // 법선 벡터 (Normal)
        if (mesh->HasNormals())
            vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

        // 탄젠트
        if (mesh->HasTangentsAndBitangents())
            vertex.tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };

        data.vertices.push_back(vertex);
    }

    // 인덱스 처리
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            data.indices.push_back(face.mIndices[j]);
    }

    auto newMesh = std::make_unique<Mesh>();
    newMesh->Init(device, data, mesh->mMaterialIndex); // MeshData를 이용해 버퍼 생성
    return newMesh;
} // ProcessMesh


void ModelManager::ProcessMaterials(
    const aiScene* scene, ID3D11Device* device,
    ID3D11DeviceContext* context, TexturesManager* texManager,
    const std::string& directory, Model* outModel)
{
    for (unsigned int i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial* aiMat = scene->mMaterials[i];
        Material myMaterial;
        myMaterial.name = aiMat->GetName().C_Str();

        auto loadTex = [&](aiTextureType type) -> std::shared_ptr<Texture> {
            aiString path;
            if (aiMat->GetTexture(type, 0, &path) == AI_SUCCESS)
            {
                // 절대 경로/상대 경로 아용하여 전체 경로 생성
                std::string fullPath = directory + "/" + path.C_Str();
                return texManager->GetTexture(device, context, fullPath);
            }
            return nullptr;
        };

        myMaterial.diffuse = loadTex(aiTextureType_DIFFUSE);
        myMaterial.ambient = loadTex(aiTextureType_AMBIENT);
        myMaterial.specular = loadTex(aiTextureType_SPECULAR);
        myMaterial.albedo = loadTex(aiTextureType_BASE_COLOR);

        myMaterial.normal = loadTex(aiTextureType_NORMALS);
        if (!myMaterial.normal)
            myMaterial.normal = loadTex(aiTextureType_HEIGHT);

        // PBR 수치 로드
        aiMat->Get(AI_MATKEY_ROUGHNESS_FACTOR, myMaterial.roughness);
        aiMat->Get(AI_MATKEY_METALLIC_FACTOR, myMaterial.metalness);

        outModel->AddMaterial(myMaterial);
    }
} // ProcessMaterials