#pragma once
#include <vector>
#include "graphicsclass.h"
#include "UIManager.h"
#include "InputManager.h"
#include "GameObject.h"
#include "dxstuff.h"

class GameManager
{
	DX* myDx;
	// Replace int with GameObjects when implemented
	std::vector<GameObject*> GameObjects;
	//Game Classes
	Graphics *myGraphics;
	InputManager *myInput;
	UI::UIManager* myUiManager;
public:
	GameManager();
	~GameManager();
	void Update(float delta);

	bool Render();

	bool Initialize(int windowWidth, int windowHeight, HWND window);
	void ShutDown();

	void SetKeyState(int keyCode, bool isPress);
	KEYSTATE GetKeyState(int keyCode);

	InputManager* GetInputManager();
	Graphics* GetGraphicsManager();
	UI::UIManager* GetUIManager();

	void CreateImage(RECT dimensions, bool interact, const char * filePath, float2 pos);
};

