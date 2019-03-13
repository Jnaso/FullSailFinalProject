#pragma once
class InputManager
{
private:
	enum KEYSTATE { UP, DOWN };

	bool m_keys[256];

public:

	InputManager();
	~InputManager();

	KEYSTATE GetKeyState(int keyCode);

	void SetKeyState(int keyCode, bool isPress);
};

