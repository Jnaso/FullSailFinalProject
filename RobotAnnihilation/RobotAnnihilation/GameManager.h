#pragma once
#include <vector>
#include "graphicsclass.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Player.h"
#include "Gun.h"
#include "dxstuff.h"

class GameManager
{
	DX* myDX;
	// Replace int with GameObjects when implemented
	std::vector<GameObject*> GameObjects;
	//Game Classes
	Graphics *myGraphics;
	InputManager *myInput;
	UIManager* myUiManager;
	HWND window;
	
	char displayString[65];
	char displayString1[65];

	Player *myPlayer;
	vector<Bullet*> bullets;
	vector<Target*> myTargets;
	unsigned int enemyCount;

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

	void ShootBullets();

	unsigned int GetEnemies() { return enemyCount; };
	unsigned int GetHealth() { return myPlayer->GetHealth(); };
};

