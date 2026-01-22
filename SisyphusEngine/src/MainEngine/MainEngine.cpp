#include "MainEngine.h"
#include "Timer.h"
#include "Fps.h"
#include "Camera.h"
// System
#include "Input.h"
#include "Gui.h"
// UI
#include "UI.h"
#include "UserInput.h"
// Common
#include "EngineSettings.h"
#include "EngineHelper.h"
// Rendering
#include "Renderer/Renderer.h"
#include "Model/DefaultModel.h"
#include "Model/Light.h"
#include "Model/TexturesManager.h"
#include "Shader/CloudShader.h"
#include "Shader/SunShader.h"

MainEngine::MainEngine()
{
    m_Timer = std::make_unique<Timer>();
    m_Fps = std::make_unique<Fps>();
    m_Renderer = std::make_unique<Renderer>();
    m_Camera = std::make_unique<Camera>();
    m_TexturesManager = std::make_unique<TexturesManager>();
    m_CloudShader = std::make_unique<CloudShader>();
    m_SunShader = std::make_unique<SunShader>();
    m_SunModel = std::make_unique<DefaultModel>();
    m_CloudArea = std::make_unique<DefaultModel>();
    m_UserInput = std::make_unique<UserInput>();
    m_UI = std::make_unique<UI>();
    m_Light = std::make_unique<Light>();
} // MainEngine


MainEngine::~MainEngine() {}


bool MainEngine::Init(HWND hwnd, std::shared_ptr<Input> input, std::shared_ptr<Gui> gui)
{
    m_Input = input;
    m_Gui = gui;

    if (m_Timer->Init() == false) return false;
    m_Fps->Init();

    if (m_Renderer->Init(hwnd, true) == false) return false;

    m_Camera->SetPosition(0.0f, 0.0f, -6.0f);
    m_Camera->InitView();
    m_Camera->InitProjection(
        EngineSettings::SCREEN_WIDTH, EngineSettings::SCREEN_HEIGHT,
        EngineSettings::SCREEN_NEAR, EngineSettings::SCREEN_DEPTH
    );

    if (m_TexturesManager->Init(
        m_Renderer->GetDevice(),
        m_Renderer->GetDeviceContext())
    == false) return false; 

    if (m_CloudShader->Init(
        m_Renderer->GetDevice(), 
        hwnd,
        EngineSettings::DEFAULT_VS,
        EngineSettings::CLOUD_PS) == false) return false;
    

    if (m_CloudArea->Init(m_Renderer->GetDevice(), DefaultModelType::Sphere) == false)
        return false;
    m_CloudArea->SetScale(10.0f);
    //m_CloudArea->SetPosition({ 0.0f, 0.0f, 0.0f });

    m_Light->Init({ 5.0f, 5.0f, 5.0f }, { 1.0f, 0.9f, 0.7f, 1.0f }, 2.5f);
    if (m_SunModel->Init(m_Renderer->GetDevice(), DefaultModelType::Sphere)
        == false) return false;
    m_SunModel->SetPosition(m_Light->GetPosition()); // Light의 위치와 동기화
    //m_SunModel->SetScale(2.0f);

    if (m_SunShader->Init(
        m_Renderer->GetDevice(),
        hwnd,
        EngineSettings::DEFAULT_VS,
        EngineSettings::SUN_PS)
        == false) return false;
    
    if (gui->Init(hwnd,
        m_Renderer->GetDevice(),
        m_Renderer->GetDeviceContext())
        == false) return false;

    //m_UserInput = std::make_unique<UserInput>();
    if (m_UserInput->Init(input) == false) return false;
    //m_UI = std::make_unique<UI>();
    if (m_UI->Init(gui) == false) return false;
    
    return true;
} // Init


void MainEngine::Shutdown()
{

    if (m_Renderer) m_Renderer->Shutdown();
} // Shutdown


bool MainEngine::Frame()
{
    m_Timer->Frame();
    m_Fps->Frame();

    if (m_UserInput->IsEscapePressed()) return false;

    Render();

    return true;
} // Frame


void MainEngine::Render()
{
    m_Renderer->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    ID3D11DeviceContext* context = m_Renderer->GetDeviceContext();

    m_Renderer->SetAlphaBlending(true);

    m_SunShader->UpdateGlobalBuffer(context,
        m_Timer->GetTotalTime(),
        m_Camera->GetPosition(),
        0.0f);
    m_SunShader->UpdateMatrixBuffer(
        context,
        m_SunModel->GetModelMatrix(),
        m_Camera->GetViewMatrix(),
        m_Camera->GetProjectionMatrix());
    m_SunShader->UpdateLightBuffer(context, m_Light.get());

    m_SunShader->SetShaders(context);
    m_SunShader->SetConstantBuffers(context);
    m_SunModel->Render(context);

    //m_Renderer->SetAlphaBlending(true);
    m_Renderer->SetSampler(0);
    m_TexturesManager->PSSetShaderResources(context, EngineSettings::NOISE_PATH, 0);
   
    m_CloudShader->UpdateGlobalBuffer(
        context,
        m_Timer->GetTotalTime(),
        m_Camera->GetPosition(),
        256.0f
    );

    m_CloudShader->UpdateMatrixBuffer(
        context,
        m_CloudArea->GetModelMatrix(),
        m_Camera->GetViewMatrix(),
        m_Camera->GetProjectionMatrix());

    m_CloudShader->SetShaders(context);
    m_CloudShader->SetConstantBuffers(context, m_SunShader->GetLightBuffer());
    m_CloudArea->Render(context);

    m_Renderer->SetAlphaBlending(false);
    m_Renderer->EndScene();
} // Render