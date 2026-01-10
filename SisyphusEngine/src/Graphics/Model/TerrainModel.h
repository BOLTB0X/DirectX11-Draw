// Graphics/Model/TerrainModel.h
#pragma once
#include <d3d11.h>
#include <vector>
#include <memory>

#include "Model/Mesh/VertexTypes.h"
#include "Model/Texture/Material.h"

class Frustum;
class TerrainModelCell;


class TerrainModel {
public:
    TerrainModel();
    TerrainModel(const TerrainModel&) = delete;
    ~TerrainModel();

    void Render(ID3D11DeviceContext* context, Frustum* frustuml);

public:
    void AddTerrainData(const std::vector<ModelVertex>&, const std::vector<unsigned int>&);
    void AddMaterial(const Material&);
    bool BuildTerrainCells(ID3D11Device*);

public:
    const std::vector<ModelVertex>& GetFullVertices() const { return m_fullVertices; }
    const std::vector<unsigned int>& GetFullIndices() const { return m_fullIndices; }
    const Material& GetMaterial(size_t index = 0) const { return m_materials[index]; }

private:
    struct TempCell { 
        std::vector<ModelVertex> v;
        std::vector<unsigned int> i;
    };

private:
    std::vector<ModelVertex> m_fullVertices;
    std::vector<unsigned int> m_fullIndices;
    std::vector<Material> m_materials;
    std::vector<std::unique_ptr<TerrainModelCell>> m_cells;
}; // TerrainModel