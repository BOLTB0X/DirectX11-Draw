#pragma once
#include <windows.h>
#include "Renderer/Resources/Model.h"
#include "Renderer/Shaders/TextureShader.h"

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

    Model* m_RockModel = nullptr;
    TextureShader* m_TextureShader = nullptr;

    DirectX::XMMATRIX m_World;
    DirectX::XMMATRIX m_View;
    DirectX::XMMATRIX m_Proj;
};
