// Framework/ActorObject.h
#pragma once
#include <d3d11.h>
#include <memory>
#include <string>

class Position;
class MeshModel;
class ActorsShader;
class Camera;


class ActorObject {
public:
    ActorObject();
    ActorObject(const ActorObject&) = delete;
    virtual ~ActorObject();

    virtual bool Init(MeshModel*, ActorsShader*, const std::string&);
    virtual void Shutdown();
    virtual void Frame(float);
    virtual void Render(ID3D11DeviceContext*, Camera*);

    void PlaceOnTerrain(float terrainWorldHeight, float footingBias);

public:
    Position* GetPosition() const { return actor_Position.get(); }
    const std::string& GetName() const { return actor_name; }
    void SetHeightOffset(float offset) { m_heightOffset = offset; }
    float GetHeightOffset() const { return m_heightOffset; }

protected:
    std::string actor_name;
    ActorsShader* actor_Shader;
    MeshModel* actor_Model;
    std::unique_ptr<Position> actor_Position;
    float m_heightOffset = 0.0f;
}; // ActorObject