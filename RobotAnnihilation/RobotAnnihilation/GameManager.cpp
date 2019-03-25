#include "GameManager.h"



GameManager::GameManager()
{
	myInput = new InputManager();
	myGraphics = new Graphics();
	myUiManager = new UI::UIManager(myGraphics, myInput);
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

InputManager * GameManager::GetInputManager()
{
	return myInput;
}

Graphics * GameManager::GetGraphicsManager()
{
	return myGraphics;
}

UI::UIManager * GameManager::GetUIManager()
{
	return myUiManager;
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
}

bool GameManager::Render()
{
	return myGraphics->Render(myInput);
}

bool GameManager::Initialize(int windowWidth, int windowHeight, HWND window)
{
	return myGraphics->Initialize(windowWidth, windowHeight, window);
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
	//Cleans up the UI object
	if (myUiManager)
	{
		delete myUiManager;
		myUiManager = 0;
	}
}
void GameManager::CreateImage(RECT dimensions, bool interact, const char * filePath, float2 pos)
{
	myUiManager->CreateImage(dimensions, interact, filePath, pos);
}