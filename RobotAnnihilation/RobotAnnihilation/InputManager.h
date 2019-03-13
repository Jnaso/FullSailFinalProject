#pragma once
#include "MathDefines.h"

#pragma region Key_Macros

#define _ARROWRIGHT 37
#define _ARROWUP 38
#define _ARROWDOWN 38
#define _ARROWLEFT 39
		
#define _SHIFT 16
#define _CTRL 17
#define _SPACE 32
#define _ESCAPE 27
#define _TILDE 192
		
#define _NUM_PAD_0 96
#define _NUM_PAD_1 97
#define _NUM_PAD_2 98
#define _NUM_PAD_3 99
#define _NUM_PAD_4 100
#define _NUM_PAD_5 101
#define _NUM_PAD_6 102
#define _NUM_PAD_7 103
#define _NUM_PAD_8 104
#define _NUM_PAD_9 105
		
#define _NUM_ROW_0 48
#define _NUM_ROW_1 49
#define _NUM_ROW_2 50
#define _NUM_ROW_3 51
#define _NUM_ROW_4 52
#define _NUM_ROW_5 53
#define _NUM_ROW_6 54
#define _NUM_ROW_7 55
#define _NUM_ROW_8 56
#define _NUM_ROW_9 57
		
#define _COMMA 188
#define _PERIOD 190  

#pragma endregion

class InputManager
{
private:
	enum KEYSTATE { UP, DOWN };

	bool *m_keys;

	float2 mousePos;

public:

	InputManager();
	~InputManager();

	void SetKeyState(int keyCode, bool isPress);
	KEYSTATE GetKeyState(int keyCode);

	void SetMousePos(float x, float y);
	float2 GetMousePos();

	
};

