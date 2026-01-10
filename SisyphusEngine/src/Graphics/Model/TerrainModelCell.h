// Graphics/MeshModel/TerrainModelCell.h
#pragma once
#include <d3d11.h>
#include <memory>
#include <vector>
#include "Model/Mesh/VertexTypes.h"

class VertexBuffer;
class IndexBuffer;
class Frustum;

class TerrainModelCell {
public:
    TerrainModelCell();
	TerrainModelCell(const TerrainModelCell&) = delete;
    ~TerrainModelCell();

    bool Init(
        ID3D11Device* device,
        const std::vector<ModelVertex>&,
        const std::vector<unsigned int>&);

    // Frustum을 받아 Culling 후 렌더링
    void Render(ID3D11DeviceContext* context, Frustum* frustum);

private:
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;

    // Culling용 범위 정보
    float m_maxWidth, m_maxHeight, m_maxDepth;
    float m_minWidth, m_minHeight, m_minDepth;
}; // TerrainModelCell