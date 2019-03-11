#pragma once

#include <dinput.h>

#define DIRECTINPUT_VERSION 0x0800

class InputManager
{
public:
	InputManager();
	~InputManager();

	bool Initialize(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight);
	bool Update();

	bool isKeyDown(unsigned int key);
	bool isKeyHit(unsigned int key);
	void getMousePosition(int& x, int& y);

private:
	const static int s_NumKeys = 256;

	bool ReadKeyBoard();
	bool ReadMouse();
	void ProcessInput();

	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;
	DIMOUSESTATE m_mouseState;

	bool m_keys[s_NumKeys];
	bool m_prevKeys[s_NumKeys];

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};

