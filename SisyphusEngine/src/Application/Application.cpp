// Application/Application.h
#include "Application.h"

#include "Common/EngineSettings.h"
#include "Common/EngineHelper.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

/* default */
/////////////////////////////////////////////////////////////////////

Application::Application()
	: m_Input(nullptr),
	m_Timer(nullptr),
	m_Fps(nullptr),
	m_Renderer(nullptr),
	m_Gui(nullptr)
{
} // Application

Application::~Application() { } // ~Application


bool Application::Init(HWND hwnd, int screenWidth, int screenHeight)
{
	char modelFilename[128], textureFilename1[128], textureFilename2[128];
	bool result;

	m_Input = new Input;
	if (m_Input == false) return false;
	
	result = m_Input->Init(GetModuleHandle(NULL), hwnd, screenWidth, screenHeight);
	if (result == false)
	{
		EngineHelper::ErrorBox(hwnd, L"Input 객체 초기화 실패");
		return false;
	}


	m_Timer = new Timer;
	if (m_Timer == false) return false;
	
	result = m_Timer->Init();
	if (result == false)
	{
		EngineHelper::ErrorBox(hwnd, L"Timer 객체 초기화 실패");
		return false;
	}

	m_Fps = new Fps;
	if (m_Fps == false) return false;
	m_Fps->Init();

	m_Renderer = new Renderer();
	if (m_Renderer->Init(hwnd, screenWidth, screenHeight)
		== false)
	{
		EngineHelper::ErrorBox(hwnd, L"Renderer 초기화 단계에서 실패했습니다.");
		return false;
	}

	m_Gui = new Gui();
	if (m_Gui->Init(hwnd, m_Renderer->GetDX11Device()->GetDevice(), m_Renderer->GetDeviceContext())
		== false) {
		return false;
	}

	return true;
} // Init


void Application::Shutdown()
{
	if (m_Gui)
	{
		delete m_Gui;
		m_Gui = 0;
	}

	if (m_Renderer)
	{
		m_Renderer->Shutdown();
		delete m_Renderer;
		m_Renderer = 0;
	}

	if (m_Fps)
		delete m_Fps; m_Fps = 0;

	if (m_Timer)
		delete m_Timer; m_Timer = 0;

	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	return;
} // Shutdown


bool Application::Frame()
{
	bool result;

	if (m_Input == false) return false;

	result = m_Input->Frame();
	if (result == false) return false;
	if (m_Input->IsEscapePressed()) return false;

	m_Timer->Frame();
	m_Fps->Frame();

	result = Render();
	if (result == false) return false;
	
	return true;
} // Frame
/////////////////////////////////////////////////////////////////////

/* private */
/////////////////////////////////////////////////////////////////////

bool Application::Render()
{
	m_Renderer->BeginScene(0.15f, 0.15f, 0.15f, 1.0f);
	m_Gui->Begin();

	{
		ImGui::Begin("Sisyphus Engine Control");

		// Rasterizer 제어
		static bool wireframe = false;
		if (ImGui::Checkbox("Wireframe Mode", &wireframe)) {
			m_Renderer->GetRasterizer()->Bind(m_Renderer->GetDeviceContext(), wireframe);
		}

		// BlendState 제어
		static int blendMode = 0; // 0: Off, 1: Alpha, 2: AlphaToCoverage
		ImGui::RadioButton("Blend Off", &blendMode, 0);
		ImGui::RadioButton("Alpha Blend", &blendMode, 1);
		ImGui::RadioButton("AlphaToCoverage (Future Ready)", &blendMode, 2);

		if (ImGui::Button("Apply Blend State")) {
			bool alpha = (blendMode == 1);
			bool atc = (blendMode == 2);
			m_Renderer->GetBlendState()->Bind(m_Renderer->GetDeviceContext(), alpha);
		}

		ImGui::End();
	}

	m_Gui->End();
	m_Renderer->EndScene();

	return true;
} // Render