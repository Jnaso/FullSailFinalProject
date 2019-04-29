#include "GameManager.h"

GameManager::GameManager()
{
	myInput = new InputManager();
	myGraphics = new Graphics(myInput);
	myPlayer = nullptr;
	//myEnemyManager = new EnemyManager();
}


void GameManager::GameEnd()
{
	isDone = true;
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
	std::string enemyText = "Enemies Left: " + std::to_string(myEnemyManager->GetEnemyCount());
	TextElement* temp = dynamic_cast<TextElement*>(m_scoreText);
	temp->SetText(enemyText);
}

void GameManager::UpdateHealthText()
{
	std::string healthText = "Health: " + std::to_string(myPlayer->GetHealth());
	TextElement* temp = dynamic_cast<TextElement*>(m_healthText);
	temp->SetText(healthText);
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
	TextElement* tempT = static_cast<TextElement*>(m_timerText);
	tempT->SetText( "Total Time: " + std::to_string(static_cast<int>(time)) );
}

void GameManager::UpdateCurrencyText()
{
	TextElement* tempT = static_cast<TextElement*>(m_Currency);
	tempT->SetText( "Points: " + std::to_string(static_cast<int>(myPlayer->GetPoints())) );
}

void GameManager::UpdateDamageTimerText()
{
	TextElement* tempT = static_cast<TextElement*>(m_damagetimerText);
	tempT->SetText("Damage Increase: " + std::to_string(static_cast<int>(myPlayer->GetTimeDamage())));
}

void GameManager::SetLowHealthImage(bool val)
{
	m_lowHealthImage->SetEnabled(val);
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
		bullets[bullets.size() - 1]->Initialize(myDX->GetDevice(), "Assets/Sphere.mesh", forward * -1, playerPos, "Player");
		myPlayer->AddSound(new Sound((char*)"Gunshot.wav"));
		myPlayer->GetSounds()[myPlayer->GetSounds().size() - 1]->Initialize(window);
		myPlayer->GetSounds()[myPlayer->GetSounds().size() - 1]->PlayWaveFile();
		myPlayer->SetTimeLeft(myPlayer->GetCurrentGun()->GetFireRate());
	}
}

void GameManager::SpawnHealthPickup(float3 pos)
{
	Pickup* newpickup = new Pickup();
	newpickup->Initialize("Assets/HealthPickup.mesh", myDX->GetDevice());
	newpickup->SetLifeTime(15.0f);
	newpickup->SetType(Pickup::PickupType::HEALTH);
	newpickup->GetPhysicsComponent()->SetPosition({ pos.x, pos.y, pos.z });
	newpickup->AddCollider(newpickup->GetPhysicsComponent()->GetPosition(), 1.0f);
	Pickups.push_back(newpickup);
}

void GameManager::SpawnDamagePickup(float3 pos)
{
	Pickup* newpickup = new Pickup();
	newpickup->Initialize("Assets/DamagePickup.mesh", myDX->GetDevice());
	newpickup->SetLifeTime(15.0f);
	newpickup->SetType(Pickup::PickupType::DAMAGE);
	newpickup->GetPhysicsComponent()->SetPosition({ pos.x, pos.y, pos.z });
	newpickup->AddCollider(newpickup->GetPhysicsComponent()->GetPosition(), 1.0f);
	Pickups.push_back(newpickup);
}

