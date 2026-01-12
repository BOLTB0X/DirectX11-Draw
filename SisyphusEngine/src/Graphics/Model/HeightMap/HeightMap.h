// World/Terrain/HeightMap.h
#pragma once
#include <vector>
#include <DirectXMath.h>
#include <string>
// Graphics
#include "Model/Mesh/VertexTypes.h"

class HeightMap {
public:
    HeightMap();
    HeightMap(const HeightMap&) = delete;
    ~HeightMap();

    bool Init(const std::vector<ModelVertex>&, const std::vector<unsigned int>&);

public:
    void GetBounds(DirectX::XMFLOAT3&, DirectX::XMFLOAT3&) const;
    float GetHeightAt(float, float) const;

public:
    bool CheckHeightOfTriangle(float, float, float&, float v0[3], float v1[3], float v2[3]);

private:
    std::vector<ModelVertex> m_vertices;
    std::vector<unsigned int> m_indices;

    DirectX::XMFLOAT3 m_minBounds;
    DirectX::XMFLOAT3 m_maxBounds;

    int m_gridWidth;
    int m_gridHeight;
    float m_cellSpacing;
}; // HeightMap