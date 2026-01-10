// World/World.h
#pragma once
#include <d3d11.h>
#include <vector>
#include <memory>
#include "Actor/ActorRenderParams.h"

class ModelManager;
class TexturesManager;
class ActorObject;
class Mountain;
class Camera;


class World {
public:
    World();
	World(const World&) = delete;
    ~World();

    bool Init(ID3D11Device*, ID3D11DeviceContext*, ModelManager*, TexturesManager*, Camera*);
    void Shutdown();
    void Frame(float frameTime, bool);
    void Render(const ActorRenderParams& params);

public:
    ActorObject* GetActor(size_t index) const;
    const std::vector<std::unique_ptr<ActorObject>>& GetActors() const;

private:
    std::vector<std::unique_ptr<ActorObject>> m_Actors;
    std::unique_ptr<Mountain> m_Mountain;
    Camera* m_Camera;
}; // World