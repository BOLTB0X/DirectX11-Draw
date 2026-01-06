// Application/ModelManager/ModelManager.h
#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Graphics/Model/Model.h"
#include "TexturesManager/TexturesManager.h"

class ModelManager {
public:
    ModelManager();
	ModelManager(const ModelManager&) = delete;
    ~ModelManager();

    bool Init(ID3D11Device*, ID3D11DeviceContext*, TexturesManager*);
    void Shutdown();

public:
    Model* GetModel(ID3D11Device*, ID3D11DeviceContext*,
        TexturesManager*, const std::string&);

private:
    // 내부 로직 분리
    void ProcessNode(aiNode*, const aiScene*, ID3D11Device*, Model*);
    std::unique_ptr<Mesh> ProcessMesh(aiMesh*, const aiScene*, ID3D11Device*);
    void ProcessMaterials(const aiScene*, ID3D11Device*,
        ID3D11DeviceContext*, TexturesManager*,
        const std::string&, Model*);

private:
    std::unordered_map<std::string, std::unique_ptr<Model>> m_modelLibrary;
}; // ModelManager