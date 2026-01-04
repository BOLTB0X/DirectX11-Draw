// Graphics/Model/VertexTypes.h
#pragma once
#include <directxmath.h>
#include <vector>

struct ModelVertex {
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT2 texture;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT3 tangent;
}; // ModelVertex


struct MeshData {
    std::vector<ModelVertex> vertices;
    std::vector<unsigned int> indices;
}; // MeshData