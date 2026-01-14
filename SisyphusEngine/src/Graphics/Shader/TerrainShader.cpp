// Graphics/Shader/TerrainShader.h
#include "TerrainShader.h"
// Common
#include "EngineHelper.h"


TerrainShader::TerrainShader() : Shader()
{
}

bool TerrainShader::Init(ID3D11Device* device, HWND hwnd, const std::wstring& vsPath, const std::wstring& psPath)
{
    ComPtr<ID3DBlob> vsBlob;
    ComPtr<ID3DBlob> psBlob;

    // 셰이더 컴파일
    if (EngineHelper::SuccessCheck(Compile(device, hwnd,
        vsPath, "main", "vs_5_0",
        vsBlob.GetAddressOf()), "버텍스 셰이더 컴파일 에러")
        == false) return false;
    if (EngineHelper::SuccessCheck(Compile(device, hwnd,
        psPath, "main", "ps_5_0",
        psBlob.GetAddressOf()), "픽셀 셰이더 컴파일 에러")
        == false) return false;

    // 셰이더 생성
    device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf());
    device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf());

    m_materialBuffer = std::make_unique<ConstantBuffer<MaterialBuffer>>();
    if (EngineHelper::SuccessCheck(m_materialBuffer->Init(device), "머테리얼버퍼 초기화 에러") == false)
    {
        return false;
    }

    // 터레인 Input Layout
    D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    device->CreateInputLayout(layoutDesc, 5, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), m_layout.GetAddressOf());

    // Matrix Buffer 초기화 (베이스 클래스 멤버)
    if (EngineHelper::SuccessCheck(m_matrixBuffer.Init(device), "상수 버퍼 초기화 에러")
        == false) return false;

    // 샘플러 상태 생성 (지형 텍스처 반복용)
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    return SUCCEEDED(device->CreateSamplerState(&samplerDesc, m_sampleState.GetAddressOf()));
} // Init


void TerrainShader::Bind(ID3D11DeviceContext* context)
{
    // 베이스 클래스의 레이아웃/VS/PS 바인딩 호출
    Shader::Bind(context);
    m_matrixBuffer.BindPS(context, 0);
    m_materialBuffer->BindPS(context, 2);
    context->PSSetSamplers(0, 1, m_sampleState.GetAddressOf());
} // Bind