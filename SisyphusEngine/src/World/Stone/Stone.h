// World/Stone/Stone.h
#pragma once
#include "Framework/Actor/ActorObject.h"
#include "Actor/ActorRenderParams.h"

class MeshModel;
class ActorsShader;
class Camera;

class Stone : public ActorObject
{
public:
    Stone();
	Stone(const Stone&) = delete;
    virtual ~Stone();

    virtual bool Init(MeshModel*, ActorsShader*, const std::string&) override;
    virtual void Shutdown() override;
    virtual void Frame(float) override;
    virtual void Render(ID3D11DeviceContext*, Camera*) override;

private:
}; // Stone