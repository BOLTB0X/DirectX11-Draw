#include "ModelLoader.h"

bool ModelLoader::LoadFromFile(const std::string& filename, MeshData& outData)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        filename,
        aiProcess_Triangulate |
        aiProcess_ConvertToLeftHanded |
        aiProcess_CalcTangentSpace |
        aiProcess_GenNormals |
        aiProcess_JoinIdenticalVertices
    );

    if (!scene || !scene->HasMeshes())
        return false;

    // 지금은 단일 메시만 처리 (Stone용)
    ProcessMesh(scene->mMeshes[0], scene, outData);

    return true;
} // LoadFromFile


void ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, MeshData& outData)
{
    outData.vertices.reserve(mesh->mNumVertices);

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        VertexType v{};

        // Position
        v.position = {
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        };

        // Normal
        if (mesh->HasNormals())
        {
            v.normal = {
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            };
        }

        // UV
        //if (mesh->HasTextureCoords(0))
        //{
        //    v.uv = {
        //        mesh->mTextureCoords[0][i].x,
        //        mesh->mTextureCoords[0][i].y
        //    };
        //}

        // Tangent / Binormal (NormalMap 대비)
        if (mesh->HasTangentsAndBitangents())
        {
            v.tangent = {
                mesh->mTangents[i].x,
                mesh->mTangents[i].y,
                mesh->mTangents[i].z
            };

            v.binormal = {
                mesh->mBitangents[i].x,
                mesh->mBitangents[i].y,
                mesh->mBitangents[i].z
            };
        }

        outData.vertices.push_back(v);
    }

    // Index
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            outData.indices.push_back(face.mIndices[j]);
        }
    }

    // Texture (Diffuse / Normal)
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        auto loadTex = [&](aiTextureType type)
            {
                if (material->GetTextureCount(type) > 0)
                {
                    aiString path;
                    material->GetTexture(type, 0, &path);
                    outData.texturePaths.push_back(path.C_Str());
                }
            };

        loadTex(aiTextureType_DIFFUSE);
        loadTex(aiTextureType_NORMALS);
    }
}