void GameManager::ExitLevel()
{
	if (myGraphics)
	{
		myGraphics->ExitLevel();
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

	Obstacles.clear();

	/*if (myShop)
	{
		delete myShop;
		myShop = nullptr;
	}*/
}

void GameManager::EndRound()
{
	for (unsigned int i = 0; i < myEnemyManager->GetEnemies().size(); i++)
	{
		myEnemyManager->GetEnemies()[i]->SetDestroy();
	}
}

void GameManager::Update(float delta, float total)
{
	keyPressTimer -= delta;
	if (!GetUIManager()->m_mainMenu && !GetUIManager()->m_pauseMenu)
	{
		myGraphics->Update(myInput, delta, myPlayer);
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

		//#pragma region Shop_Controls
		//if (myInput->GetKeyState('K'))
		//{
		//	shopVisible = !shopVisible;
		//	myShop->SetShopVisibility(!shopVisible);
		//	paused = !paused;
		//	myInput->SetKetState('K', false);
		//}
		//myShop->Update();
		//#pragma endregion


		if (myPlayer->getTimeLeft() >= 0)
		{
			myPlayer->SubTimeLeft(delta);
		}
		if (myInput->GetCurrMouseState().rgbButtons[1])
		{
			myPlayer->MeleeAttack(2);
		}
		if (myInput->GetCurrMouseState().rgbButtons[0] && (!myGraphics->GetUIManager()->m_mainMenu && !myGraphics->GetUIManager()->m_pauseMenu) && !myPlayer->isAttacking())
		{
			ShootBullets();
		}
		myPlayer->Update(delta);

		myEnemyManager->Update(delta, myPlayer, Obstacles, bullets, myDX->GetDevice(), window);

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

			if (DitanceFloat3(bullets[i]->GetPhysicsComponent()->GetPosition(), myPlayer->GetPhysicsComponent()->GetPosition()) <= 3.0f && bullets[i]->GetTag() == "Enemy")
			{
				if (BetterSphereToAABB(*bullets[i]->GetCollider(0), myPlayer->GetAABB()))
				{				
					//std::cout << "Boom, Boom, Boom, Boom, Boom, Boom, Boom, Boom" << std::endl;
					bullets[i]->SetDestroy();
					if (!myPlayer->GetInvincible())
					{
						myPlayer->SetHealth(myPlayer->GetHealth() - 10.0f);
					}
				}
			}

			BombEnemy *currBomb;

			for (unsigned int j = 0; j < myEnemyManager->GetEnemies().size(); j++)
			{
				currBomb = dynamic_cast<BombEnemy*>(myEnemyManager->GetEnemies()[j]);
				if (DitanceFloat3(bullets[i]->GetPhysicsComponent()->GetPosition(), myEnemyManager->GetEnemies()[j]->GetPhysicsComponent()->GetPosition()) <= 2.0f && bullets[i]->GetTag() == "Player")
				{
					if (MovingSphereToSphere(*bullets[i]->GetCollider(0), bullets[i]->GetPhysicsComponent()->GetVelocity(), *myEnemyManager->GetEnemies()[j]->GetCollider(1), delta))
					{
						bullets[i]->SetDestroy();
						myEnemyManager->AddSound(new Sound((char*)"Assets/Headshot.wav", -1000));
						myEnemyManager->GetSounds()[myEnemyManager->GetSounds().size() - 1]->Initialize(window);
						myEnemyManager->GetSounds()[myEnemyManager->GetSounds().size() - 1]->PlayWaveFile();

						if (currBomb)
						{
							currBomb->Attack(myPlayer, myEnemyManager->GetEnemies(), window);
						}

						if (myPlayer->GetTimeDamage() > 0)
						{
							myEnemyManager->GetEnemies()[j]->SubHealth(myPlayer->GetCurrentGun()->GetDamageAmount() * 1.5f * 1.3f, Target::DamageType::Gun, window);
							myEnemyManager->GetEnemies()[j]->SetHurt();
						}
						else
						{
							myEnemyManager->GetEnemies()[j]->SubHealth(myPlayer->GetCurrentGun()->GetDamageAmount() * 1.3f, Target::DamageType::Gun, window);						
							myEnemyManager->GetEnemies()[j]->SetHurt();
						}
						if (myEnemyManager->GetEnemies()[j]->GetHealth() <= 0)
						{
							myEnemyManager->AddSound(new Sound((char*)"Assets/Explosion.wav", 0));
							myEnemyManager->GetSounds()[myEnemyManager->GetSounds().size() - 1]->Initialize(window);
							myEnemyManager->GetSounds()[myEnemyManager->GetSounds().size() - 1]->PlayWaveFile();
							int chance = rand() % 100;
							if (chance < 25)
							{
								if (chance < 10)
								{
									SpawnDamagePickup(myEnemyManager->GetEnemies()[j]->GetPhysicsComponent()->GetPosition());
								}
								else
								{
									SpawnHealthPickup(myEnemyManager->GetEnemies()[j]->GetPhysicsComponent()->GetPosition());
								}
							}
							myPlayer->AddCurrency(myEnemyManager->GetEnemies()[j]->GetCurrency() * 1.5f);
							myEnemyManager->GetEnemies()[j]->SetDestroy();
						}
					}
					else if (MovingSphereToSphere(*bullets[i]->GetCollider(0), bullets[i]->GetPhysicsComponent()->GetVelocity(), *myEnemyManager->GetEnemies()[j]->GetCollider(0), delta))
					{
						bullets[i]->SetDestroy();
						
						if (currBomb)
						{
							currBomb->Attack(myPlayer, myEnemyManager->GetEnemies(), window);
						}

						if (myPlayer->GetTimeDamage() > 0)
						{
							myEnemyManager->GetEnemies()[j]->SubHealth(myPlayer->GetCurrentGun()->GetDamageAmount() * 1.5f, Target::DamageType::Gun, window);
							myEnemyManager->AddSound(new Sound((char*)"Assets/HitSound.wav", -1000));
							myEnemyManager->GetSounds()[myEnemyManager->GetSounds().size() - 1]->Initialize(window);
							myEnemyManager->GetSounds()[myEnemyManager->GetSounds().size() - 1]->PlayWaveFile();
							myEnemyManager->GetEnemies()[j]->SetHurt();
						}
						else
						{
							myEnemyManager->GetEnemies()[j]->SubHealth(myPlayer->GetCurrentGun()->GetDamageAmount(), Target::DamageType::Gun, window);
							myEnemyManager->AddSound(new Sound((char*)"Assets/HitSound.wav", -1000));
							myEnemyManager->GetSounds()[myEnemyManager->GetSounds().size() - 1]->Initialize(window);
							myEnemyManager->GetSounds()[myEnemyManager->GetSounds().size() - 1]->PlayWaveFile();
							myEnemyManager->GetEnemies()[j]->SetHurt();
						}

						if (myEnemyManager->GetEnemies()[j]->GetHealth() <= 0)
						{
							myEnemyManager->AddSound(new Sound((char*)"Assets/Explosion.wav", -1000));
							myEnemyManager->GetSounds()[myEnemyManager->GetSounds().size() - 1]->Initialize(window);
							myEnemyManager->GetSounds()[myEnemyManager->GetSounds().size() - 1]->PlayWaveFile();
							int chance = rand() % 100;
							if (chance < 25)
							{								
								if (chance < 10)
								{
									SpawnDamagePickup(myEnemyManager->GetEnemies()[j]->GetPhysicsComponent()->GetPosition());
								}
								else
								{
									SpawnHealthPickup(myEnemyManager->GetEnemies()[j]->GetPhysicsComponent()->GetPosition());
								}
							}
							myPlayer->AddCurrency(myEnemyManager->GetEnemies()[j]->GetCurrency());
							myEnemyManager->GetEnemies()[j]->SetDestroy();

						}
					}
				}
			}
			for (size_t j = 0; j < Obstacles.size(); j++)
			{
				for (size_t k = 0; k < Obstacles[j]->GetColliders().size(); k++)
				{
					if (DitanceFloat3(bullets[i]->GetPhysicsComponent()->GetPosition(), Obstacles[j]->GetCollider(k)->center) <= (bullets[i]->GetCollider(0)->radius + Obstacles[j]->GetCollider(k)->radius))
					{				
						if (MovingSphereToSphere(*bullets[i]->GetCollider(0), bullets[i]->GetPhysicsComponent()->GetVelocity(), *Obstacles[j]->GetCollider(k), delta))
						{
							bullets[i]->SetDestroy();
						}
					}
				}
			}
		}

		myPlayer->SetForward(false);
		myPlayer->SetBackward(false);
		myPlayer->SetLeft(false);
		myPlayer->SetRight(false);
		for (size_t i = 0; i < Obstacles.size(); i++)
		{
			if (DitanceFloat3(Obstacles[i]->GetPhysicsComponent()->GetPosition(), myPlayer->GetPhysicsComponent()->GetPosition()) <= 8.0f)
			{
				for (size_t j = 0; j < Obstacles[i]->GetColliders().size(); j++)
				{
					if (lineCircle(myPlayer->GetForwardArrow(), *Obstacles[i]->GetCollider(j)))
					{
						//std::cout << "BoomBoom" << std::endl;
						myPlayer->SetForward(true);
					}

					if (lineCircle(myPlayer->GetBackwardArrow(), *Obstacles[i]->GetCollider(j)))
					{
						//std::cout << "BoomBoom" << std::endl;
						myPlayer->SetBackward(true);
					}

					if (lineCircle(myPlayer->GetLeftArrow(), *Obstacles[i]->GetCollider(j)))
					{
						//std::cout << "BoomBoom" << std::endl;
						myPlayer->SetLeft(true);
					}

					if (lineCircle(myPlayer->GetRightArrow(), *Obstacles[i]->GetCollider(j)))
					{
						//std::cout << "BoomBoom" << std::endl;
						myPlayer->SetRight(true);
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
				continue;
			}
			if (DitanceFloat3(Pickups[i]->GetPhysicsComponent()->GetPosition(), myPlayer->GetPhysicsComponent()->GetPosition()) <= 3.0f)
			{
				if (SphereToAABB(*Pickups[i]->GetCollider(0), myPlayer->GetAABB()))
				{					
					if (Pickups[i]->GetType() == Pickup::PickupType::DAMAGE)
					{
						myPlayer->AddSound(new Sound((char*)"Assets/DamagePickup.wav", 0));
						myPlayer->GetSounds()[myPlayer->GetSounds().size() - 1]->Initialize(window);
						myPlayer->GetSounds()[myPlayer->GetSounds().size() - 1]->PlayWaveFile();
						myPlayer->SetTimeDamage(30.0f);
					}
					if (Pickups[i]->GetType() == Pickup::PickupType::HEALTH)
					{
						myPlayer->AddSound(new Sound((char*)"Assets/Boost.wav", 0));
						myPlayer->GetSounds()[myPlayer->GetSounds().size() - 1]->Initialize(window);
						myPlayer->GetSounds()[myPlayer->GetSounds().size() - 1]->PlayWaveFile();
						myPlayer->SetHealth(myPlayer->GetHealth() + 25);
					}
					Pickups.erase(Pickups.begin() + i);
					--i;
					continue;
				}
			}
		}
		for (size_t i = 0; i < myEnemyManager->GetEnemies().size(); i++)
		{
			if (DitanceFloat3(myEnemyManager->GetEnemies()[i]->GetPhysicsComponent()->GetPosition(), myPlayer->GetPhysicsComponent()->GetPosition()) < 5.0f && myPlayer->isAttacking())
			{
				float3 dot = myEnemyManager->GetEnemies()[i]->GetPhysicsComponent()->GetPosition() - myPlayer->GetPhysicsComponent()->GetPosition();
				dot.normalize();
				float result = DotProduct(dot, myPlayer->GetPhysicsComponent()->GetForward() * -1);
				if (result > 0.7f)
				{
				
					if (myPlayer->GetTimeDamage() > 0)
					{
						myEnemyManager->GetEnemies()[i]->SubHealth(myPlayer->GetCurrentGun()->GetDamageAmount() * 1.5f, Target::DamageType::Melee, window);
					}
					else
					{
						myEnemyManager->GetEnemies()[i]->SubHealth(myPlayer->GetCurrentGun()->GetDamageAmount(), Target::DamageType::Melee, window);
					}
					if (myEnemyManager->GetEnemies()[i]->GetHealth() <= 0)
					{
						int chance = rand() % 100;
						if (chance < 25)
						{
							
							if (chance < 10)
							{
								SpawnDamagePickup(myEnemyManager->GetEnemies()[i]->GetPhysicsComponent()->GetPosition());
							}
							else
							{
								SpawnHealthPickup(myEnemyManager->GetEnemies()[i]->GetPhysicsComponent()->GetPosition());
							}
						}
						myEnemyManager->GetEnemies()[i]->SetDestroy();

						myPlayer->AddCurrency(myEnemyManager->GetEnemies()[i]->GetCurrency());

					}
					float3 dir = myEnemyManager->GetEnemies()[i]->GetPhysicsComponent()->GetPosition() - myPlayer->GetPhysicsComponent()->GetPosition();
					dir.normalize();
				}
			}
		}
		myInput->GetMouseInput()->Acquire();
		myInput->SetCurrMouseState();
		myGraphics->Update();
		UpdateScoreText();
		UpdateHealthText();
		UpdateWeaponText();
		if (myPlayer->GetTimeDamage() > 0)
		{
			UpdateDamageTimerText();
		}
		UpdateTimerText(total);
		UpdateCurrencyText();
		if (myPlayer->GetTimeDamage() <= 0 && m_damagetimerText->GetEnabled())
		{
			myPlayer->AddSound(new Sound((char*)"Assets/DamagePickupEnd.wav", 0));
			myPlayer->GetSounds()[myPlayer->GetSounds().size() - 1]->Initialize(window);
			myPlayer->GetSounds()[myPlayer->GetSounds().size() - 1]->PlayWaveFile();
			m_damagetimerText->SetEnabled(false);
		}
		if (myPlayer->GetTimeDamage() > 0 && !m_damagetimerText->GetEnabled())
		{
			m_damagetimerText->SetEnabled(true);
		}

		if (GetHealth() <= 0)
		{
			if (!m_YouLose)
			{
				m_YouLose = GetUIManager()->CreateText(RECT{ 0,0,0,0 }, false, true, float2{ 640,360 }, F_ARIAL, "YOU LOSE!!!");
				paused = true;
			}
			if (!m_youLoseQuitButton)
			{
				m_youLoseQuitButton = GetUIManager()->CreateButton(RECT{ 0,0,0,0 }, true, true, float2{ 0,0 }, GetGraphicsManager()->GetGraphicsEngine()->GetDevice(), F_ARIAL, "QUIT");
				ButtonElement* lqb = static_cast<ButtonElement*>(m_youLoseQuitButton);
				if (lqb)
				{
					lqb->SetDefaultTexture("DrawingStuff/ButtonDefault.dds");
					lqb->SetMouseOverTexture("DrawingStuff/ButtonMouseOver.dds");
					lqb->SetMouseClickTexture("DrawingStuff/ButtonMouseClick.dds");
					lqb->m_OnMouseClick = [this]()
					{
						GameEnd();
						
					};
					lqb->SetSize(100, 50);
					lqb->SetPos((screenW * 0.5f) - (lqb->GetSize().x * 0.5f), (screenH * 0.5f) - (lqb->GetSize().y * 0.5f));
				}
			}
			GetPlayer()->SetHealth(0);
		}
		if (GetHealth() >= 0 && GetHealth() <= 100)
		{
			m_lowHealthImage->SetEnabled(true);
		}
		else
		{
			if(m_lowHealthImage->GetEnabled()) m_lowHealthImage->SetEnabled(false);
		}

		if (GetEnemies() <= 0 && !betweenRounds)
		{
			countDown = 5.0f;
			betweenRounds = true;
		}
		if (countDown > 0.0f && betweenRounds)
		{
			countDown -= delta;
			if (countDown < 0.0f)
			{
				myEnemyManager->StartNewRound();
				betweenRounds = false;
				countDown = 0;
				currentRound++;
			}
		}
	}
}

bool GameManager::Render()
{
	if (myEnemyManager)
	{
		return myGraphics->Render(myInput, myPlayer, bullets, myEnemyManager->GetEnemies(), Obstacles, Pickups);
	}
	else
	{
		myGraphics->RenderOnlyUI();
		return true;
	}
	
}

bool GameManager::Initialize(int windowWidth, int windowHeight, HWND window)
{
	bool result = myGraphics->Initialize(windowWidth, windowHeight, window, myInput);

	myDX = myGraphics->GetGraphicsEngine();

	if (!myEnemyManager)
	{
		myEnemyManager = new EnemyManager();
	}

	myPlayer = new Player();
	myPlayer->Initialize("Assets/Teddy_Idle.mesh", myDX->GetDevice());
	myPlayer->AddAninimation("Assets/Teddy_Idle.anim", myDX->GetDevice(), 0);
	myPlayer->AddAninimation("Assets/Teddy_Run.anim", myDX->GetDevice(), 1);
	myPlayer->AddAninimation("Assets/Teddy_Attack1.anim", myDX->GetDevice(), 2);
	myPlayer->GetPhysicsComponent()->SetVelocity(float3{ 0, 1.5, 0 });
	myPlayer->GetPhysicsComponent()->SetAccel(float3{ 0, -0.50, 0 });
	myPlayer->GetPhysicsComponent()->SetMass(50);
	myPlayer->GetPhysicsComponent()->SetDamping(.99f);
	myPlayer->SetAnimationUpper(1);
	Gun* Pistol = new Gun();
	Pistol->SetGunClass(Gun::PISTOL);
	Pistol->SetFireRate(0.5f);
	Pistol->SetDamageAmount(25);
	myPlayer->AddGun(Pistol);
	//Gun* MachineGun = new Gun();
	//MachineGun->SetGunClass(Gun::MACHINE);
	//MachineGun->SetFireRate(0.3f);
	//MachineGun->SetDamageAmount(35);
	//myPlayer->AddGun(MachineGun);
	//Gun* SubMachineGun = new Gun();
	//SubMachineGun->SetGunClass(Gun::SUBMACHINE);
	//SubMachineGun->SetFireRate(0.25f);
	//SubMachineGun->SetDamageAmount(30);
	//myPlayer->AddGun(SubMachineGun);
	myPlayer->SetCurrentGun(0);
	if (!myPlayer)
	{
		return false;
	}
	myEnemyManager->Initialize(myDX->GetDevice());
	currentRound++;

	playerSphere.center = { myPlayer->GetPhysicsComponent()->GetPosition().x, myPlayer->GetPhysicsComponent()->GetPosition().y + 2.0f, myPlayer->GetPhysicsComponent()->GetPosition().z };
	playerSphere.radius = 1.0f;

	unsigned int ObstaclesCount = rand() % 10 + 5;
	for (unsigned int i = 0; i < ObstaclesCount; i++)
	{
		Obstacles.push_back(new GameObject());
		Obstacles[i]->Initialize("Assets/Obstacle.mesh", myDX->GetDevice());
		Obstacles[i]->GetPhysicsComponent()->SetPosition({(float)(rand() % 50 - 25), 0, (float)(rand() % 50 - 25)});
		while (DitanceFloat3(Obstacles[i]->GetPhysicsComponent()->GetPosition(), myPlayer->GetPhysicsComponent()->GetPosition()) < 5.0f)
		{
			Obstacles[i]->GetPhysicsComponent()->SetPosition({ (float)(rand() % 50 - 25), 0, (float)(rand() % 50 - 25) });
		}
		Obstacles[i]->AddCollider({ Obstacles[i]->GetPhysicsComponent()->GetPosition().x, Obstacles[i]->GetPhysicsComponent()->GetPosition().y + 1.0f, Obstacles[i]->GetPhysicsComponent()->GetPosition().z }, 3.0f);
		Obstacles[i]->AddCollider({ Obstacles[i]->GetPhysicsComponent()->GetPosition().x, Obstacles[i]->GetPhysicsComponent()->GetPosition().y + 4.0f, Obstacles[i]->GetPhysicsComponent()->GetPosition().z }, 3.0f);
		Obstacles[i]->AddCollider({ Obstacles[i]->GetPhysicsComponent()->GetPosition().x, Obstacles[i]->GetPhysicsComponent()->GetPosition().y + 7.0f, Obstacles[i]->GetPhysicsComponent()->GetPosition().z }, 3.0f);
		Obstacles[i]->AddCollider({ Obstacles[i]->GetPhysicsComponent()->GetPosition().x, Obstacles[i]->GetPhysicsComponent()->GetPosition().y + 10.0f, Obstacles[i]->GetPhysicsComponent()->GetPosition().z }, 3.0f);
		Obstacles[i]->AddCollider({ Obstacles[i]->GetPhysicsComponent()->GetPosition().x, Obstacles[i]->GetPhysicsComponent()->GetPosition().y + 13.0f, Obstacles[i]->GetPhysicsComponent()->GetPosition().z }, 3.0f);
		
	}

	this->window = window;

	myShop = new Shop(GetUIManager(), myDX->GetDevice(), myPlayer);
	if (!myShop)
	{
		return false;
	}

	if (!myShop->Initialize())
	{
		return false;
	}

	m_lowHealthImage = GetUIManager()->CreateImage(RECT{ 0,0,0,0 }, false, false, float2{ 0,0 }, "DrawingStuff/almostDead.dds", GetGraphicsManager()->GetGraphicsEngine()->GetDevice());
	ImageElement* tempImg = static_cast<ImageElement*>(m_lowHealthImage);
	if (tempImg)
	{

		tempImg->SetSize(screenW, screenH);
	}

	screenW = windowWidth;
	screenH = windowHeight;

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

	if (myShop)
	{
		delete myShop;
		myShop = nullptr;
	}

	for (unsigned int i = 0; i < Obstacles.size(); i++)
	{
		Obstacles[i]->Shutdown();
		delete Obstacles[i];
	}

	Obstacles.clear();
}
