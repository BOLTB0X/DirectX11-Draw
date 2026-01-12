// World/World.cpp
#include <string>

#include "World.h"
#include "Stone/Stone.h"
#include "Terrain/Ground.h"
// Common
#include "EngineSettings.h"
#include "EngineHelper.h"
// Application
#include "ModelManager/ModelManager.h"
#include "ShaderManager/ShaderManager.h"
#include "TexturesManager/TexturesManager.h"
/// Framework
#include "Position/Position.h"
#include "Actor/ActorRenderParams.h"
#include "Actor/ActorObject.h"
/// Graphics
#include "Camera/Camera.h"
#include "Shader/GroundShader.h"
#include "Shader/ActorsShader.h"


/* default */
/////////////////////////////////////////////////////////////////

World::World()
	: m_Actors(),
    m_Camera(nullptr),
    m_DeviceContext(nullptr)
{
} // World

World::~World()
{
    Shutdown();
}


bool World::Init(const WorldInitParam& p)
{
    m_Camera = std::make_unique<Camera>();
    m_Camera->InitProjection(
        EngineSettings::SCREEN_WIDTH,
        EngineSettings::SCREEN_HEIGHT,
        EngineSettings::SCREEN_NEAR,
        EngineSettings::SCREEN_DEPTH
    );

    m_Camera->GetPosition()->SetPosition(0.0f, 50.0f, -100.0f);
    m_Camera->GetPosition()->SetRotation(25.0f, 0.0f, 0.0f);

    if (p.context == nullptr) return false;
    m_DeviceContext = p.context;

    MeshModel* stoneModel = p.modelManager->GetMeshModel(p.device, m_DeviceContext, p.textureManager, EngineSettings::STONE_PATH);
    MeshModel* mountainModel = p.modelManager->GetMeshModel(p.device, m_DeviceContext, p.textureManager, EngineSettings::MOUNTAIN_PATH);
    ActorsShader* actorShader = p.shaderManager->GetShader<ActorsShader>("Actors");

    if (stoneModel && actorShader)
    {
        auto stone = std::make_unique<Stone>();
        stone->Init(stoneModel, actorShader, "FirstStone");
        stone->GetPosition()->SetPosition(50.0f, 0.0f, 5.0f);
        m_Actors.push_back(std::move(stone));
    }

    if (mountainModel && actorShader) {
        auto stone3 = std::make_unique<Stone>();
        stone3->Init(mountainModel, actorShader, "test");
        stone3->GetPosition()->SetPosition(0.0f, 0.0f, 10.0f);
        m_Actors.push_back(std::move(stone3));
    }

 
    for (auto& actor : m_Actors)
    {
        DirectX::XMFLOAT3 pos = actor->GetPosition()->GetPosition();
        //float height = m_Ground->GetHeight(pos.x, pos.z);
        //actor->GetPosition()->SetPosition(pos.x, height, pos.z);
    }

    return true;
} // Init

void World::Shutdown()
{
    //if (m_Ground) m_Ground->Shutdown();

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
        m_Camera->Render();
    }

    for (auto& actor : m_Actors)
    {
        actor->Frame(frameTime);
    }
} // Frame


void World::Render()
{
    if (m_Camera == nullptr) return;
    m_Camera->Render();


    for (auto& actor : m_Actors)
    {
        actor->Render(m_DeviceContext, m_Camera.get());
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


Camera* World::GetCamera() const
{
    return m_Camera.get();
} // GetCamera


Position* World::GetCameraPosition() const
{
    return m_Camera->GetPosition();
} // GetCameraPosition

/////////////////////////////////////////////////////////////////

/* public */
