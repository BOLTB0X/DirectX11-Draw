// World/Stone/Stone.h
#pragma once
#include "Framework/Actor/ActorObject.h"
#include "Actor/ActorRenderParams.h"

class MeshModel;

class Stone : public ActorObject
{
public:
    Stone();
	Stone(const Stone&) = delete;
    virtual ~Stone();

    virtual bool Init(MeshModel*, const std::string&) override;
    virtual void Shutdown() override;
    virtual void Frame(float) override;
    virtual void Render(const ActorRenderParams&) override;

private:
}; // Stone