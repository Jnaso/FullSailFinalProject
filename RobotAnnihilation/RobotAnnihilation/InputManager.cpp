#include "InputManager.h"
#include <iostream>


InputManager::InputManager()
{
	m_keys = new bool[258];

	for (int i = 0; i < 258; i++)
	{
		m_keys[i] = false;
	}
}


InputManager::~InputManager()
{
	delete[] m_keys;
}

bool InputManager::Initialize(HINSTANCE instance, HWND wind)
{
	HRESULT hr;
	hr = DirectInput8Create(instance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&DirectInput,
		NULL);

	hr = DirectInput->CreateDevice(GUID_SysMouse,
		&mouseinput,
		NULL);

	hr = mouseinput->SetDataFormat(&c_dfDIMouse);
	hr = mouseinput->SetCooperativeLevel(wind, DISCL_NONEXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);;

	return true;
}

void InputManager::SetKeyState(int keyCode, bool isPress)
{
	m_keys[keyCode] = isPress;
}

void InputManager::SetKetState(char keyCode, bool isPressed)
{
	m_keys[(int)keyCode] = isPressed;
}

KEYSTATE InputManager::GetKeyState(int keyCode)
{
	
	if (m_keys[keyCode])
	{
		return KEYSTATE::DOWN;
	}
	else
	{
		return KEYSTATE::UP;
	}
}

KEYSTATE InputManager::GetKeyState(char keyCode)
{
	if (m_keys[(int)keyCode])
	{
		return KEYSTATE::DOWN;
	}
	else
	{
		return KEYSTATE::UP;
	}
}

IDirectInputDevice8* InputManager::GetMouseInput()
{
	return mouseinput;
}

DIMOUSESTATE InputManager::GetPrevMouseState()
{
	return lastMouseState;
}

void InputManager::SetPrevMouseState(DIMOUSESTATE state)
{
	lastMouseState = state;
}

DIMOUSESTATE InputManager::GetCurrMouseState()
{
	return currMouseState;
}

void InputManager::SetCurrMouseState()
{
	mouseinput->GetDeviceState(sizeof(DIMOUSESTATE), &currMouseState);
}



