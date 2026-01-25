#include "Pch.h"
#include "SamplerState.h"


SamplerState::SamplerState()
{
}

SamplerState::~SamplerState()
{
}


bool SamplerState::Init(ID3D11Device* device)
{
    D3D11_SAMPLER_DESC sampDesc = {};

    // 필터 설정: 선형 보간을 통해 노이즈를 부드럽게 만듦
    //sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    sampDesc.MipLODBias = 0.0f;
    sampDesc.MaxAnisotropy = 1;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.BorderColor[0] = 0;
    sampDesc.BorderColor[1] = 0;
    sampDesc.BorderColor[2] = 0;
    sampDesc.BorderColor[3] = 0;

    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    HRESULT hr = device->CreateSamplerState(&sampDesc, m_SamplerState.GetAddressOf());
    if (FAILED(hr))
    {
        // DebugHelper 혹은 기존 에러 출력 방식 사용
        return false;
    }

    return true;
} // Init


void SamplerState::VSSetSamplers(ID3D11DeviceContext* context, UINT slot)
{
    context->VSSetSamplers(slot, 1, m_SamplerState.GetAddressOf());
} // VSSetSamplers


void SamplerState::PSSetSamplers(ID3D11DeviceContext* context, UINT slot)
{
    context->PSSetSamplers(slot, 1, m_SamplerState.GetAddressOf());
} // PSSetSamplers