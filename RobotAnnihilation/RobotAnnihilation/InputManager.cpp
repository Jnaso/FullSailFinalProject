#include "InputManager.h"



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

void InputManager::SetKeyState(int keyCode , bool isPress)
{
	m_keys[keyCode] = isPress;
}
