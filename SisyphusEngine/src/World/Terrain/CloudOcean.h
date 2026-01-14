// World/Terrain/CloudTerrain.h
#pragma once
#include <d3d11.h>
#include <memory>
#include <string>

class TerrainModel;
class TerrainShader;
class Camera;
class Position;

class CloudOcean {
public:
    CloudOcean();
    CloudOcean(const CloudOcean& other) = delete;
    ~CloudOcean();

    bool Init(TerrainModel* model, TerrainShader* shader, const std::string& name);

    void Render(ID3D11DeviceContext* context, Camera* camera);
    void Shutdown();

public:
    float GetHeight(float x, float z) const;
    Position* GetPosition() const { return m_Position.get(); }
    float GetHeightAtWorld(float worldX, float worldZ) const;

private:
    std::string m_name;
    std::unique_ptr<Position> m_Position;
    TerrainModel* m_modelPtr;   // ModelManager로부터 받은 포인터
    TerrainShader* m_shaderPtr; // ShaderManager로부터 받은 포인터
}; // CloudOcean
