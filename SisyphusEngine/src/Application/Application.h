// Application/Application.h
#pragma once

#include "Base/Input.h"
#include "Base/Timer.h"
#include "Base/Fps.h"

#include "Graphics/Renderer/Renderer.h"
#include "Graphics/Gui/Gui.h"

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

private:
    Input* m_Input;
    Timer* m_Timer;
    Fps* m_Fps;

private:
	Renderer* m_Renderer;
	Gui* m_Gui;

}; // Application