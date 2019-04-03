#include "GameManager.h"



GameManager::GameManager()
{
	myInput = new InputManager();
	myGraphics = new Graphics(myInput);
	myPlayer = nullptr;
	myEnemyManager = new EnemyManager();
}

GameManager::~GameManager()
{
}

void GameManager::SetKeyState(int keyCode, bool isPress)
{
	myInput->SetKeyState(keyCode, isPress);
}

KEYSTATE GameManager::GetKeyState(int keyCode)
{
	return myInput->GetKeyState(keyCode);
}

void GameManager::UpdateScoreText()
{
	
	memset(displayString, '\0', sizeof(displayString));
	_itoa_s(myEnemyManager->GetEnemyCount(), displayString, 65, 10);
	TextElement* temp = dynamic_cast<TextElement*>(m_scoreText);
	temp->SetText((const char*)displayString);
}

void GameManager::UpdateHealthText()
{
	std::string tempText = "Health: "/* + GetGraphicsManager()->GetHealth()*/;
	tempText += this->GetHealth();
	memset(displayString1, '\0', sizeof(displayString1));
	_itoa_s(myPlayer->GetHealth(), displayString1, 65, 10);
	TextElement* temp = dynamic_cast<TextElement*>(m_healthText);
	temp->SetText((const char*)displayString1);
}

void GameManager::UpdateWeaponText()
{
	TextElement* temp = static_cast<TextElement*>(m_weapon);
	switch (myPlayer->GetCurrentGun()->GetGunClass())
	{
	case Gun::PISTOL:
		temp->SetText("Current Weapon: Pistol");
		break;
	case Gun::SUBMACHINE:
		temp->SetText("Current Weapon: Sub-Machine Gun");
		break;
	case Gun::MACHINE:
		temp->SetText("Current Weapon: Machine Gun");
		break;
	}
}

void GameManager::UpdateTimerText(float time)
{
	std::string temp = std::to_string(time);
	TextElement* tempT = static_cast<TextElement*>(m_timerText);
	tempT->SetText(temp.c_str());
}

InputManager * GameManager::GetInputManager()
{
	return myInput;
}

Graphics * GameManager::GetGraphicsManager()
{
	return myGraphics;
}

void GameManager::ShootBullets()
{
	if (myPlayer->getTimeLeft() <= 0)
	{
		float3 forward = float3{ myGraphics->GetCamera()->GetCamDirection().m128_f32[0], myGraphics->GetCamera()->GetCamDirection().m128_f32[1], myGraphics->GetCamera()->GetCamDirection().m128_f32[2] };
		float3 playerPos = { myPlayer->GetPhysicsComponent()->GetPosition().x, myPlayer->GetPhysicsComponent()->GetPosition().y + 2.5f, myPlayer->GetPhysicsComponent()->GetPosition().z };
		bullets.push_back(new Bullet());
		bullets[bullets.size() - 1]->Initialize(myDX->GetDevice(), "Assets/Sphere.mesh", forward * -1, playerPos);
		myPlayer->AddSound(new Sound((char*)"Gunshot.wav"));
		myPlayer->GetSounds()[myPlayer->GetSounds().size() - 1]->Initialize(window);
		myPlayer->GetSounds()[myPlayer->GetSounds().size() - 1]->PlayWaveFile();
		myPlayer->SetTimeLeft(myPlayer->GetCurrentGun()->GetFireRate());
	}
}

void GameManager::SpawnPickup(float3 pos)
{
	GameObject* newpickup = new GameObject();
	newpickup->Initialize("Assets/HealthPickup.mesh", myDX->GetDevice());
	newpickup->SetLifeTime(15.0f);
	newpickup->GetPhysicsComponent()->SetPosition({ pos.x, pos.y, pos.z });
	newpickup->AddCollider(newpickup->GetPhysicsComponent()->GetPosition(), 1.0f);
	Pickups.push_back(newpickup);
}

