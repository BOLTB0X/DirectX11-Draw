// Graphics/Renderer/DepthStencilState.cpp
#include "DepthStencilState.h"
#include "Common/EngineHelper.h"


/* defualt */
/////////////////////////////////////////////////////////////////////

DepthStencilState::DepthStencilState() {}


DepthStencilState::~DepthStencilState() {}


bool DepthStencilState::Init(ID3D11Device* device) {
    D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc = {};
    ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

    // 깊이 테스트 활성화 상태 설정
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // 정면(Front Face) 스텐실 작업 설정
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // 뒷면(Back Face) 스텐실 작업 설정
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    if (EngineHelper::SuccessCheck(device->CreateDepthStencilState(&depthStencilDesc, &m_depthEnabledState),
        "DepthEnabled State 생성 실패")
        == false) return false;

    // 깊이 테스트 비활성화 상태 설정
    depthDisabledStencilDesc.DepthEnable = false;
    depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // 쓰기 방지
    depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
    depthDisabledStencilDesc.StencilEnable = false;

    if (EngineHelper::SuccessCheck(device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledState),
        "DepthDisabled State 생성 실패")
        == false) return false;

    return true;
} // Init


void DepthStencilState::Bind(ID3D11DeviceContext* context, bool depthEnable) {
    if (depthEnable)
        context->OMSetDepthStencilState(m_depthEnabledState.Get(), 1);
    else 
        context->OMSetDepthStencilState(m_depthDisabledState.Get(), 1);
} // Bind