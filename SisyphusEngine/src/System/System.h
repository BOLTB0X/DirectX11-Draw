// System/System.h
#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "Base/Input.h"
#include "Application/Application.h"


class System {
public:
    System();
    System(const System&) = delete;
    ~System();

    bool Init();
    void Shutdown();
    void Run();
    bool Frame();
    LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
    bool InitWindows(int&, int&);
    void ShutdownWindows();

private:
    LPCWSTR m_applicationName;
    HINSTANCE m_hinstance;
    HWND m_hwnd;

private:
    Input* m_InputPtr;
    Application* m_Application;
};

//static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static System* ApplicationHandle = nullptr;