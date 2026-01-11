// World/World.cpp
#include "World.h"
#include "Position.h"
//
#include "EngineSettings.h"
#include "EngineHelper.h"
//
#include "Stone/Stone.h"
#include "Terrain/Mountain.h"
// Application
#include "ModelManager/ModelManager.h"
/// Framework
#include "Actor/ActorRenderParams.h"
#include "Actor/ActorObject.h"
/// Graphics
#include "Camera/Camera.h"

/* default */
/////////////////////////////////////////////////////////////////

World::World()
	: m_Actors(),
    m_Camera(nullptr)
{
    m_Mountain = std::make_unique<Mountain>();
} // World

World::~World()
{
    Shutdown();
}


bool World::Init(ID3D11Device* device, ID3D11DeviceContext* context,
    ModelManager* modelManager, TexturesManager* texManager, Camera* camera)
{

    MeshModel* stoneModel = modelManager->GetMeshModel(device, context, texManager, EngineSettings::STONE_PATH);
    MeshModel* testModel = modelManager->GetMeshModel(device, context, texManager, EngineSettings::MOUNTAIN_PATH);
    m_Camera = camera;

    if (m_Camera)
    {
        m_Camera->GetPosition()->SetPosition(0.0f, 50.0f, -100.0f);
        m_Camera->GetPosition()->SetRotation(25.0f, 0.0f, 0.0f);
        m_Camera->Render();
    }

    if (stoneModel)
    {
        auto stone1 = std::make_unique<Stone>();
        stone1->Init(stoneModel, "FirstStone");
        stone1->GetPosition()->SetPosition(0.0f, 0.0f, 5.0f);

        auto stone2 = std::make_unique<Stone>();
        stone2->Init(stoneModel, "SecondStone");
        stone2->GetPosition()->SetPosition(-2.0f, 0.0f, 10.0f);


        m_Actors.push_back(std::move(stone1));
        m_Actors.push_back(std::move(stone2));
    }

    if (testModel) {
        auto stone3 = std::make_unique<Stone>();
        stone3->Init(testModel, "test");
        stone3->GetPosition()->SetPosition(0.0f, 0.0f, 10.0f);
        m_Actors.push_back(std::move(stone3));
    }

    //TerrainModel* mountain = modelManager->GetTerrainModel(device, context, texManager, EngineSettings::MOUNTAIN_PATH);
    //if (mountain) {
    //    m_Mountain->Init(mountain, "TERRAIN");
    //}
    //else {
    //    EngineHelper::DebugPrint("World: modelManager->GetTerrainModel(..) 이게 문제");
    //}
 
    for (auto& actor : m_Actors)
    {
        DirectX::XMFLOAT3 pos = actor->GetPosition()->GetPosition();
        float height = m_Mountain->GetHeight(pos.x, pos.z);
        actor->GetPosition()->SetPosition(pos.x, height, pos.z);
    }

    return true;
} // Init

void World::Shutdown()
{
    if (m_Mountain) m_Mountain->Shutdown();

    for (auto& actor : m_Actors)
        actor->Shutdown();
    
    m_Camera = nullptr;
    m_Actors.clear();
    return;
} // Shutdown


void World::Frame(float frameTime, bool canControlWorld)
{
    if (canControlWorld)
    {
        //m_Camera->Render();
    }

    for (auto& actor : m_Actors)
    {
        actor->Frame(frameTime);
    }
} // Frame


void World::Render(const ActorRenderParams& params)
{
    m_Camera->Render();

    //if (m_Mountain)
    //{
    //    // Mountain의 Render 인자에 맞게 전달 (frustum 포함)
    //    m_Mountain->Render(
    //        params.context,
    //        params.shader,
    //        m_Camera->GetFrustum(),
    //        m_Camera->GetViewMatrix(),
    //        m_Camera->GetProjectionMatrix()
    //    );
    //}

    for (auto& actor : m_Actors)
    {
        actor->Render(params);
    }
} // Render

/////////////////////////////////////////////////////////////////


/* get */
/////////////////////////////////////////////////////////////////

const std::vector<std::unique_ptr<ActorObject>>& World::GetActors() const
{ 
    return m_Actors; 
} // GetActors


ActorObject* World::GetActor(size_t index) const
{
    if (index < m_Actors.size()) 
        return m_Actors[index].get();
    return nullptr;
} // GetActor