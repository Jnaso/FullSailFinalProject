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
	const float DEFAULTKEYPRESST = 0.01f;
	float keyPressTimer = 0.01f;

	DX* myDX;
	Graphics *myGraphics;
	//Game Classes
	Shop* myShop;
	InputManager *myInput;
	HWND window;
	
	int screenW, screenH;
	
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
	bool isDone = false;
	bool paused = false;

	GameManager();
	~GameManager();
	void Update(float delta, float total);

	bool Render();

	void GameEnd();

	bool Initialize(int windowWidth, int windowHeight, HWND window);
	void ShutDown();

	void SetKeyState(int keyCode, bool isPress);
	KEYSTATE GetKeyState(int keyCode);

	UIElement* m_scoreText;
	UIElement* m_healthText;
	UIElement* m_timerText;
	UIElement* m_damagetimerText;

	UIElement* m_YouLose;
	UIElement* m_youLoseQuitButton;

	UIElement* m_YouWin;

	UIElement* m_weapon;

	UIElement* m_lowHealthImage;

	//UI Modifier Functions
	void UpdateScoreText();
	void UpdateHealthText();
	void UpdateWeaponText();
	void UpdateTimerText(float time);
	void UpdateDamageTimerText();
	void SetLowHealthImage(bool val);
	
	InputManager* GetInputManager();
	Graphics* GetGraphicsManager();
	UIManager* GetUIManager() { return myGraphics->GetUIManager(); }
	Shop* GetShopPtr() { return myShop; }
	EnemyManager* GetEnemyMangerPtr() { return myEnemyManager; }


	void ShootBullets();

	unsigned int GetEnemies() { return myEnemyManager->GetEnemyCount(); };
	unsigned int GetHealth() { return myPlayer->GetHealth(); };
	Player* GetPlayer() { return myPlayer; }

	void SpawnHealthPickup(float3 pos);
	void SpawnDamagePickup(float3 pos);

	void ExitLevel();

#pragma region CheatCodes
	void MaxHealth() { myPlayer->SetHealth(myPlayer->GetMaxHealth()); }
	void FlipInvincible() { myPlayer->FlipInvincible(); }
	void AddMoney() { myPlayer->AddCurrency(1000); }
	void UnlockAllGuns() {
		if (myPlayer->GetCurrentGuns().size() <=1)
		{
			Gun* MachineGun = new Gun();
			MachineGun->SetGunClass(Gun::MACHINE);
			MachineGun->SetFireRate(0.3f);
			MachineGun->SetDamageAmount(35);
			myPlayer->AddGun(MachineGun);
			Gun* SubMachineGun = new Gun();
			SubMachineGun->SetGunClass(Gun::SUBMACHINE);
			SubMachineGun->SetFireRate(0.25f);
			SubMachineGun->SetDamageAmount(30);
			myPlayer->AddGun(SubMachineGun);
		}
		else if (myPlayer->GetCurrentGuns().size() <= 1 && myPlayer->GetCurrentGuns()[2]->GetGunClass() == Gun::GunClass::MACHINE)
		{
			Gun* SubMachineGun = new Gun();
			SubMachineGun->SetGunClass(Gun::SUBMACHINE);
			SubMachineGun->SetFireRate(0.25f);
			SubMachineGun->SetDamageAmount(30);
			myPlayer->AddGun(SubMachineGun);
		}
		else if (myPlayer->GetCurrentGuns().size() <= 1 && myPlayer->GetCurrentGuns()[2]->GetGunClass() == Gun::GunClass::SUBMACHINE)
		{
			Gun* MachineGun = new Gun();
			MachineGun->SetGunClass(Gun::MACHINE);
			MachineGun->SetFireRate(0.3f);
			MachineGun->SetDamageAmount(35);
			myPlayer->AddGun(MachineGun);
		}
	}
	void EndRound();
#pragma endregion
};

