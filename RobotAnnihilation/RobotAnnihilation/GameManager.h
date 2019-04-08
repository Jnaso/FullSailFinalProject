#pragma once
#include <vector>
#include "graphicsclass.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Player.h"
#include "Gun.h"
#include "dxstuff.h"
#include "EnemyManager.h"

class GameManager
{
	DX* myDX;
	//Game Classes
	Graphics *myGraphics;
	InputManager *myInput;
	HWND window;
	
	
	char displayString[65];
	char displayString1[65];
	char displayString2[65];

	bool betweenRounds = false;
	float countDown = 0.0f;
	unsigned int currentRound = 0;

	std::string numberToChr;

	Player *myPlayer;
	vector<Bullet*> bullets;
	EnemyManager *myEnemyManager;

	vector<GameObject*> Obstacles;

	vector<GameObject*> Pickups;

public:
	GameManager();
	~GameManager();
	void Update(float delta, float total);

	bool Render();

	bool Initialize(int windowWidth, int windowHeight, HWND window);
	void ShutDown();

	void SetKeyState(int keyCode, bool isPress);
	KEYSTATE GetKeyState(int keyCode);

	UIElement* m_scoreText;
	UIElement* m_healthText;
	UIElement* m_timerText;

	UIElement* m_YouLose;
	UIElement* m_YouWin;

	UIElement* m_weapon;

	void UpdateScoreText();
	void UpdateHealthText();
	void UpdateWeaponText();
	void UpdateTimerText(float time);
	
	InputManager* GetInputManager();
	Graphics* GetGraphicsManager();
	UIManager* GetUIManager() { return myGraphics->GetUIManager(); }


	void ShootBullets();

	unsigned int GetEnemies() { return myEnemyManager->GetEnemyCount(); };
	unsigned int GetHealth() { return myPlayer->GetHealth(); };

	void SpawnPickup(float3 pos);
};

