#pragma once
class InventoryManager
{
private:
	enum KEYSTATE { UP, DOWN };

	bool m_keys[256];

public:

	InventoryManager();
	~InventoryManager();

	KEYSTATE GetKeyState(int keyCode);

	void SetKeyState(int keyCode, bool isPress);
};

