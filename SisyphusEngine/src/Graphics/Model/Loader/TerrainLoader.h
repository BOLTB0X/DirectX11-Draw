// Graphics/Model/Loader/TerrainLoader.h
#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include <string>
#include <vector>
// Graphics
#include "Model/Mesh/VertexTypes.h"

class TerrainModel;
class TexturesManager;

class TerrainLoader {
public:
    TerrainLoader();
    TerrainLoader(const TerrainLoader& other) = delete;
    ~TerrainLoader();

    bool LoadTerrainFromSetup(ID3D11Device*, ID3D11DeviceContext*, TexturesManager*, const std::string&, TerrainModel*);

public:
    struct TerrainInfo {
        std::string heightMapFilename;
        int terrainHeight;
        int terrainWidth;
        float terrainScaling;
        std::string colorMapFilename;
    };

private:
    bool LoadSetupFile(const std::string& path, TerrainInfo& info);

    bool LoadRawHeightMap(const TerrainInfo& info, std::vector<DirectX::XMFLOAT3>& outPositions);
    bool CalculateNormals(const TerrainInfo& info, std::vector<ModelVertex>& vertices);
    void CalculateTangentBinormal(ModelVertex v1, ModelVertex v2, ModelVertex v3,
        DirectX::XMFLOAT3& tangent, DirectX::XMFLOAT3& binormal);

    void CreateTerrainGeometry(const TerrainInfo& info,
        const std::vector<DirectX::XMFLOAT3>& positions,
        std::vector<ModelVertex>& outVertices,
        std::vector<unsigned int>& outIndices);
}; // TerrainLoader