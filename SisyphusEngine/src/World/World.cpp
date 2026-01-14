// World/World.cpp
#include "World.h"
#include "Stone/Stone.h"
#include "Terrain/CloudOcean.h"
#include "MountPick/MountPick.h"
// Common
#include "EngineSettings.h"
#include "EngineHelper.h"
// Application
#include "Manager/ModelManager.h"
#include "Manager/ShaderManager.h"
#include "Manager/TexturesManager.h"
/// Framework
#include "Shader/Shader.h"
#include "Position/Position.h"
#include "Actor/ActorObject.h"
/// Graphics
#include "Camera/Camera.h"
#include "Shader/TerrainShader.h"
#include "Shader/ActorsShader.h"
// etc
#include <string>


/* default */
/////////////////////////////////////////////////////////////////

World::World()
    : m_Actors(),
    m_Camera(nullptr),
    m_DeviceContext(nullptr),
    actorsShaderPtr(nullptr),
    terrainShaderPtr(nullptr)
{
} // World


World::~World()
{
    Shutdown();
} // ~World


bool World::Init(const WorldInitParam& p)
{
    m_Camera = std::make_unique<Camera>();
    m_Camera->InitProjection(
        EngineSettings::SCREEN_WIDTH,
        EngineSettings::SCREEN_HEIGHT,
        EngineSettings::SCREEN_NEAR,
        EngineSettings::SCREEN_DEPTH
    );

    if (p.context == nullptr) return false;
    m_DeviceContext = p.context;

    TerrainModel* cloudModel = p.modelManager->GetTerrainModel(p.device, p.context, p.textureManager, EngineSettings::TERRAIN_PATH);
    terrainShaderPtr = p.shaderManager->GetShader<TerrainShader>("Terrain");
    float centerX = 400.0f;
    float centerZ = 400.0f;

    if (cloudModel && terrainShaderPtr)
    {
        m_CloudOcean = std::make_unique<CloudOcean>();
        m_CloudOcean->Init(cloudModel, terrainShaderPtr, "CloudOcean");
        m_CloudOcean->GetPosition()->SetPosition(0.0f, 0.0f, 0.0f);
    }

    actorsShaderPtr = p.shaderManager->GetShader<ActorsShader>("Actors");
    MeshModel* stoneModel = p.modelManager->GetMeshModel(p.device, m_DeviceContext, p.textureManager, EngineSettings::STONE_PATH);
    MeshModel* mountainModel = p.modelManager->GetMeshModel(p.device, m_DeviceContext, p.textureManager, EngineSettings::MOUNTAIN_PATH);

    if (mountainModel && actorsShaderPtr)
    {
        auto mount = std::make_unique<MountPick>();
        mount->Init(mountainModel, actorsShaderPtr, "MountPick");
        mount->GetPosition()->SetPosition(centerX, 0.0, centerZ);
        m_Actors.push_back(std::move(mount));
    }

    if (stoneModel && actorsShaderPtr)
    {
        auto stone = std::make_unique<Stone>();
        stone->Init(stoneModel, actorsShaderPtr, "Stone");
        stone->GetPosition()->SetPosition(centerX - 100.0f, 0.0f, centerZ - 100.0f);
        m_Actors.push_back(std::move(stone));
    }

    if (m_CloudOcean)
    {
        for (auto& actor : m_Actors)
        {
            DirectX::XMFLOAT3 actorPos = actor->GetPosition()->GetPosition();
            float worldHeight = m_CloudOcean->GetHeightAtWorld(actorPos.x, actorPos.z);

            if (actor->GetName() == "MountPick")
            {
                actor->SetHeightOffset(10.0f);
            }

            actor->PlaceOnTerrain(worldHeight, actor->GetHeightOffset());
        }

        m_Camera->GetPosition()->SetPosition(centerX - 100, 100.0f, centerZ - 150.0f);
        m_Camera->GetPosition()->SetRotation(30.0f, 0.0f, 0.0f);
    }

    return true;
} // Init

void World::Shutdown()
{
    if (m_CloudOcean)
    {
        m_CloudOcean->Shutdown();
        m_CloudOcean.reset();
    }

    for (auto& actor : m_Actors)
    {
        if (actor) actor->Shutdown();
    }
    m_Actors.clear();

    actorsShaderPtr = nullptr;
    terrainShaderPtr = nullptr;
    m_DeviceContext = nullptr;

    m_Camera = nullptr;
} // Shutdown


void World::Frame(float frameTime, bool canControlWorld)
{
    if (canControlWorld)
    {
        m_Camera->GetPosition()->SetFrameTime(frameTime);
        m_Camera->Render();
    }

    for (auto& actor : m_Actors)
    {
        actor->Frame(frameTime);
    }
} // Frame


void World::Render(ShaderManager* shaderManager)
{
    if (m_Camera == nullptr) return;
    m_Camera->Render();

    if (m_CloudOcean)
    {
        m_CloudOcean->Render(m_DeviceContext,m_Camera.get());
    }


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