void GameManager::Update(float delta, float total)
{
	if (!GetUIManager()->m_mainMenu && !GetUIManager()->m_pauseMenu)
	{
		myGraphics->Update(myInput, delta);
		bool moving = false;
		if (myInput->GetKeyState((int)'W') || myInput->GetKeyState((int)'A') || myInput->GetKeyState((int)'S') || myInput->GetKeyState((int)'D'))
		{
			moving = true;
		}
		if (moving)
		{
			myPlayer->SetAnimationLower(1);
		}
		else
		{
			myPlayer->SetAnimationLower(0);
		}

		myPlayer->GetPhysicsComponent()->SetForward(float3{ myGraphics->GetCamera()->GetCharDirection().m128_f32[0], myGraphics->GetCamera()->GetCharDirection().m128_f32[1], myGraphics->GetCamera()->GetCharDirection().m128_f32[2] });

		if (myInput->GetKeyState((int)'1'))
		{
			myPlayer->SetCurrentGun(0);
		}
		if (myInput->GetKeyState((int)'2'))
		{
			myPlayer->SetCurrentGun(1);
		}
		if (myInput->GetKeyState((int)'3'))
		{
			myPlayer->SetCurrentGun(2);
		}
		if (myPlayer->getTimeLeft() >= 0)
		{
			myPlayer->SubTimeLeft(delta);
		}
		myPlayer->Update(delta);

		myEnemyManager->Update(delta, myPlayer);

		for (unsigned int i = 0; i < bullets.size(); i++)
		{
			bullets[i]->Update(delta);
			if (bullets[i]->Destroy() || bullets[i]->GetPhysicsComponent()->GetPosition().y <= 0.0f)
			{
				Bullet *temp;
				bullets[i]->Shutdown();
				temp = bullets[i];
				bullets.erase(bullets.begin() + i);
				delete temp;
				break;
			}


			for (unsigned int j = 0; j < myEnemyManager->GetEnemies().size(); j++)
			{
				if (DitanceFloat3(bullets[i]->GetPhysicsComponent()->GetPosition(), myEnemyManager->GetEnemies()[j]->GetPhysicsComponent()->GetPosition()) <= 2.0f)
				{
					if (MovingSphereToSphere(*bullets[i]->GetCollider(0), bullets[i]->GetPhysicsComponent()->GetVelocity(), *myEnemyManager->GetEnemies()[j]->GetCollider(0), delta))
					{
						bullets[i]->SetDestroy();
						myEnemyManager->GetEnemies()[j]->AddSound(new Sound((char*)"Assets/HitSound.wav"));
						myEnemyManager->GetEnemies()[j]->GetSounds()[myEnemyManager->GetEnemies()[j]->GetSounds().size() - 1]->Initialize(window);
						myEnemyManager->GetEnemies()[j]->GetSounds()[myEnemyManager->GetEnemies()[j]->GetSounds().size() - 1]->PlayWaveFile();
						myEnemyManager->GetEnemies()[j]->SubHealth(myPlayer->GetCurrentGun()->GetDamageAmount());
						if (myEnemyManager->GetEnemies()[j]->GetHealth() <= 0)
						{
							int chance = rand() % 100;
							if (chance < 25)
							{
								SpawnPickup(myEnemyManager->GetEnemies()[j]->GetPhysicsComponent()->GetPosition());
							}
							myEnemyManager->GetEnemies()[j]->SetDestroy();
#ifdef DEBUG
							std::cout << myEnemyManager->GetEnemyCount() << std::endl;
#endif // DEBUG

						}
					}
				}
			}
			for (size_t j = 0; j < Obstacles.size(); j++)
			{
				if (DitanceFloat3(bullets[i]->GetPhysicsComponent()->GetPosition(), Obstacles[j]->GetPhysicsComponent()->GetPosition()) <= 2.0f)
				{
					for (size_t k = 0; k < Obstacles[j]->GetColliders().size(); k++)
					{
						if (MovingSphereToSphere(*bullets[i]->GetCollider(0), bullets[i]->GetPhysicsComponent()->GetVelocity(), *Obstacles[j]->GetCollider(k), delta))
						{
							bullets[i]->SetDestroy();
						}
					}
				}
			}
		}
		for (size_t i = 0; i < Obstacles.size(); i++)
		{
			if (DitanceFloat3(Obstacles[i]->GetPhysicsComponent()->GetPosition(), myPlayer->GetPhysicsComponent()->GetPosition()) <= 2.0f)
			{
				for (size_t j = 0; j < Obstacles[i]->GetColliders().size(); j++)
				{
					if (SphereToAABB(*Obstacles[i]->GetCollider(j), myPlayer->GetAABB()))
					{

					}
				}
			}
		}
		for (size_t i = 0; i < Pickups.size(); i++)
		{
			Pickups[i]->Update(delta);
			if (Pickups[i]->GetLifeTime() <= 0)
			{
				Pickups.erase(Pickups.begin() + i);
				--i;
			}
		}
		for (size_t i = 0; i < Pickups.size(); i++)
		{
			if (DitanceFloat3(Pickups[i]->GetPhysicsComponent()->GetPosition(), myPlayer->GetPhysicsComponent()->GetPosition()) <= 3.0f)
			{
				if (SphereToAABB(*Pickups[i]->GetCollider(0), myPlayer->GetAABB()))
				{
					myPlayer->SetHealth(myPlayer->GetHealth() + 25);
					Pickups.erase(Pickups.begin() + i);
					--i;
				}
			}
		}
		myInput->GetMouseInput()->Acquire();
		myInput->SetCurrMouseState();
		myGraphics->Update();
		UpdateScoreText();
		UpdateHealthText();
		UpdateWeaponText();
		//UpdateTimerText(total);

		if (GetHealth() <= 0)
		{
			if (!m_YouLose)
			{
				m_YouLose = GetUIManager()->CreateText(RECT{ 0,0,0,0 }, false, true, float2{ 640,360 }, F_ARIAL, "YOU LOSE!!!");
			}
		}
		if (GetEnemies() <= 0)
		{
			if (!m_YouWin)
			{
				m_YouWin = GetUIManager()->CreateText(RECT{ 0,0,0,0 }, false, true, float2{ 640,360 }, F_ARIAL, "YOU WIN!!!");
			}
		}
	}
}

