// Framework/ActorObject.h
#pragma once
#include <memory>
#include <string>
#include "Actor/ActorRenderParams.h"

class Position;
class MeshModel;
class Shader;
class Frustum;


class ActorObject {
public:
    ActorObject();
    ActorObject(const ActorObject&) = delete;
    virtual ~ActorObject();

    virtual bool Init(MeshModel*, const std::string&);
    virtual void Shutdown();
    virtual void Frame(float);
    virtual void Render(const ActorRenderParams&);

public:
    Position* GetPosition() const { return actor_Position.get(); }
    const std::string& GetName() const { return actor_name; }

protected:
    std::string actor_name;
    MeshModel* actor_Model;
    std::unique_ptr<Position> actor_Position;
}; // ActorObject