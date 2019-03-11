#include "stdafx.h"
#include "InputManager.h"





InputManager::InputManager()
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
	memset(m_keys, 0, sizeof(bool)*s_NumKeys);
}


InputManager::~InputManager()
{
	if (m_mouse)
	{
		m_mouse->Release();
		m_mouse = 0;
	}
	if (m_keyboard)
	{ 
		m_keyboard->Release();
		m_keyboard = 0;
	}
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}
}

bool InputManager::Initialize(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT res;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_mouseX = 0;
	m_mouseY = 0;

	//Initialize direct input interface
	res = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, nullptr);
	if (FAILED(res))
	{
		return false;
	}

	//Initialize Keyboard Interface

	res = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, nullptr);
	if (FAILED(res))
	{
		return false;
	}

	//Set Keyboard Data Format
	res = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(res))
	{
		return false;
	}

	//Set Cooperative level of keyboard to prevent from sharing with other programs
	res = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(res))
	{
		return false;
	}

	res = m_keyboard->Acquire();
	if (FAILED(res))
	{
		return false;
	}

	//Initialize Mouse Interface
	res = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, nullptr);
	if (FAILED(res))
	{
		return false;
	}

	//Set data format for mouse
	res = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(res))
	{
		return false;
	}

	res = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(res))
	{
		return false;
	}

	res = m_mouse->Acquire();
	if (FAILED(res))
	{
		return false;
	}

	return true;
}

bool InputManager::Update()
{
	memcpy(m_prevKeys, m_keys, sizeof(bool) * s_NumKeys);

	bool result;

	//Read Keyboard
	result = ReadKeyBoard();
	if (!result)
	{
		return false;
	}

	//Read Mouse
	result = ReadMouse();
	if (!result)
	{
		return false;
	}


	//Update Mouse Position
	ProcessInput();
	return true;
}

bool InputManager::isKeyDown(unsigned int key)
{
	return m_keys[key];
}

bool InputManager::isKeyHit(unsigned int key)
{
	return m_keys[key] && !m_prevKeys[key];
}

void InputManager::getMousePosition(int & x, int & y)
{
	x = m_mouseX;
	y = m_mouseY;
}

bool InputManager::ReadKeyBoard()
{
	HRESULT res;

	//Read Keyboard Device
	res = m_keyboard->GetDeviceState(sizeof(m_keys), (LPVOID)&m_keys);
	if (FAILED(res))
	{
		if ((res) == DIERR_INPUTLOST || (res) == DIERR_NOTACQUIRED)
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool InputManager::ReadMouse()
{
	//Sometimes Mouse Doesn't Get Initialized when putting a break point before it reaches update
	if (m_mouse == nullptr)
	{
		m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, nullptr);
		return false;
	}

	HRESULT res;

	//Read mouse device
	res = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(res))
	{
		if ((res) == DIERR_INPUTLOST || (res) == DIERR_NOTACQUIRED)
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}
	
}

void InputManager::ProcessInput()
{
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	if (m_mouseX < 0) m_mouseX = 0;
	if (m_mouseY < 0) m_mouseY = 0;
	if (m_mouseX < m_screenWidth) m_mouseX = m_screenWidth;
	if (m_mouseY < m_screenHeight) m_mouseY = m_screenHeight;
}
