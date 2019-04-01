#pragma once
#include <vector>
#include "graphicsclass.h"
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
	UIManager* myUiManager;
	
	char displayString[65];
	char displayString1[65];

public:
	GameManager();
	~GameManager();
	void Update(float delta);

	bool Render();

	bool Initialize(int windowWidth, int windowHeight, HWND window);
	void ShutDown();

	void SetKeyState(int keyCode, bool isPress);
	KEYSTATE GetKeyState(int keyCode);

	UIElement* m_scoreText;
	UIElement* m_healthText;

	UIElement* m_YouLose;
	UIElement* m_YouWin;

	void UpdateScoreText();
	void UpdateHealthText();
	
	InputManager* GetInputManager();
	Graphics* GetGraphicsManager();
	UIManager* GetUIManager() { return myGraphics->GetUIManager(); }
};