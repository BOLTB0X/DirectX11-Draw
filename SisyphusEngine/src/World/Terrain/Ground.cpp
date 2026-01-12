// World/Terrain/Ground.cpp
#include "Ground.h"
// 
#include "Position/Position.h"
// Graphics
#include "Model/TerrainModel.h"
#include "Model/HeightMap/HeightMap.h"
#include "Shader/Shader.h"
#include "Camera/Frustum.h"
// Common
#include "EngineHelper.h"

/* defulat */
///////////////////////////////////////////////////////////////////////

Ground::Ground()
	: m_name(""),
	m_TerrainModel(nullptr)
{
    m_HeightMap = std::make_unique<HeightMap>();
    m_Position = std::make_unique<Position>();
}


Ground::~Ground()
{
    Shutdown();
} // ~Ground

bool Ground::Init(TerrainModel* model, const std::string& name)
{
    if (model == nullptr) return false;

    m_TerrainModel = model;
    if (m_TerrainModel == nullptr) {
        EngineHelper::DebugPrint("m_TerrainModel 널");
    }
    m_name = name;

    if (EngineHelper::SuccessCheck(
        m_HeightMap->Init(m_TerrainModel->GetFullVertices(), m_TerrainModel->GetFullIndices()),
        "Mountain HeightMap 초기화 실패")
        == false) return false;

    m_Position->SetPosition(0.0f, 0.0f, 0.0f);

    return true;
} // Init


void Ground::Shutdown()
{
    if (m_HeightMap) m_HeightMap.reset();
    if (m_Position) m_Position.reset();

    m_TerrainModel = nullptr;
} // Shutdown


void Ground::Frame(float frameTime)
{
 
} // Frame


void Ground::Render(ID3D11DeviceContext* context,
    Shader* shader,
    Frustum* frustum,
    const DirectX::XMMATRIX& view,
    const DirectX::XMMATRIX& projection)
{
    EngineHelper::DebugPrint("Mountain: Render 시작");
    if (m_TerrainModel == nullptr || shader == nullptr) return;

    DirectX::XMMATRIX world = m_Position->GetWorldMatrix();

    if (shader->UpdateMatrixBuffer(context, m_Position->GetWorldMatrix(), view, projection)
        == false)
    {

        return;
    }
    shader->Bind(context);

    m_TerrainModel->Render(context, frustum);
} // Render

///////////////////////////////////////////////////////////////////////


/* publc */
///////////////////////////////////////////////////////////////////////

float Ground::GetHeight(float x, float z) const
{
    if (m_HeightMap == nullptr) return 0.0f;
    return m_HeightMap->GetHeightAt(x, z);
} // GetHeight

