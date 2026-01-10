// Graphics/Model/Mesh.h
#pragma once

#include <memory>
#include <d3d11.h>
#include "VertexTypes.h"

class VertexBuffer;
class IndexBuffer;

class Mesh {
public:

	Mesh();
	Mesh(const Mesh&) = delete;
	~Mesh();

    bool Init(ID3D11Device*, const MeshData&, unsigned int);
    void Bind(ID3D11DeviceContext* context);

public:
    unsigned int GetMaterialIndex() const;
    unsigned int GetIndexCount() const;
    const MeshData& GetMeshData() const;

private:
    std::unique_ptr<VertexBuffer> vertexBuffer;
    std::unique_ptr<IndexBuffer> indexBuffer;
    unsigned int m_materialIndex;
    MeshData m_meshData;
}; // Mesh