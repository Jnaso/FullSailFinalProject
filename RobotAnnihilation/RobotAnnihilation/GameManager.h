#pragma once
#include <vector>
#include "graphicsclass.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Player.h"
#include "Gun.h"
#include "dxstuff.h"
#include "EnemyManager.h"
#include "Shop.h"
#include "Pickup.h"
class GameManager
{
	DX* myDX;
	Graphics *myGraphics;
	//Game Classes
	Shop* myShop;
	InputManager *myInput;
	HWND window;
	
	
	char displayString[65];
	char displayString1[65];
	char displayString2[65];
	bool shopVisible = false;

	bool betweenRounds = false;
	float countDown = 0.0f;
	unsigned int currentRound = 0;

	std::string numberToChr;

	Player *myPlayer;
	vector<Bullet*> bullets;
	EnemyManager *myEnemyManager;
	Sphere playerSphere;

	vector<GameObject*> Obstacles;

	vector<Pickup*> Pickups;

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
	UIElement* m_damagetimerText;

	UIElement* m_YouLose;
	UIElement* m_YouWin;

	UIElement* m_weapon;

	void UpdateScoreText();
	void UpdateHealthText();
	void UpdateWeaponText();
	void UpdateTimerText(float time);
	void UpdateDamageTimerText();
	
	InputManager* GetInputManager();
	Graphics* GetGraphicsManager();
	UIManager* GetUIManager() { return myGraphics->GetUIManager(); }


	void ShootBullets();

	unsigned int GetEnemies() { return myEnemyManager->GetEnemyCount(); };
	unsigned int GetHealth() { return myPlayer->GetHealth(); };
	Player* GetPlayer() { return myPlayer; }

	void SpawnHealthPickup(float3 pos);
	void SpawnDamagePickup(float3 pos);
};

