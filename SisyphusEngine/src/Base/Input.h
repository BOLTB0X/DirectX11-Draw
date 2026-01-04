// System/Input.h
#pragma once
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "Common/EngineHelper.h"

class Input
{
public:
	Input();
	Input(const Input&) = delete;
	~Input();

	bool Init(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

public:
	// 마우스
	void GetMouseLocation(int&, int&);
	bool IsMousePressed();

	bool IsEscapePressed();

	// 방향키
	bool IsLeftPressed();
	bool IsRightPressed();
	bool IsUpPressed();
	bool IsDownPressed();

	bool IsAPressed();
	bool IsWPressed();
	bool IsSPressed();
	bool IsDPressed();
	bool IsXPressed();

	bool IsZPressed();

	bool IsPgUpPressed();
	bool IsPgDownPressed();

	// 펑션키
	bool IsF1Toggled();
	bool IsF2Toggled();
	bool IsF3Toggled();
	bool IsF4Toggled();

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight, m_mouseX, m_mouseY;
	bool m_F1_released, m_F2_released, m_F3_released, m_F4_released;
}; // Input