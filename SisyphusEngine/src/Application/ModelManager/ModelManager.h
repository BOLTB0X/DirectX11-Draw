// Application/ModelManager/ModelManager.h
#pragma once
#include "d3d11.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class MeshModel;
class TerrainModel;
class TexturesManager;
class Mesh;
class Texture;

class ModelManager {
public:
    ModelManager();
	ModelManager(const ModelManager&) = delete;
    ~ModelManager();

    bool Init(ID3D11Device*, ID3D11DeviceContext*, TexturesManager*);
    void Shutdown();

public:
    MeshModel* GetMeshModel(ID3D11Device*,
        ID3D11DeviceContext*,
        TexturesManager*,
        const std::string&);

    TerrainModel* ModelManager::GetTerrainModel(
        ID3D11Device* device,
        ID3D11DeviceContext*,
        TexturesManager*,
        const std::string&);

private:
    void ProcessNode(aiNode*, const aiScene*, ID3D11Device*, MeshModel*);

    std::unique_ptr<Mesh> ProcessMesh(
        aiMesh*,
        const aiScene*,
        ID3D11Device*);

    void ProcessMaterials(
        const aiScene*,
        ID3D11Device*,
        ID3D11DeviceContext*,
        TexturesManager*,
        const std::string&, MeshModel*);

    void ProcessMaterialsForTerrain(
        const aiScene*,
        ID3D11Device*,
        ID3D11DeviceContext*,
        TexturesManager*,
        const std::string&,
        TerrainModel*);

    void ExtractAllMeshData(
        aiNode*,
        const aiScene*,
        TerrainModel*);

    std::shared_ptr<Texture> LoadPBRTexture(
        ID3D11Device*,
        ID3D11DeviceContext*,
        TexturesManager*,
        const std::string&,
        const std::string&,
        const std::string&
    );

private:
    std::unordered_map<std::string, std::unique_ptr<MeshModel>> m_modelLibrary;
    std::unordered_map<std::string, std::unique_ptr<TerrainModel>> m_terrainLibrary;
}; // ModelManager