// Graphics/Shader/GroundShader.h
#pragma once

#include "Shader/Shader.h"

class GroundShader : public Shader
{
public:
    GroundShader();
    GroundShader(const GroundShader& other) = delete;
    virtual ~GroundShader();

    bool Init(ID3D11Device* device, HWND hwnd,
        const std::wstring& vsPath, const std::wstring& psPath);

    void Bind(ID3D11DeviceContext* context) override;
}; // CloudTerrain