// Application/Application.h
#pragma once
#include <windows.h>
#include <memory>

class Input;
class Timer;
class Fps;
class Gui;
class MainSideBarWidget;
class Renderer;
class ShaderManager;
class ModelManager;
class TexturesManager;
class World;
class Camera;

class Application
{
public:
    Application();
    Application(const Application&) = delete;
    ~Application();

    bool Init(HWND, int, int);
    void Shutdown();
    bool Frame();

public:
    Input* GetInput() { return m_Input; };

private:
    bool Render();
    void InitGui();
	void ApplySideBarRenderStates();
    void HandleSideBar();

private:
    Input* m_Input;
    Timer* m_Timer;
    Fps* m_Fps;
    Gui* m_Gui;
	MainSideBarWidget* m_SideBarWidget;

private:
    std::unique_ptr<Renderer> m_Renderer;
    std::unique_ptr<ShaderManager> m_ShaderManager;
    std::unique_ptr<ModelManager> m_ModelManager;
    std::unique_ptr<TexturesManager> m_TextureManager;
    std::unique_ptr<World> m_World;
    std::unique_ptr<Camera> m_Camera;

}; // Application