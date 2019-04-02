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
	memset(displayString1, '\0', sizeof(displayString1));
	_itoa_s(myPlayer->GetHealth(), displayString1, 65, 10);
	TextElement* temp = dynamic_cast<TextElement*>(m_healthText);
	temp->SetText((const char*)displayString1);
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

void GameManager::Update(float delta)
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
					//std::cout << "Boom, Collision!" << std::endl;
					bullets[i]->SetDestroy();
					myEnemyManager->GetEnemies()[j]->SetDestroy();
					std::cout << myEnemyManager->GetEnemyCount() << std::endl;
				}
			}
		}
	}
	myInput->GetMouseInput()->Acquire();
	myInput->SetCurrMouseState();
	for (unsigned int i = 0; i < GameObjects.size(); i++)
	{
		// Uncomment when GameObjects have been implemented
		//GameObjects[i].Update(delta);
	}

	myGraphics->Update();

	UpdateScoreText();
	UpdateHealthText();

	if (myEnemyManager->GetEnemyCount() <= 0)
	{
		m_YouWin = myGraphics->GetUIManager()->CreateText(RECT{ 0,0,0,0 }, false, true, float2{ 50,50 }, F_ARIAL, "You WIN");
		GetUIManager()->GetUIElements()[4]->SetEnabled(true);
	}
	if (myPlayer->GetHealth() <= 0)
	{
		m_YouLose = myGraphics->GetUIManager()->CreateText(RECT{ 0,0,0,0 }, false, true, float2{ 50,50 }, F_ARIAL, "You WIN");
		GetUIManager()->GetUIElements()[4]->SetEnabled(true);
	}
}

bool GameManager::Render()
{
	return myGraphics->Render(myInput, myPlayer, bullets, myEnemyManager->GetEnemies());
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
	Pistol->SetFireRate(0.5f);
	Pistol->SetDamageAmount(25);
	myPlayer->AddGun(Pistol);
	Gun* MachineGun = new Gun();
	MachineGun->SetFireRate(0.3f);
	MachineGun->SetDamageAmount(35);
	myPlayer->AddGun(MachineGun);
	Gun* SubMachineGun = new Gun();
	SubMachineGun->SetFireRate(0.15f);
	SubMachineGun->SetDamageAmount(30);
	myPlayer->AddGun(SubMachineGun);
	myPlayer->SetCurrentGun(0);
	if (!myPlayer)
	{
		return false;
	}
	myEnemyManager->Initialize(myDX->GetDevice());
	this->window = window;
	return result;}

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
