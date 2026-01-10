// Graphics/Model/Mesh.cpp
#include "Mesh.h"
#include "Common/EngineHelper.h"
#include "Model/Texture/Material.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

/* defulat */
///////////////////////////////////////////////////////////////////

Mesh::Mesh()
    : m_materialIndex(0)
{
    m_meshData = { {}, {} };
}

Mesh::~Mesh() {}


bool Mesh::Init(ID3D11Device* device, const MeshData& data, unsigned int matIndex)
{
    m_meshData = data;
    m_materialIndex = matIndex;

    vertexBuffer = std::make_unique<VertexBuffer>();
    if (EngineHelper::SuccessCheck(
        vertexBuffer->Init(device, data.vertices), "버텍스버퍼 초기화 실패")
        == false) return false;

    indexBuffer = std::make_unique<IndexBuffer>();
    if (EngineHelper::SuccessCheck(
        indexBuffer->Init(device, data.indices), "인덱스버퍼 초기화 실패")
        == false) return false;

    return true;
} // Init


void Mesh::Bind(ID3D11DeviceContext* context)
{
    if (!vertexBuffer || !indexBuffer) return;
    vertexBuffer->Bind(context);
    indexBuffer->Bind(context);
} // Render

///////////////////////////////////////////////////////////////////

/* public */
///////////////////////////////////////////////////////////////////


unsigned int Mesh::GetMaterialIndex() const
{
    return m_materialIndex;
} // GetMaterialIndex


unsigned int Mesh::GetIndexCount() const
{
    return indexBuffer->GetIndexCount();
} // GetIndexCount


const MeshData& Mesh::GetMeshData() const
{
    return m_meshData;
} // GetMeshData
