// World/Terrain/Ground.h
#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include <memory>
#include <string>

class TerrainModel;
class Position;
class Shader;
class Frustum;
class HeightMap;

class Ground {
public:
    Ground();
    Ground(const Ground&) = delete;
    ~Ground();

    bool Init(TerrainModel*, const std::string&);
    void Shutdown();
    void Frame(float);
    void Render(ID3D11DeviceContext* context,
        Shader* shader,
        Frustum* frustum,
        const DirectX::XMMATRIX& view,
        const DirectX::XMMATRIX& projection);

public:
    float GetHeight(float x, float z) const;
    HeightMap* GetHeightMap() const { return m_HeightMap.get(); }

private:
    std::string m_name;
    TerrainModel* m_TerrainModel;
    std::unique_ptr<HeightMap> m_HeightMap;
    std::unique_ptr<Position> m_Position;
}; // Ground
