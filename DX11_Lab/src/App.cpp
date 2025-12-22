#include "App.h"
#include "Window/Win32Window.h"
#include "Renderer/DX11Renderer.h"

App::App(HINSTANCE hInstance)
    : m_hInstance(hInstance)
{
} // App

App::~App() {
} // ~App

void App::Run()
{
    Win32Window window(m_hInstance, 1280, 720);

    DX11Renderer renderer;
    if (!renderer.Init(window.GetHWND()))
        return;

    while (!window.ShouldClose())
    {
        window.PollEvents();

        renderer.BeginFrame();
       
        // Draw

        renderer.EndFrame();
    }

    renderer.Shutdown();
} // Run
