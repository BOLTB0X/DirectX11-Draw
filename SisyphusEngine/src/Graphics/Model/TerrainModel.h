// Graphics/Model/TerrainModel.h
#pragma once
#include <d3d11.h>
#include <vector>
#include <memory>
// Graphics
#include "HeightMap/HeightMap.h"
#include "Model/Mesh/VertexTypes.h"
#include "Model/Texture/Material.h"
#include "Shader/ConstantBuffer.h"

class Frustum;
class TerrainModelCell;
class HeightMap;
class TerrainShader;

class TerrainModel {
public:
    TerrainModel();
    TerrainModel(const TerrainModel&) = delete;
    ~TerrainModel();

   // bool Init(ID3D11Device* device, HeightMap* heightMap, int cellDimension = 42);
    void Render(ID3D11DeviceContext* context, TerrainShader* shader, Frustum* frustuml, DirectX::XMMATRIX worldMatrix);

public:
    bool InitConstantBuffer(ID3D11Device* device);
    bool InitHeightMap();
    bool CreateCells(ID3D11Device* device, int cellDimension = 42);

public:
    void AddTerrainData(const std::vector<ModelVertex>&, const std::vector<unsigned int>&);
    void AddMaterial(const Material&);
    bool BuildTerrainCells(ID3D11Device*);

public:
    const std::vector<ModelVertex>& GetFullVertices() const { return m_fullVertices; }
    const std::vector<unsigned int>& GetFullIndices() const { return m_fullIndices; }
    const Material& GetMaterial(size_t) const;
    const std::vector<std::unique_ptr<TerrainModelCell>>& GetCell() const { return m_cells; }
    HeightMap* GetHeightMap() { return m_HeightMap.get(); }
    void SetHeightMap(std::unique_ptr<HeightMap>);

private:
    struct TempCell { 
        std::vector<ModelVertex> v;
        std::vector<unsigned int> i;
    };

    void CalculateTangentBinormal(const ModelVertex& v1, const ModelVertex& v2, const ModelVertex& v3,
        DirectX::XMFLOAT3& tangent, DirectX::XMFLOAT3& binormal);

private:
    std::vector<ModelVertex> m_fullVertices;
    std::vector<unsigned int> m_fullIndices;
    std::vector<Material> m_materials;
    std::vector<std::unique_ptr<TerrainModelCell>> m_cells;
    std::unique_ptr<ConstantBuffer<MaterialBuffer>> m_materialBuffer;
    std::unique_ptr<HeightMap> m_HeightMap;

    int m_terrainWidth, m_terrainHeight;
}; // TerrainModel