// World/World.h
#pragma once
#include <vector>
#include <memory>
#include "Stone.h"

#include "Application/ModelManager/ModelManager.h"
#include "Graphics/Camera/Camera.h"

class World {
public:
    World();
	World(const World&) = delete;
    ~World();

    bool Init(ID3D11Device* device, ID3D11DeviceContext* context,
        ModelManager* modelManager, TexturesManager* texManager, int screenWidth, int screenHeight);

    void Frame(float frameTime);
    void Render(ID3D11DeviceContext* context, Shader* shader);

private:
    std::vector<std::unique_ptr<ActorObject>> m_actors;
    std::unique_ptr<Camera> m_mainCamera;
}; // World