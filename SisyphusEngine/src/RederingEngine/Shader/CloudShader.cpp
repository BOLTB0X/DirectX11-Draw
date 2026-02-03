#include "Pch.h"
#include "CloudShader.h"
// Common
#include "ConstantHelper.h"
#include "DebugHelper.h"

using namespace DirectX;


CloudShader::CloudShader()
	: Shader()
{
    m_type = ShaderType::Cloud;
} // CloudShader


bool CloudShader::Init(ID3D11Device* device, HWND hwnd,
    const std::wstring& vsPath, const std::wstring& psPath)
{
    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;

    // 셰이더 컴파일
    if (DebugHelper::SuccessCheck(Compile(device, hwnd, vsPath, "main", "vs_5_0", &vsBlob), 
        "CloudShader::Init, vs 컴파일 에러") == false) return false;
    if (DebugHelper::SuccessCheck(Compile(device, hwnd, psPath, "main", "ps_5_0", &psBlob),
        "CloudShader::Init, ps 컴파일 에러") == false) return false;

    // 셰이더 생성
    device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_vertexShader);
    device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_pixelShader);

    // 입력 레이아웃 생성
    D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    device->CreateInputLayout(layoutDesc, sizeof(layoutDesc) / sizeof(layoutDesc[0]), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_layout);

    vsBlob->Release();
    psBlob->Release();

    return InitBuffers(device);
} // Init


bool CloudShader::InitBuffers(ID3D11Device* device)
{
    // 상수 버퍼 생성 (매트릭스 버퍼 - b0)
    D3D11_BUFFER_DESC matrixBufferDesc = {};
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    device->CreateBuffer(&matrixBufferDesc, nullptr, &m_matrixBuffer);

    // 상수 버퍼 생성 (글로벌 버퍼 - b1)
    matrixBufferDesc.ByteWidth = sizeof(GlobalBuffer);
    device->CreateBuffer(&matrixBufferDesc, nullptr, &m_globalBuffer);

    // 상수 버퍼 생성 (클라우드 버퍼 - b3)
    D3D11_BUFFER_DESC cloudBufferDesc = {};
    cloudBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    cloudBufferDesc.ByteWidth = sizeof(CloudBuffer); // 현재 80 bytes (16의 배수)
    cloudBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cloudBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    if (FAILED(device->CreateBuffer(&cloudBufferDesc, nullptr, &m_cloudBuffer)))
        return false;

    return true;
} // InitBuffers


void CloudShader::SetShaders(ID3D11DeviceContext* context)
{
    context->IASetInputLayout(m_layout.Get());
    context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
} // SetShaders


void CloudShader::SetConstantBuffers(ID3D11DeviceContext* context, ID3D11Buffer* lightBuffer)
{
    context->VSSetConstantBuffers(0, 1, m_matrixBuffer.GetAddressOf()); // 행렬(b0)
    context->PSSetConstantBuffers(1, 1, m_globalBuffer.GetAddressOf()); // 레이마칭(b1)
    
	// 조명 버퍼(b2)
    ID3D11Buffer* buffers[] = { lightBuffer };
    context->PSSetConstantBuffers(2, 1, buffers);

	// 구름 버퍼(b3)
    context->PSSetConstantBuffers(3, 1, m_cloudBuffer.GetAddressOf());
} // SetConstantBuffers


bool CloudShader::UpdateCloudBuffer(ID3D11DeviceContext* context, const CloudBuffer& data)
{
    D3D11_MAPPED_SUBRESOURCE mapped;
    if (FAILED(context->Map(m_cloudBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
        return false;

    // 데이터 복사
    CloudBuffer* pData = (CloudBuffer*)mapped.pData;
    *pData = data;

    context->Unmap(m_cloudBuffer.Get(), 0);
    return true;
} // UpdateCloudBuffer