#include "AssimpLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

bool AssimpLoader::Load(
    const std::string& path,
    std::vector<MeshData>& outMeshes
)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path.c_str(),
        aiProcess_Triangulate |
        aiProcess_GenNormals |
        aiProcess_JoinIdenticalVertices
    );

    if (!scene || !scene->HasMeshes())
        return false;

    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
        MeshData data;

        for (unsigned int v = 0; v < mesh->mNumVertices; v++)
        {
            VertexPN vert{};
            vert.position = {
                mesh->mVertices[v].x,
                mesh->mVertices[v].y,
                mesh->mVertices[v].z
            };

            vert.normal = {
                mesh->mNormals[v].x,
                mesh->mNormals[v].y,
                mesh->mNormals[v].z
            };

            data.vertices.push_back(vert);
        }

        for (unsigned int f = 0; f < mesh->mNumFaces; f++)
        {
            aiFace& face = mesh->mFaces[f];
            data.indices.push_back(face.mIndices[0]);
            data.indices.push_back(face.mIndices[1]);
            data.indices.push_back(face.mIndices[2]);
        }

        outMeshes.push_back(data);
    }

    return true;
}
