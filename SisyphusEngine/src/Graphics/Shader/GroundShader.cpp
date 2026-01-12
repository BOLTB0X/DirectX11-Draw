// Graphics/Shader/GroundShader.cpp
#include "GroundShader.h"
#include "EngineHelper.h"

using namespace EngineHelper;


/* defuatl */
///////////////////////////////////////////////////////////////////////////////

GroundShader::GroundShader() : Shader() {}


GroundShader::~GroundShader() {}


bool GroundShader::Init(ID3D11Device* device, HWND hwnd,
    const std::wstring& vsPath, const std::wstring& psPath)
{
    ComPtr<ID3DBlob> vsBlob;
    ComPtr<ID3DBlob> psBlob;

    if (SuccessCheck(Compile(device, hwnd, vsPath, "VS_Main", "vs_5_0", vsBlob.GetAddressOf()),
        "Ground VS 컴파일 에러") == false) return false;

    if (SuccessCheck(Compile(device, hwnd, psPath, "PS_Main", "ps_5_0", psBlob.GetAddressOf()),
        "Ground PS 컴파일 에러") == false) return false;

    device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf());
    device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf());

    D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    device->CreateInputLayout(layoutDesc, 5, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), m_layout.GetAddressOf());

 
    if (SuccessCheck(m_matrixBuffer.Init(device), "Ground MatrixBuffer 초기화 에러") == false) return false;

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; // 지형 텍스처 반복 허용
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    return SUCCEEDED(device->CreateSamplerState(&samplerDesc, m_sampleState.GetAddressOf()));
} // Init


void GroundShader::Bind(ID3D11DeviceContext* context)
{
    Shader::Bind(context);
    m_matrixBuffer.BindVS(context, 0);
    context->PSSetSamplers(0, 1, m_sampleState.GetAddressOf());
} // Bind