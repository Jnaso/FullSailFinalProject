#pragma once
#include "MathDefines.h"

class InputManager
{
private:
	enum KEYSTATE { UP, DOWN };

	bool m_keys[256];

	float2 mousePos;

public:

	InputManager();
	~InputManager();

	void SetKeyState(int keyCode, bool isPress);
	KEYSTATE GetKeyState(int keyCode);

	void SetMousePos(float x, float y);
	float2 GetMousePos();

	
};

