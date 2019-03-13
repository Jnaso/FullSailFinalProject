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
}


InputManager::~InputManager()
{
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


