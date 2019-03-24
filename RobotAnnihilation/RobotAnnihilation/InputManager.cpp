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

void InputManager::SetKeyState(int keyCode, bool isPress)
{
	m_keys[keyCode] = isPress;

	/*std::cout << "Key: " << keyCode << std::endl;
	if (keyCode == _SPACE)
	{
		std::cout << "Space";
	}
	else if (keyCode == _SHIFT)
	{
		std::cout << "Shift";
	}
	else if (keyCode == _CTRL)
	{
		std::cout << "Control";
	}
	else if (keyCode == _ESCAPE)
	{
		std::cout << "Escape";
	}
	else if (keyCode == _ARROWRIGHT)
	{
		std::cout << "Right";
	}
	else if (keyCode == _ARROWUP)
	{
		std::cout << "Up";
	}
	else if (keyCode == _ARROWLEFT)
	{
		std::cout << "Left";
	}
	else if (keyCode == _ARROWDOWN)
	{
		std::cout << "Down";
	}
	else if (keyCode == _LMOUSE)
	{
		std::cout << "Left Mouse";
	}
	else if (keyCode == _RMOUSE)
	{
		std::cout << "Right Mouse";
	}
	else
	{
		std::cout << (char)keyCode;
	}
	

	if (isPress)
	{
		std::cout << " is Pressed" << std::endl;
	}
	else
	{
		std::cout << " is Released" << std::endl;
	}*/
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

void InputManager::SetMousePos(float x, float y)
{
	prevMousePos = { mousePos.x, mousePos.y };
	mousePos.x = x;
	mousePos.y = y;

	//std::cout << "MousePOS: " << "( " << mousePos.x << ", " << mousePos.y << " )" << std::endl;
}

float2 InputManager::GetPrevMousePos()
{
	return prevMousePos;
}

float2 InputManager::GetMousePos()
{
	return mousePos;
}



