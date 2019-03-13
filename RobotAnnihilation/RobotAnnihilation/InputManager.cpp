#include "InputManager.h"
#include <iostream>


void InputManager::SetKeyState(int keyCode, bool isPress)
{
	m_keys[keyCode] = isPress;
}

void InputManager::SetMousePos(float x, float y)
{
	mousePos.x = x;
	mousePos.y = y;

	//std::cout << "MousePOS: " << "( " << mousePos.x << ", " << mousePos.y << " )" << std::endl;
}

InputManager::InputManager()
{
	m_keys = new bool[256];

	for (int i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}
}


InputManager::~InputManager()
{
	delete[] m_keys;
}

InputManager::KEYSTATE InputManager::GetKeyState(int keyCode)
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

float2 InputManager::GetMousePos()
{
	return mousePos;
}


