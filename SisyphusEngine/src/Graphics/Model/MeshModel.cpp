// Graphics/Model/MeshModel.cpp
#include "MeshModel.h"
#include "Mesh/Mesh.h"
#include "Texture/Material.h"
#include "Shader/ActorsShader.h"
/* default */
////////////////////////////////////////////////////////////////////////////

MeshModel::MeshModel() {};

MeshModel::~MeshModel() {};


void MeshModel::Render(ID3D11DeviceContext* context, ActorsShader* shader)
{
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    for (const auto& mesh : m_meshes)
    {
        // 해당 메쉬의 머터리얼 인덱스 확인
        unsigned int matIdx = mesh->GetMaterialIndex();

        if (matIdx < m_materials.size())
        {
			const Material& material = m_materials[matIdx];
            //if (material.diffuse) material.diffuse->Bind(context, 0);
            //if (material.ambient) material.ambient->Bind(context, 1);
            //if (material.specular) material.specular->Bind(context, 2);

            MaterialBuffer data;
            data.type = (int)material.type;
            shader->UpdateMaterialTag(context, (int)material.type);
            if (m_materialBuffer)
            {
                m_materialBuffer->Update(context, data);
                m_materialBuffer->BindPS(context, 2);
            }

            if (material.albedo) material.albedo->Bind(context, 0);
            if (material.normal)    material.normal->Bind(context, 1);
            if (material.metallic)  material.metallic->Bind(context, 2);
            if (material.roughness) material.roughness->Bind(context, 3);
            if (material.ao)        material.ao->Bind(context, 4);

        }

        // 메쉬 그리기
        mesh->Bind(context);
        context->DrawIndexed(mesh->GetIndexCount(), 0, 0);
    }
} // Render

void MeshModel::AddMesh(std::unique_ptr<Mesh> mesh)
{
    m_meshes.push_back(std::move(mesh));
} // AddMesh


void MeshModel::AddMaterial(const Material& material)
{
    m_materials.push_back(material);
} // AddMaterial


bool MeshModel::InitConstantBuffer(ID3D11Device* device)
{
    m_materialBuffer = std::make_unique<ConstantBuffer<MaterialBuffer>>();
    return m_materialBuffer->Init(device);
} // InitConstantBuffer


std::vector<MeshData> MeshModel::GetFullMeshData() const
{
    std::vector<MeshData> allData;
    for (const auto& mesh : m_meshes)
        allData.push_back(mesh->GetMeshData());
    
    return allData;
} // GetFullMeshData