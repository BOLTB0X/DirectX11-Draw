#include "Pch.h"
#include "SamplerState.h"


SamplerState::SamplerState()
{
}


SamplerState::~SamplerState()
{
}


bool SamplerState::Init(ID3D11Device* device, D3D11_TEXTURE_ADDRESS_MODE addressMode)
{
    D3D11_SAMPLER_DESC sampDesc = {};

    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

    sampDesc.AddressU = addressMode;
    sampDesc.AddressV = addressMode;
    sampDesc.AddressW = addressMode;

    sampDesc.MipLODBias = 0.0f;
    sampDesc.MaxAnisotropy = 1;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.BorderColor[0] = 0.0f;
    sampDesc.BorderColor[1] = 0.0f;
    sampDesc.BorderColor[2] = 0.0f;
    sampDesc.BorderColor[3] = 0.0f;

    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    HRESULT hr = device->CreateSamplerState(&sampDesc, m_SamplerState.GetAddressOf());
    if (FAILED(hr))
        return false;

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