bool GameManager::Render()
{
	//return myGraphics->Render(myInput, myPlayer, bullets, myTargets);
	return myGraphics->Render(myInput, myPlayer, bullets, myEnemyManager->GetEnemies(), Obstacles, Pickups);
}

bool GameManager::Initialize(int windowWidth, int windowHeight, HWND window)
{

	bool result = myGraphics->Initialize(windowWidth, windowHeight, window, myInput);

	myDX = myGraphics->GetGraphicsEngine();

	myPlayer = new Player();
	myPlayer->Initialize("Assets/Teddy_Idle.mesh", myDX->GetDevice());
	myPlayer->AddAninimation("Assets/Teddy_Idle.anim", myDX->GetDevice(), 0);
	myPlayer->AddAninimation("Assets/Teddy_Run.anim", myDX->GetDevice(), 1);
	myPlayer->GetPhysicsComponent()->SetVelocity(float3{ 0, 1.5, 0 });
	myPlayer->GetPhysicsComponent()->SetAccel(float3{ 0, -0.50, 0 });
	myPlayer->GetPhysicsComponent()->SetMass(50);
	myPlayer->GetPhysicsComponent()->SetDamping(.99f);
	myPlayer->SetAnimationUpper(0);
	Gun* Pistol = new Gun();
	Pistol->SetGunClass(Gun::PISTOL);
	Pistol->SetFireRate(0.5f);
	Pistol->SetDamageAmount(25);
	myPlayer->AddGun(Pistol);
	Gun* MachineGun = new Gun();
	MachineGun->SetGunClass(Gun::MACHINE);
	MachineGun->SetFireRate(0.3f);
	MachineGun->SetDamageAmount(35);
	myPlayer->AddGun(MachineGun);
	Gun* SubMachineGun = new Gun();
	SubMachineGun->SetGunClass(Gun::SUBMACHINE);
	SubMachineGun->SetFireRate(0.15f);
	SubMachineGun->SetDamageAmount(30);
	myPlayer->AddGun(SubMachineGun);
	myPlayer->SetCurrentGun(0);
	if (!myPlayer)
	{
		return false;
	}
	myEnemyManager->Initialize(myDX->GetDevice());

	unsigned int ObstaclesCount = rand() % 10 + 5;
	for (unsigned int i = 0; i < ObstaclesCount; i++)
	{
		Obstacles.push_back(new GameObject());
		Obstacles[i]->Initialize("Assets/Obstacle.mesh", myDX->GetDevice());
		Obstacles[i]->GetPhysicsComponent()->SetPosition({(float)(rand() % 50 - 25), 0, (float)(rand() % 50 - 25)});
		Obstacles[i]->AddCollider({ Obstacles[i]->GetPhysicsComponent()->GetPosition().x, Obstacles[i]->GetPhysicsComponent()->GetPosition().y + 1.0f, Obstacles[i]->GetPhysicsComponent()->GetPosition().z }, 1.0f);
		Obstacles[i]->AddCollider({ Obstacles[i]->GetPhysicsComponent()->GetPosition().x, Obstacles[i]->GetPhysicsComponent()->GetPosition().y + 4.0f, Obstacles[i]->GetPhysicsComponent()->GetPosition().z }, 1.0f);
		Obstacles[i]->AddCollider({ Obstacles[i]->GetPhysicsComponent()->GetPosition().x, Obstacles[i]->GetPhysicsComponent()->GetPosition().y + 7.0f, Obstacles[i]->GetPhysicsComponent()->GetPosition().z }, 1.0f);
	}

	this->window = window;
	return result;
}

void GameManager::ShutDown()
{
	//Cleans up the Graphics object
	if (myGraphics)
	{
		myGraphics->Shutdown();
		delete myGraphics;
		myGraphics = 0;
	}

	//Cleans up the input object
	if (myInput)
	{
		delete myInput;
		myInput = 0;
	}

	if (myPlayer)
	{
		myPlayer->Shutdown();
		delete myPlayer;
		myPlayer = nullptr;
	}

	if (myEnemyManager)
	{
		myEnemyManager->Shutdown();
		delete myEnemyManager;
		myEnemyManager = nullptr;
	}
	
}
