// World/Terrain/CloudTerrain.cpp
#include "CloudOcean.h"
// Graphics
#include "Shader/TerrainShader.h"
#include "Model/TerrainModel.h"
#include "Model/HeightMap/HeightMap.h"
#include "Camera/Camera.h"
// Common
#include "EngineHelper.h"
// Framework
#include "Position/Position.h"

/* default */
////////////////////////////////////////////////////////////////////


CloudOcean::CloudOcean()
    : m_modelPtr(nullptr),
    m_shaderPtr(nullptr),
    m_name("")
{
} // CloudOcean


CloudOcean::~CloudOcean()
{
    Shutdown();
} // ~CloudOcean


bool CloudOcean::Init(TerrainModel* model, TerrainShader* shader, const std::string& name)
{
    if (model == nullptr || shader == nullptr) return false;

    m_modelPtr = model;
    m_shaderPtr = shader;
    m_name = name;

    m_Position = std::make_unique<Position>();
    if (m_Position)
    {
        m_Position->SetPosition(0.0f, 0.0f, 0.0f);
        m_Position->SetRotation(0.0f, 0.0f, 0.0f);
        m_Position->SetScale(1.0f);
    }

    EngineHelper::DebugPrint("CloudOcean: " + m_name + " 초기화 성공");
    return true;
} // Init


void CloudOcean::Render(ID3D11DeviceContext* context, Camera* camera)
{
    if (m_modelPtr == nullptr || m_shaderPtr == nullptr || camera == nullptr) return;

    DirectX::XMMATRIX worldMatrix = m_Position->GetWorldMatrix();
    m_shaderPtr->UpdateMatrixBuffer(
        context,
        worldMatrix,
        camera->GetViewMatrix(),
        camera->GetProjectionMatrix()
    );

    m_shaderPtr->Bind(context);

    auto frustum = camera->GetFrustum();
    m_modelPtr->Render(context, m_shaderPtr, frustum, worldMatrix);
} // Render


void CloudOcean::Shutdown()
{
    m_modelPtr = nullptr;
    m_shaderPtr = nullptr;

    if (m_Position)
    {
        m_Position.reset();
    }
} // Shutdown

////////////////////////////////////////////////////////////////////


float CloudOcean::GetHeight(float x, float z) const
{
    if (m_modelPtr && m_modelPtr->GetHeightMap())
    {
        return m_modelPtr->GetHeightMap()->GetHeightAtMesh(x, z);
    }
    return 0.0f;
} // GetHeight


float CloudOcean::GetHeightAtWorld(float worldX, float worldZ) const
{
    if (m_modelPtr == nullptr || m_modelPtr->GetHeightMap() == nullptr) return 0.0f;

    // 1. 월드 좌표 -> 로컬 좌표 변환
    DirectX::XMFLOAT3 myPos = m_Position->GetPosition();
    float localX = worldX - myPos.x;
    float localZ = worldZ - myPos.z;

    // 2. [중요] TerrainLoader의 Z축 역전 보정 (필요한 경우)
    // Init 시점에 z를 j(0~height)로 넣으셨으므로, 로더와 맞추기 위해 보정합니다.
    // 만약 로더에서 (Height-1)-j 로 넣었다면 아래 코드가 필요합니다.
    int terrainH = 512; // 실제 지형 높이값 사용 (정보가 있다면 가져오기)
    float correctedZ = (float)(terrainH - 1) - localZ;

    // 3. 로컬 높이값 추출
    float localHeight = m_modelPtr->GetHeightMap()->GetHeightAtGrid(localX, localZ);

    // 4. 로컬 높이 -> 월드 높이 변환 (지형 자체의 Y축 위치와 Scale 반영)
    return (localHeight * m_Position->GetScale().x) + myPos.y;
} // GetHeightAtWorld