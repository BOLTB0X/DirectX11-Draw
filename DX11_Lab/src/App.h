#pragma once
#include <windows.h>

class Win32Window;
class DX11Renderer;

class App
{
public:
    App(HINSTANCE hInstance);
    ~App();

    void Run();

private:
    HINSTANCE m_hInstance;
};
