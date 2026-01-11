// Graphics/Shader/ActorsShader.h
#pragma once
#include "Shader.h"
#include "ConstantBuffer.h"
#include "Model/Texture/Material.h"
#include <memory>

class ActorsShader : public Shader {
public:
    ActorsShader();
    virtual ~ActorsShader();

    bool Init(ID3D11Device*, HWND, const std::wstring&, const std::wstring&) override;
    void Bind(ID3D11DeviceContext* context) override;

public:
    void UpdateMaterialTag(ID3D11DeviceContext* context, int type);

private:
    std::unique_ptr<ConstantBuffer<MaterialBuffer>> m_materialBuffer;
}; // ActorsShader