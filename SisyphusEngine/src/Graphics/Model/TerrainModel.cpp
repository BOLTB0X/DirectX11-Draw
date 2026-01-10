// Graphics/Model/TerrainModel.cpp
#include "TerrainModel.h"
#include "TerrainModelCell.h"
#include "Camera/Frustum.h"
#include "MathHelper.h"
#include "EngineHelper.h"
#include <cstdio>
/* default */
////////////////////////////////////////////////////////////////////

TerrainModel::TerrainModel() { }


TerrainModel::~TerrainModel() {}


void TerrainModel::Render(ID3D11DeviceContext* context, Frustum* frustum)
{

    if (m_materials.empty())
    {
        EngineHelper::DebugPrint("TerrainModel:Render 내 m_materials 이 비어있음");
        return;
    }
    const Material& terrainMaterial = m_materials[0];

    if (terrainMaterial.albedo)    terrainMaterial.albedo->Bind(context, 0);
    if (terrainMaterial.normal)    terrainMaterial.normal->Bind(context, 1);
    if (terrainMaterial.metallic)  terrainMaterial.metallic->Bind(context, 2);
    if (terrainMaterial.roughness) terrainMaterial.roughness->Bind(context, 3);
    if (terrainMaterial.ao)        terrainMaterial.ao->Bind(context, 4);

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //printf("m_cells: %d\n", m_cells.size());
    if (m_cells.empty())
    {
        EngineHelper::DebugPrint("TerrainModel: m_cells 이 비어있음");
        return;
    }
    for (auto& cell : m_cells)
        cell->Render(context, frustum);
} // Render


void TerrainModel::AddTerrainData(const std::vector<ModelVertex>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int vertexOffset = static_cast<unsigned int>(m_fullVertices.size());
    m_fullVertices.insert(m_fullVertices.end(), vertices.begin(), vertices.end());

    for (auto idx : indices)
        m_fullIndices.push_back(idx + vertexOffset);
    return;
} // AddTerrainData


void TerrainModel::AddMaterial(const Material& material)
{
    m_materials.push_back(material);
} // AddMaterial


bool TerrainModel::BuildTerrainCells(ID3D11Device* device)
{
    if (m_fullVertices.empty() || m_fullIndices.empty()) return false;

    float minX = FLT_MAX, minZ = FLT_MAX, maxX = -FLT_MAX, maxZ = -FLT_MAX;

    for (const auto& v : m_fullVertices)
    {
        minX = (std::min)(minX, v.position.x);
        minZ = (std::min)(minZ, v.position.z);
        maxX = (std::max)(maxX, v.position.x);
        maxZ = (std::max)(maxZ, v.position.z);
    }

    const int cellsPerRow = 4;
    float cellWidth = (maxX - minX) / cellsPerRow;
    float cellDepth = (maxZ - minZ) / cellsPerRow;

    // 임시 바구니 생성
    std::vector<TempCell> tempCells(cellsPerRow * cellsPerRow);

    if (m_fullIndices.empty()) return false;

    // 2. 삼각형 분할 로직 (3개씩 끊어서 처리)
    for (size_t i = 0; i < m_fullIndices.size(); i += 3)
    {
        ModelVertex v[3] = { m_fullVertices[m_fullIndices[i]], m_fullVertices[m_fullIndices[i + 1]], m_fullVertices[m_fullIndices[i + 2]] };

        // 중심점 기준으로 어느 셀에 들어갈지 결정
        float cx = (v[0].position.x + v[1].position.x + v[2].position.x) / 3.0f;
        float cz = (v[0].position.z + v[1].position.z + v[2].position.z) / 3.0f;

        int ix = MathHelper::clamp(static_cast<int>((cx - minX) / cellWidth), 0, cellsPerRow - 1);
        int iz = MathHelper::clamp(static_cast<int>((cz - minZ) / cellDepth), 0, cellsPerRow - 1);
        int cellIdx = (iz * cellsPerRow) + ix;

        // 해당 셀에 정점과 인덱스 추가
        unsigned int base = (unsigned int)tempCells[cellIdx].v.size();
        for (int k = 0; k < 3; ++k)
            tempCells[cellIdx].v.push_back(v[k]);

        tempCells[cellIdx].i.push_back(base);
        tempCells[cellIdx].i.push_back(base + 1);
        tempCells[cellIdx].i.push_back(base + 2);
    } // for

    m_cells.clear();
    for (auto& data : tempCells)
    {
        if (data.v.empty()) continue;

        auto cell = std::make_unique<TerrainModelCell>();
        if (cell->Init(device, data.v, data.i))
            m_cells.push_back(std::move(cell));
    }

    EngineHelper::DebugPrint("TerrainModel: " + std::to_string(m_cells.size()) + " Cells Built.");

    return true;
} // BuildTerrainCells