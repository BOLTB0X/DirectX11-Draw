#include "DX11Renderer.h"
#include <dxgi.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

bool DX11Renderer::Init(HWND hwnd)
{
    DXGI_SWAP_CHAIN_DESC sd{};
    sd.BufferCount = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION,
        &sd, &m_SwapChain, &m_Device, nullptr, &m_Context
    );
    if (FAILED(hr)) return false;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

    hr = m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    if (FAILED(hr)) return false;

    hr = m_Device->CreateRenderTargetView(
        backBuffer.Get(),
        nullptr,
        &m_RTV
    );
    if (FAILED(hr)) return false;

    return true;
} // init

DX11Renderer::~DX11Renderer() = default;

void DX11Renderer::BeginFrame()
{
    float color[4] = { 0.1f, 0.2f, 0.6f, 1.0f };
    m_Context->ClearRenderTargetView(m_RTV.Get(), color);
} // Render

void DX11Renderer::EndFrame()
{
    m_SwapChain->Present(1, 0);
} // Shutdown


void DX11Renderer::Shutdown() {
    m_RTV.Reset();
    m_SwapChain.Reset();
    m_Context.Reset();
    m_Device.Reset();
} // Shutdown
