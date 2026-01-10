// Graphics/Model/MeshModel.h
#pragma once

#include <d3d11.h>
#include <memory>
#include <vector>
#include "Model/Mesh/VertexTypes.h"
#include "Model/Texture/Material.h"

class Mesh;


class MeshModel {
public:
    MeshModel();
	MeshModel(const MeshModel&) = delete;
	~MeshModel();

    void Render(ID3D11DeviceContext*);

public:
    void AddMesh(std::unique_ptr<Mesh>);
    void AddMaterial(const Material&);

public:
    std::vector<MeshData> GetFullMeshData() const;

private:
    std::vector<std::unique_ptr<Mesh>> m_meshes;
    std::vector<Material> m_materials;
}; // MeshModel