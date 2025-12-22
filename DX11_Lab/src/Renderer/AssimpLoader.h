#pragma once
#include <string>
#include <vector>
#include "VertexTypes.h"
#include "Mesh.h"

struct MeshData
{
    std::vector<VertexPN> vertices;
    std::vector<uint32_t> indices;
};

class AssimpLoader
{
public:
    static bool Load(
        const std::string& path,
        std::vector<MeshData>& outMeshes
    );
};
