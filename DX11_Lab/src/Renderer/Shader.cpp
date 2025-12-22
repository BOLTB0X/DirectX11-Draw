#include "Shader.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

bool Shader::Init(ID3D11Device* device)
{
    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;

    D3DCompileFromFile(
        L"shaders/Basic.hlsl",
        nullptr, nullptr,
        "VSMain", "vs_5_0",
        0, 0, &vsBlob, nullptr
    );

    D3DCompileFromFile(
        L"shaders/Basic.hlsl",
        nullptr, nullptr,
        "PSMain", "ps_5_0",
        0, 0, &psBlob, nullptr
    );

    device->CreateVertexShader(
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        nullptr, &mVS
    );

    device->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr, &mPS
    );

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
          D3D11_INPUT_PER_VERTEX_DATA, 0 },

        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
          D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    device->CreateInputLayout(
        layout, 2,
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        &mLayout
    );

    vsBlob->Release();
    psBlob->Release();

    return true;
} // Init

void Shader::Bind(ID3D11DeviceContext* context)
{
    context->IASetInputLayout(mLayout);
    context->VSSetShader(mVS, nullptr, 0);
    context->PSSetShader(mPS, nullptr, 0);
} // Bind
