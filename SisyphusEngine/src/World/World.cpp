// World/World.cpp
#include "World.h"
#include "Common/EngineSettings.h"

/* default */
/////////////////////////////////////////////////////////////////

World::World()
	: m_actors(),
	  m_mainCamera(std::make_unique<Camera>())
{
} // World

World::~World() {}


bool World::Init(ID3D11Device* device, ID3D11DeviceContext* context,
    ModelManager* modelManager, TexturesManager* texManager, int screenWidth, int screenHeight)
{
    m_mainCamera = std::make_unique<Camera>();
    m_mainCamera->InitProjection(screenWidth, screenHeight,
		EngineSettings::SCREEN_NEAR, EngineSettings::SCREEN_DEPTH);

    Model* stoneModel = modelManager->GetModel(device, context, texManager, EngineSettings::STONE_PATH);

    if (stoneModel)
    {
        auto stone1 = std::make_unique<Stone>();
        stone1->Init(stoneModel, "FirstStone");
        stone1->GetTransform()->SetPosition(0.0f, 0.0f, 5.0f); // 앞에 배치

        auto stone2 = std::make_unique<Stone>();
        stone2->Init(stoneModel, "SecondStone");
        stone2->GetTransform()->SetPosition(-2.0f, 0.0f, 10.0f); // 좀 더 멀리 왼쪽

        m_actors.push_back(std::move(stone1));
        m_actors.push_back(std::move(stone2));
    }

    return true;
} // Init


void World::Frame(float frameTime)
{
    for (auto& actor : m_actors)
    {
        actor->Frame(frameTime);
    }
} // Frame


void World::Render(ID3D11DeviceContext* context, Shader* shader)
{
  
    XMMATRIX view = m_mainCamera->GetViewMatrix();
    XMMATRIX projectiom = m_mainCamera->GetProjectionMatrix();

    for (auto& actor : m_actors)
    {
        actor->Render(context, shader, view, projectiom);
    }
} // Render