// Graphics/Model/TerrainModel.cpp
#include "TerrainModel.h"
#include "TerrainModelCell.h"
#include "HeightMap/HeightMapType.h"
#include "Model/Texture/Material.h"
#include "Camera/Frustum.h"
#include "Shader/TerrainShader.h"
// Common
#include "MathHelper.h"
#include "EngineHelper.h"
#include "EngineSettings.h"

/* default */
////////////////////////////////////////////////////////////////////

TerrainModel::TerrainModel()
    : m_HeightMap(nullptr),
    m_terrainWidth(0),
    m_terrainHeight(0),
    m_time(0.0f)
{}


TerrainModel::~TerrainModel() = default;


void TerrainModel::Render(
    ID3D11DeviceContext* context,
    TerrainShader* shader,
    Frustum* frustum,
    DirectX::XMMATRIX worldMatrix)
{
    DirectX::XMFLOAT4X4 worldFB4;
    DirectX::XMStoreFloat4x4(&worldFB4, worldMatrix);
    float offsetX = worldFB4._41;
    float offsetY = worldFB4._42;
    float offsetZ = worldFB4._43;

    if (m_materials.empty())
    {
        EngineHelper::DebugPrint("TerrainModel:Render 내 m_materials 이 비어있음");
        return;
    }

    const Material& terrainMaterial = m_materials.front();
    MaterialBuffer data;
    data.type = (int)terrainMaterial.type;
    data.gTime = m_time;

    m_materialBuffer->Update(context, data);
    m_materialBuffer->BindPS(context, 2);

    //if (terrainMaterial.normal) terrainMaterial.normal->Bind(context, 0);
    //if (terrainMaterial.metallic) terrainMaterial.metallic->Bind(context, 1);
    if (terrainMaterial.alpha) terrainMaterial.alpha->Bind(context, 0);
    if (terrainMaterial.roughness) terrainMaterial.roughness->Bind(context, 1);

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    if (m_cells.empty())
    {
        EngineHelper::DebugPrint("TerrainModel: m_cells 이 비어있음");
        return;
    }

    unsigned int drawCount = 0;
    unsigned int cullCount = 0;

    for (auto& cell : m_cells)
    {
        float maxWidth = cell->GetMaxWidth() + offsetX;
        float maxHeight = cell->GetMaxHeight() + offsetY;
        float maxDepth = cell->GetMaxDepth() + offsetZ;

        float minWidth = cell->GetMinWidth() + offsetX;
        float minHeight = cell->GetMinHeight() + offsetY;
        float minDepth = cell->GetMinDepth() + offsetZ;

        if (frustum->CheckBoundingBoxMinMax(maxWidth, maxHeight, maxDepth, minWidth, minHeight, minDepth))
        {
            cell->Render(context);
            drawCount++;
        }
        else
        {
            cullCount++;
        }
    }
    return;
} // Render

////////////////////////////////////////////////////////////////////

/* public */
///////////////////////////////////////////////////////////////////


bool TerrainModel::InitConstantBuffer(ID3D11Device* device)
{
    m_materialBuffer = std::make_unique<ConstantBuffer<MaterialBuffer>>();
    if (m_materialBuffer->Init(device) == false) return false;

    return true;
} // InitConstantBuffer

void TerrainModel::InitHeightMap()
{
    m_HeightMap = std::make_unique<HeightMap>();
    int w = EngineSettings::terrainWidth;
    int h = EngineSettings::terrainHeight;

    m_HeightMap->InitPlane(w, h);
    m_terrainWidth = w;
    m_terrainHeight = h;
    //// 이미 소유권 이전을 통해 m_HeightMap이 설정되어 있는지 확인
    //if (m_HeightMap == nullptr)
    //{
    //    EngineHelper::DebugPrint("TerrainModel: m_HeightMap이 nullptr, InitHeightMap 실패.");
    //    return false;
    //}
    //DirectX::XMFLOAT3 minB, maxB;
    //m_HeightMap->GetBounds(minB, maxB);
    //m_terrainWidth = static_cast<int>(maxB.x - minB.x) + 1;
    //m_terrainHeight = static_cast<int>(maxB.z - minB.z) + 1;

    //return true;
} // InitHeightMap


void TerrainModel::InitMaterial(const Material& mat)
{
    this->AddMaterial(mat);
} // InitMaterial


void TerrainModel::AddTerrainData(const std::vector<ModelVertex>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int vertexOffset = static_cast<unsigned int>(m_fullVertices.size());
    m_fullVertices.insert(m_fullVertices.end(), vertices.begin(), vertices.end());
    unsigned int startIdx = (unsigned int)m_fullVertices.size();

    for (auto idx : indices)
    {
        if (startIdx + idx >= m_fullVertices.size())
        {
            EngineHelper::DebugPrint("에러: 잘못된 인덱스 참조!");
            continue;
        }
        m_fullIndices.push_back(idx + startIdx);
    }
    return;
} // AddTerrainData


