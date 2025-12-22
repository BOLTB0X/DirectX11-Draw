#pragma once
#include <d3d11.h>

class Shader
{
public:
    bool Init(ID3D11Device* device);
    void Bind(ID3D11DeviceContext* context);

private:
    ID3D11VertexShader* mVS = nullptr;
    ID3D11PixelShader* mPS = nullptr;
    ID3D11InputLayout* mLayout = nullptr;
};
