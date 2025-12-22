#include "Model.h"
#include "AssimpLoader.h"

bool Model::Load(ID3D11Device* device, const std::string& path)
{
    std::vector<MeshData> meshes;
    if (!AssimpLoader::Load(path, meshes))
        return false;

    for (auto& data : meshes)
    {
        Mesh mesh;
        mesh.Init(device, data.vertices, data.indices);
        mMeshes.push_back(mesh);
    }

    return true;
} // Load

void Model::Draw(ID3D11DeviceContext* context)
{
    for (auto& mesh : mMeshes)
        mesh.Draw(context);
} // Draw
