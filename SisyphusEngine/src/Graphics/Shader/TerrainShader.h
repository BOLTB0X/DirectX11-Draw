// Graphics/Shader/TerrainShader.h
#pragma once
#include <memory>
#include "ConstantBuffer.h"
// Framework
#include "Shader/Shader.h"
// Graphics
#include "Model/Texture/Material.h"


class TerrainShader : public Shader {
public:
    TerrainShader();
    TerrainShader(const TerrainShader& other) = delete;
    virtual ~TerrainShader() override = default;


    virtual bool Init(ID3D11Device* device, HWND hwnd, const std::wstring& vsPath, const std::wstring& psPath) override;
    virtual void Bind(ID3D11DeviceContext* context) override;

private:
    std::unique_ptr<ConstantBuffer<MaterialBuffer>> m_materialBuffer;
}; // TerrainShader