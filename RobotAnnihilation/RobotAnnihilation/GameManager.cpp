#include "GameManager.h"



GameManager::GameManager()
{
	myInput = new InputManager();
	myGraphics = new Graphics(myInput);
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
	_itoa_s(GetGraphicsManager()->GetEnemies(), displayString, 65, 10);
	TextElement* temp = dynamic_cast<TextElement*>(m_scoreText);
	temp->SetText((const char*)displayString);
}

void GameManager::UpdateHealthText()
{
	memset(displayString1, '\0', sizeof(displayString1));
	_itoa_s(GetGraphicsManager()->GetHealth(), displayString1, 65, 10);
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

void GameManager::Update(float delta)
{
	myInput->GetMouseInput()->Acquire();
	myInput->SetCurrMouseState();
	myGraphics->Update(myInput, delta);
	for (unsigned int i = 0; i < GameObjects.size(); i++)
	{
		// Uncomment when GameObjects have been implemented
		//GameObjects[i].Update(delta);
	}

	myGraphics->Update();

	UpdateScoreText();
	UpdateHealthText();

	if (myGraphics->GetEnemies() <= 0)
	{
		m_YouWin = myGraphics->GetUIManager()->CreateText(RECT{ 0,0,0,0 }, false, true, float2{ 50,50 }, F_ARIAL, "You WIN");
		GetUIManager()->GetUIElements()[4]->SetEnabled(true);
	}
	if (myGraphics->GetHealth() <= 0)
	{
		m_YouLose = myGraphics->GetUIManager()->CreateText(RECT{ 0,0,0,0 }, false, true, float2{ 50,50 }, F_ARIAL, "You WIN");
		GetUIManager()->GetUIElements()[4]->SetEnabled(true);
	}
}

bool GameManager::Render()
{
	return myGraphics->Render(myInput);
}

bool GameManager::Initialize(int windowWidth, int windowHeight, HWND window)
{
	return myGraphics->Initialize(windowWidth, windowHeight, window, myInput);
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

}
