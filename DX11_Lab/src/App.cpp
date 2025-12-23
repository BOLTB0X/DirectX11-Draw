#include "App.h"
#include "Window/Win32Window.h"
#include "Renderer/Core/DX11Renderer.h"

App::App(HINSTANCE hInstance)
    : m_hInstance(hInstance),
    m_RockModel(nullptr),
    m_TextureShader(nullptr),
    m_World(DirectX::XMMatrixIdentity()),
    m_View(DirectX::XMMatrixIdentity()),
    m_Proj(DirectX::XMMatrixIdentity())
{
    m_RockModel = new Model();
    m_TextureShader = new TextureShader();
}

App::~App() {
    if (m_TextureShader) {
        delete m_TextureShader;
        m_TextureShader = nullptr;
    }
    if (m_RockModel) {
        delete m_RockModel;
        m_RockModel = nullptr;
    }
} // ~App

void App::Run()
{
    Win32Window window(m_hInstance, 1280, 720);

    DX11Renderer renderer;
    if (!renderer.Init(window.GetHWND()))
        return;

    // 1번 체크: Init 자체가 실패하는가?
    if (!renderer.Init(window.GetHWND())) {
        MessageBoxW(NULL, L"Renderer Init 자체가 실패함!", L"Error", MB_OK);
        return;
    }

    if (renderer.GetDevice() == nullptr) {
        MessageBoxW(NULL, L"GetDevice()가 NULL을 반환", L"Error", MB_OK);
        return;
    }


    //MessageBoxW(NULL, L"여기까지 오면 Renderer는 정상임!", L"Success", MB_OK);

    //if (!m_RockModel->Load(renderer.GetDevice(), "assets/rock_base/rock_base_lp.fbx")) {
    //    MessageBoxW(window.GetHWND(), L"Failed to load Model", L"Error", MB_OK);
    //}

    m_View = DirectX::XMMatrixLookAtLH(
        { 0.0f, 2.0f, -5.0f }, // 카메라 위치
        { 0.0f, 0.0f, 0.0f },  // 바라보는 지점
        { 0.0f, 1.0f, 0.0f }   // 하늘 방향
    );

    m_Proj = DirectX::XMMatrixPerspectiveFovLH(
        DirectX::XM_PIDIV4,    // 45도 시야각
        1280.0f / 720.0f,      // 종횡비
        0.1f, 1000.0f          // 근평면/원평면
    );

    m_World = DirectX::XMMatrixScaling(100.0f, 100.0f, 100.0f);
    m_World *= DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);

    while (!window.ShouldClose())
    {
        window.PollEvents();

        renderer.BeginFrame();
       
        // Draw
        m_RockModel->Draw(renderer.GetContext(), m_TextureShader, m_World, m_View, m_Proj);

        renderer.EndFrame();
    }

    renderer.Shutdown();
} // Run