void TerrainModel::AddMaterial(const Material& material)
{
    m_materials.push_back(material);
    return;
} // AddMaterial


bool TerrainModel::BuildTerrainCells(ID3D11Device* device)
{
    if (m_fullVertices.empty() || m_fullIndices.empty())
    {
        EngineHelper::DebugPrint("TerrainModel::BuildTerrainCells 에러, m_fullVertices/m_fullIndices 비어있음 ");
        return false;
    }

    float minX = FLT_MAX, minZ = FLT_MAX, maxX = -FLT_MAX, maxZ = -FLT_MAX;

    for (const auto& v : m_fullVertices)
    {
        minX = (std::min)(minX, v.position.x);
        minZ = (std::min)(minZ, v.position.z);
        maxX = (std::max)(maxX, v.position.x);
        maxZ = (std::max)(maxZ, v.position.z);
    }

    float width = maxX - minX;
    float depth = maxZ - minZ;
    if (width <= 0.0001f || depth <= 0.0001f)
    {
        EngineHelper::DebugPrint("TerrainModel::BuildTerrainCells 지형 크기가 너무 작거나 데이터가 비정상");
        return false;
    }

    const int cellsPerRow = 4;
    float cellWidth = (maxX - minX) / cellsPerRow;
    float cellDepth = (maxZ - minZ) / cellsPerRow;

    std::vector<TempCell> tempCells(cellsPerRow * cellsPerRow);

    if (m_fullIndices.empty()) return false;

    for (size_t i = 0; i < m_fullIndices.size(); i += 3)
    {
        ModelVertex v[3] = { m_fullVertices[m_fullIndices[i]], m_fullVertices[m_fullIndices[i + 1]], m_fullVertices[m_fullIndices[i + 2]] };

        float cx = (v[0].position.x + v[1].position.x + v[2].position.x) / 3.0f;
        float cz = (v[0].position.z + v[1].position.z + v[2].position.z) / 3.0f;

        int ix = MathHelper::clamp(static_cast<int>((cx - minX) / cellWidth), 0, cellsPerRow - 1);
        int iz = MathHelper::clamp(static_cast<int>((cz - minZ) / cellDepth), 0, cellsPerRow - 1);
        int cellIdx = (iz * cellsPerRow) + ix;

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


const Material& TerrainModel::GetMaterial(size_t index) const
{
    if (index >= m_materials.size())
    {
        static Material defaultMaterial;
        return defaultMaterial;
    }
    return m_materials[index];
} // GetMaterial


bool TerrainModel::CreateCells(ID3D11Device* device, int cellDimension)
{
    int cellsPerRow = (m_terrainWidth - 1) / (cellDimension - 1);
    int cellsPerCol = (m_terrainHeight - 1) / (cellDimension - 1);

    for (int cz = 0; cz < cellsPerCol; cz++)
    {
        for (int cx = 0; cx < cellsPerRow; cx++)
        {

            std::vector<ModelVertex> cellVertices;
            std::vector<unsigned int> cellIndices;

            //  버텍스 기본 데이터 채우기
            for (int z = 0; z < cellDimension; z++)
            {
                for (int x = 0; x < cellDimension; x++)
                {
                    int globalX = cx * (cellDimension - 1) + x;
                    int globalZ = cz * (cellDimension - 1) + z;

                    // HeightMap의 RAW 데이터에서 미리 계산된 좌표와 노말을 가져옴
                    auto& hData = m_HeightMap->GetRawData()[(globalZ * m_terrainWidth) + globalX];

                    ModelVertex v;
                    v.position = { hData.x, 0.0f, hData.z };
                    //v.position = { hData.x, hData.y, hData.z };
                    v.texture = { (float)globalX / (m_terrainWidth - 1), (float)globalZ / (m_terrainHeight - 1) };

                    // HeightMap이 가진 고유 노말값을 그대로 복사
                    v.normal = { 0.0f, 1.0f, 0.0f };
                    //v.normal = { hData.nx, hData.ny, hData.nz };

                    // 벡터 누적을 위해 초기화
                    v.tangent = { 0, 0, 0 };
                    v.binormal = { 0, 0, 0 };

                    cellVertices.push_back(v);
                }
            } // for - 버텍스 기본 데이터 채우기

            // 인덱스 생성 및 Tangent/Binormal 누적 계산
            for (int z = 0; z < cellDimension - 1; z++)
            {
                for (int x = 0; x < cellDimension - 1; x++)
                {
                    unsigned int i0 = (z * cellDimension) + x;
                    unsigned int i1 = (z * cellDimension) + (x + 1);
                    unsigned int i2 = ((z + 1) * cellDimension) + x;
                    unsigned int i3 = ((z + 1) * cellDimension) + (x + 1);

                    // 두 개의 삼각형에 대해 각각 Tangent 계산 및 누적
                    auto accumulateVectors = [&](int idx0, int idx1, int idx2)
                        {
                        DirectX::XMFLOAT3 t, b;
                        MathHelper::CalculateTangentBinormal(cellVertices[idx0], cellVertices[idx1], cellVertices[idx2], t, b);

                        // 공유되는 각 정점에 결과값을 더해줌
                        auto addVec = [](DirectX::XMFLOAT3& dest, const DirectX::XMFLOAT3& src) {
                            dest.x += src.x; dest.y += src.y; dest.z += src.z;
                            };
                        addVec(cellVertices[idx0].tangent, t); addVec(cellVertices[idx0].binormal, b);
                        addVec(cellVertices[idx1].tangent, t); addVec(cellVertices[idx1].binormal, b);
                        addVec(cellVertices[idx2].tangent, t); addVec(cellVertices[idx2].binormal, b);
                        }; // 두 개의 삼각형에 대해 각각 Tangent 계산 및 누적 

                    accumulateVectors(i0, i2, i1); // 첫 번째 삼각형
                    accumulateVectors(i1, i2, i3); // 두 번째 삼각형

                    cellIndices.push_back(i0); cellIndices.push_back(i2); cellIndices.push_back(i1);
                    cellIndices.push_back(i1); cellIndices.push_back(i2); cellIndices.push_back(i3);
                }
            } // for - 인덱스 생성 및 Tangent/Binormal 누적 계산

            // 최종 정규화 (Normalize) - 정점 공유 시 평균값 계산
            for (auto& v : cellVertices)
            {
                DirectX::XMVECTOR t = DirectX::XMLoadFloat3(&v.tangent);
                DirectX::XMVECTOR b = DirectX::XMLoadFloat3(&v.binormal);
                DirectX::XMStoreFloat3(&v.tangent, DirectX::XMVector3Normalize(t));
                DirectX::XMStoreFloat3(&v.binormal, DirectX::XMVector3Normalize(b));
            }

            // 셀 초기화 및 저장
            auto cell = std::make_unique<TerrainModelCell>();
            if (cell->Init(device, cellVertices, cellIndices))
            {
                m_cells.push_back(std::move(cell));
            }
        }
    }

    return true;
} // CreateCells


//void TerrainModel::CalculateTangentBinormal(
//    const ModelVertex& v1, const ModelVertex& v2, const ModelVertex& v3,
//    DirectX::XMFLOAT3& tangent, DirectX::XMFLOAT3& binormal)
//{
//    float vector1[3], vector2[3];
//    float tuVector[2], tvVector[2];
//
//    // 두 변의 벡터 계산
//    vector1[0] = v2.position.x - v1.position.x;
//    vector1[1] = v2.position.y - v1.position.y;
//    vector1[2] = v2.position.z - v1.position.z;
//
//    vector2[0] = v3.position.x - v1.position.x;
//    vector2[1] = v3.position.y - v1.position.y;
//    vector2[2] = v3.position.z - v1.position.z;
//
//    // UV 좌표 차이 계산
//    tuVector[0] = v2.texture.x - v1.texture.x;
//    tvVector[0] = v2.texture.y - v1.texture.y;
//
//    tuVector[1] = v3.texture.x - v1.texture.x;
//    tvVector[1] = v3.texture.y - v1.texture.y;
//
//    // 접선/종법선 공식 적용
//    float den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);
//
//    tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
//    tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
//    tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;
//
//    binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
//    binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
//    binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;
//
//    // 정규화
//    DirectX::XMVECTOR t = DirectX::XMLoadFloat3(&tangent);
//    DirectX::XMVECTOR b = DirectX::XMLoadFloat3(&binormal);
//    DirectX::XMStoreFloat3(&tangent, DirectX::XMVector3Normalize(t));
//    DirectX::XMStoreFloat3(&binormal, DirectX::XMVector3Normalize(b));
//
//    return;
//} // CalculateTangentBinormal


void TerrainModel::SetHeightMap(std::unique_ptr<HeightMap> hMap)
{
    if (m_HeightMap == nullptr)
    {
        m_HeightMap = std::make_unique<HeightMap>();
    }
    m_HeightMap = std::move(hMap);
} // SetHeightMap