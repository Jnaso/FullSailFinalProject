#include "InputManager.h"
#include <iostream>

#pragma region Key_Macros

#define ARROWRIGHT 37
#define ARROWUP 38
#define ARROWDOWN 38
#define ARROWLEFT 39

#define SHIFT 16
#define CTRL 17
#define SPACE 32
#define ESCAPE 27
#define TILDE 192

#define NUM_PAD_0 96
#define NUM_PAD_1 97
#define NUM_PAD_2 98
#define NUM_PAD_3 99
#define NUM_PAD_4 100
#define NUM_PAD_5 101
#define NUM_PAD_6 102
#define NUM_PAD_7 103
#define NUM_PAD_8 104
#define NUM_PAD_9 105

#define NUM_ROW_0 48
#define NUM_ROW_1 49
#define NUM_ROW_2 50
#define NUM_ROW_3 51
#define NUM_ROW_4 52
#define NUM_ROW_5 53
#define NUM_ROW_6 54
#define NUM_ROW_7 55
#define NUM_ROW_8 56
#define NUM_ROW_9 57

#define COMMA 188
#define PERIOD 190  

#pragma endregion

void InputManager::SetKeyState(int keyCode, bool isPress)
{
	m_keys[keyCode] = isPress;
}

void InputManager::SetMousePos(float x, float y)
{
	mousePos.x = x;
	mousePos.y = y;

	std::cout << "MousePOS: " << "( " << mousePos.x << ", " << mousePos.y << " )" << std::endl;
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


