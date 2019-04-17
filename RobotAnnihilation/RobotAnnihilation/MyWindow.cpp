#include "MyWindow.h"

MyWindow::MyWindow()
{
	gameManager = nullptr;
}

bool MyWindow::Run()
{
	bool result;
	static int frameCounter = 0;
	frameCounter++;
	if (frameCounter==10)
	{
		ShowFPS();
		frameCounter = 0;
	}

	timer.Signal();

	//Subtract Time So Input Isnt checked every frame
	keyPressTimer -= timer.Delta();
	
	if (!paused)
	{
		Update(timer.Delta());
	}
	gameManager->GetUIManager()->Update(timer.Delta());

	//If user presses escape, close the window 
	//if (gameManager->GetKeyState(_ESCAPE))
	//{
	//	return false;
	//}


		if (gameManager->GetKeyState(_ESCAPE))
		{
			paused = !paused;
			gameManager->GetShopPtr()->SetShopVisibility(false);
			SetPauseMenu(paused);
			gameManager->SetLowHealthImage(false);
			gameManager->SetKeyState(_ESCAPE, false);
			pauseMenuBool = paused;
		}
		
		if (gameManager->GetKeyState('L'))
		{
			showFPS = !showFPS;
			m_FPSText->SetEnabled(showFPS);
			gameManager->SetKeyState('L', false);
		}
		
		if (gameManager->GetKeyState((int)'9')) {
			gameManager->FlipInvincible();  gameManager->SetKeyState('9', false);
		}
		if (gameManager->GetKeyState((int)'8')) {
			gameManager->AddMoney();  gameManager->SetKeyState('8', false);
		}
		if (gameManager->GetKeyState((int)'7')) {
			gameManager->UnlockAllGuns(); gameManager->SetKeyState('7', false);
		}
		if (gameManager->GetKeyState((int)'6')) {
			gameManager->EndRound(); gameManager->SetKeyState('6', false);
		}
		if (gameManager->GetKeyState((int)'0')){
			gameManager->MaxHealth(); gameManager->SetKeyState('0', false);
		}

		if (keyPressTimer <= 0)
		{
			//Make Sure This Is On The Bottom!!!!
			keyPressTimer = DEFAULTKEYPRESST;
		}
	

	//Render every frame and stop if anything goes wrong 
	result = gameManager->Render();
	if (!result)
	{
		return false;
	}

	return true;
}

void MyWindow::CreateWindows(int &screenW, int &screenH)
{
	WNDCLASSEX wc;
	DEVMODE screenSettings;
	int posX, posY;

	ApplicationHandle = this;

	myInstance = GetModuleHandle(NULL);

	myAppName = L"RobotAnnihilation";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	// CS_HREDRAW: redraws everything if the width is adjusted in any way
	// CS_VREDRAW: redraws everything if the height is adjusted in any way
	// CS_OWNDC: Each window gets their own context
	wc.lpfnWndProc = WndProc;//Window Procedure(LRESULT CALLBACK)
	wc.cbClsExtra = 0;//Number of extra bytes to allocate 
	wc.cbWndExtra = 0;//Number of extra bytes to allocate 
	wc.hInstance = myInstance;//Window handle
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);//Window icon 
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);//Mouse cursor reference 
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);//Window color
	wc.lpszMenuName = NULL;//Class menu name
	wc.lpszClassName = myAppName;//Application name
	wc.cbSize = sizeof(WNDCLASSEX);//Byte size of the structure

	RegisterClassEx(&wc); //Register the window 

	screenW = GetSystemMetrics(SM_CXSCREEN); //System width
	screenH = GetSystemMetrics(SM_CYSCREEN);//System height

	if (FULL_SCREEN)
	{
		memset(&screenSettings, 0, sizeof(screenSettings));
		screenSettings.dmSize = sizeof(screenSettings);//Byte size 
		screenSettings.dmPelsWidth = (unsigned long)screenW;//Width of the window 
		screenSettings.dmPelsHeight = (unsigned long)screenH;//Height of the window
		screenSettings.dmBitsPerPel = 32;//Resolution 
		screenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;//You must specifiy which fields have been initialized

		ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN);//Set the full screen settings

		posX = posY = 0;//Set the window to the top left corner 
	}
	else
	{
		screenW = 1920;
		screenH = 1080;
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenW) / 2;//Middle of the x-axis
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenH) / 2;//Middle of the y - axis
	}

	myWindow = CreateWindowEx(WS_EX_APPWINDOW, myAppName, myAppName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenW, screenH, NULL, NULL, myInstance, NULL);



	//Display window 
	ShowWindow(myWindow, SW_SHOW);
	SetForegroundWindow(myWindow);
	SetFocus(myWindow);
}

void MyWindow::ShutdownWindows()
{
	ShowCursor(true);

	if (FULL_SCREEN)
	{
		//Remove the full screen 
		ChangeDisplaySettings(NULL, 0);
	}

	//Remove the window 
	DestroyWindow(myWindow);
	myWindow = NULL;

	//Free all window data 
	UnregisterClass(myAppName, myInstance);
	myInstance = NULL;

	ApplicationHandle = NULL;
}

ID3D11Device * MyWindow::GetDevice()
{
	return gameManager->GetGraphicsManager()->GetGraphicsEngine()->GetDevice();
}

void MyWindow::HideMainMenu()
{
	gameManager->GetUIManager()->m_mainMenu = false;
	for (unsigned int i = 0; i < 5; i++)
	{
		mainMenu[i]->SetEnabled(false);
		mainMenu[i]->SetInteractable(false);
	}
}

void MyWindow::ShowMainMenu()
{
	gameManager->GetUIManager()->m_mainMenu = true;
	for (unsigned int i = 0; i < 5; i++)
	{
		mainMenu[i]->SetEnabled(true);
		mainMenu[i]->SetInteractable(true);
	}
}

void MyWindow::SetPauseMenu(bool value)
{
	gameManager->GetUIManager()->m_pauseMenu = value;
	for (unsigned int i = 0; i < ARRAYSIZE(pauseMenu); i++)
	{
		pauseMenu[i]->SetEnabled(value);
	}
}

void MyWindow::GameIsDone()
{
	m_done = true;
}

void MyWindow::ShowPlayerUI()
{
	for (unsigned int i = 0; i < ARRAYSIZE(playerUI); i++)
	{
		playerUI[i]->SetEnabled(true);
	}
}

void MyWindow::HidePlayerUI()
{
	for (unsigned int i = 0; i < ARRAYSIZE(playerUI); i++)
	{
		playerUI[i]->SetEnabled(false);
	}
}

void MyWindow::SetOptionsMenu(bool val)
{
	for (int i = 0; i < ARRAYSIZE(optionsMenu); i++)
	{
		optionsMenu[i]->SetEnabled(val);
	}
}

void MyWindow::ShowFPS()
{	
	numberToChr = std::to_string((int)FPS);
	TextElement* tempT = static_cast<TextElement*>(m_FPSText);
	tempT->SetText("FPS: " + numberToChr);
}

void MyWindow::VolumeUp()
{
	//Volume Up
}

void MyWindow::VolumeDown()
{
	//Volume Down
}

void MyWindow::CalcFPS()
{
	static const int NUM_SAMPLES = 100;
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;
	static float prevTicks = GetTickCount();

	float currTicks = GetTickCount();

	_frameTime = currTicks - prevTicks;
	frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

	prevTicks = currTicks;

	int count;
	if (currentFrame < NUM_SAMPLES)
	{
		count = currentFrame;
	}
	else
	{
		count = NUM_SAMPLES;
	}
	float frameTimeAvg = 0;
	for (size_t i = 0; i < count; i++)
	{
		frameTimeAvg += frameTimes[i];
	}
	frameTimeAvg /= count;
	if (frameTimeAvg > 0)
	{
		FPS = 1000.0f / frameTimeAvg;
	}
	else
	{
		FPS = 60.0f;
	}

	currentFrame++;
}

#define BUTTONSIZE float2{200,50}
#define CENTERX screenW * 0.5f
#define CENTERY screenH * 0.5f

bool MyWindow::Initialize()
{
	int screenW, screenH;
	bool result;

	//Set up the window and the dimensions 
	CreateWindows(screenW, screenH);

	/*if (!myInput)
	{
		return false;
	}*/
	gameManager = new GameManager();
	if (!gameManager)
	{
		return false;
	}

	//if (!myGraphics)
	//{
	//	return false;
	//}
	//myInput = new InputManager();
	if (!gameManager->GetInputManager())
	{
		return false;
	}

	//myInput = new InputManager();
	if (!gameManager->GetInputManager()->Initialize(myInstance, myWindow))
	{
		return false;
	}

	////Set up for graphics object
	//myGraphics = new Graphics();
	if (!gameManager->GetGraphicsManager())
	{
		return false;
	}

	result = gameManager->Initialize(screenW, screenH, myWindow);
	if (!result)
	{
		return false;
	}
	
	//Constructors Take In Default Variables
	//Graphics Class calls both Update() and Render() for UIManager Class
	//UIManager Class deals with Update() and Render() for UIElements
	//Update() takes care of checking for MouseOver checking

	#pragma region UIElement Creation
	//ENABLE AFTER GAMEPLAY IMPLEMENTATION

	#pragma region Main_Menu
	UIElement* robotAnnText = gameManager->GetUIManager()->CreateText(RECT{ 0,0,0,0 }, false, true, float2{ 50, 50 }, F_COMICSANS, "Robot Annihilation");
	robotAnnText->SetPos(float2{ (screenW * 0.5f) - 50, robotAnnText->m_pos.y });
	mainMenu[0] = robotAnnText;

	//Images
	UIElement* mainMenuBkrnd = gameManager->GetUIManager()->CreateImage(RECT{ 0,0,0,0 }, false, true, float2{ 0,0 }, "DrawingStuff/MainMenu.dds", gameManager->GetGraphicsManager()->GetGraphicsEngine()->GetDevice());
	mainMenuBkrnd->SetSize(float2{ 1920, 1080 });
	mainMenuBkrnd->SetPos(float2{ 0.0f, 0.0f });
	mainMenu[1] = mainMenuBkrnd;

	UIElement* sButton = gameManager->GetUIManager()->CreateButton(RECT{ 0,0,0,0 }, true, true, float2{ 0,0 }, this->GetDevice(), 0, "Start");
	#pragma region Start_Button
	ButtonElement* startButton = static_cast<ButtonElement*>(sButton);
	if (startButton)
	{
		startButton->SetDefaultTexture("DrawingStuff/ButtonDefault.dds");
		startButton->SetMouseOverTexture("DrawingStuff/ButtonMouseOver.dds");
		startButton->SetMouseClickTexture("DrawingStuff/ButtonMouseClick.dds");
		//Lamda [Place Scope Here](Parameters){Code Here} 
		//Used To Set the function pointer in UIElement
		startButton->m_OnMouseClick = [this, screenW, screenH]()
		{
			if (!gameManager->GetEnemyMangerPtr())
			{
				gameManager->Initialize(screenW, screenH, myWindow);
			}
			timer.Restart();
			HideMainMenu();
			ShowPlayerUI();
			SetPaused(false);
			this->mainMenuBool = false;
		};

		startButton->SetSize(float2{ 200, 50 });
		startButton->SetPos(float2{ (screenW * 0.5f), (screenH * 0.5f) });
	}
	#pragma endregion
	mainMenu[2] = startButton;

	UIElement* oButton = gameManager->GetUIManager()->CreateButton(RECT{ 0,0,0,0 }, true, true, float2{ 0,0 }, this->GetDevice(), 0, "Options");
	#pragma region Options_Button
	ButtonElement* optionsButton = static_cast<ButtonElement*>(oButton);
	if (optionsButton)
	{
		optionsButton->SetDefaultTexture("DrawingStuff/ButtonDefault.dds");
		optionsButton->SetMouseOverTexture("DrawingStuff/ButtonMouseOver.dds");
		optionsButton->SetMouseClickTexture("DrawingStuff/ButtonMouseClick.dds");
		optionsButton->m_OnMouseClick = [this]()
		{
			//Open Options Menu
			HideMainMenu();
			SetOptionsMenu(true);
		};
		optionsButton->SetSize(float2{ 200, 50 });
		optionsButton->SetPos(float2{ (screenW * 0.5f), (screenH * 0.5f) + startButton->GetSize().y });
	}
	#pragma endregion
	mainMenu[3] = optionsButton;

	UIElement* qButton = gameManager->GetUIManager()->CreateButton(RECT{ 0,0,0,0 }, true, true, float2{ 0, 0 }, this->GetDevice(), 0, "Quit");
	#pragma region Quit_Button
	ButtonElement* quitButton1 = static_cast<ButtonElement*>(qButton);
	if (quitButton1)
	{
		quitButton1->SetDefaultTexture("DrawingStuff/ButtonDefault.dds");
		quitButton1->SetMouseOverTexture("DrawingStuff/ButtonMouseOver.dds");
		quitButton1->SetMouseClickTexture("DrawingStuff/ButtonMouseClick.dds");
		quitButton1->m_OnMouseClick = [this]()
		{
			GameIsDone();
		};
		quitButton1->SetSize(float2{ 200, 50 });
		quitButton1->SetPos(float2{ (screenW * 0.5f), (screenH * 0.5f) + (startButton->GetSize().y * 2) });
	}
	#pragma endregion  
	mainMenu[4] = quitButton1;
	#pragma endregion

	#pragma region Pause_Menu
	UIElement* rButton = gameManager->GetUIManager()->CreateButton(RECT{ 0,0,0,0 }, false, false, float2{ 0,0 }, this->GetDevice(), 0, "Resume");
	#pragma region Resume_Button
	ButtonElement* resumeButton = static_cast<ButtonElement*>(rButton);
	if (resumeButton)
	{
		resumeButton->SetDefaultTexture("DrawingStuff/ButtonDefault.dds");
		resumeButton->SetMouseOverTexture("DrawingStuff/ButtonMouseOver.dds");
		resumeButton->SetMouseClickTexture("DrawingStuff/ButtonMouseClick.dds");
		resumeButton->m_OnMouseClick = [this]()
		{
			this->SetPaused(false);
			SetPauseMenu(false);
		};
		resumeButton->SetSize(float2{ 200,50 });
		resumeButton->SetPos(float2{ screenW * 0.5f, screenH * 0.5f });
	}
	#pragma endregion
	pauseMenu[0] = rButton;

	UIElement* oButton1 = gameManager->GetUIManager()->CreateButton(RECT{ 0,0,0,0 }, false, false, float2{ 0,0 }, this->GetDevice(), 0, "Options");
	#pragma region Options_Button
	ButtonElement* optionsButton1 = static_cast<ButtonElement*>(oButton1);
	if (optionsButton1)
	{
		optionsButton1->SetDefaultTexture("DrawingStuff/ButtonDefault.dds");
		optionsButton1->SetMouseOverTexture("DrawingStuff/ButtonMouseOver.dds");
		optionsButton1->SetMouseClickTexture("DrawingStuff/ButtonMouseClick.dds");
		optionsButton1->m_OnMouseClick = [this]()
		{
			//Open Options Menu
			SetPauseMenu(false);
			SetOptionsMenu(true);
		};
		optionsButton1->SetSize(BUTTONSIZE);
		optionsButton1->SetPos(float2{ CENTERX, CENTERY + 50 });
	}
	#pragma endregion
	pauseMenu[1] = optionsButton1;

	UIElement* mButton = gameManager->GetUIManager()->CreateButton(RECT{ 0,0,0,0 }, false, false, float2{ 0,0 }, this->GetDevice(), 0, "Main Menu");
	#pragma region MainMenu_Button
	ButtonElement* mainMenuButton = static_cast<ButtonElement*>(mButton);
	if (mainMenuButton)
	{
		mainMenuButton->SetDefaultTexture("DrawingStuff/ButtonDefault.dds");
		mainMenuButton->SetMouseOverTexture("DrawingStuff/ButtonMouseOver.dds");
		mainMenuButton->SetMouseClickTexture("DrawingStuff/ButtonMouseClick.dds");
		mainMenuButton->m_OnMouseClick = [this]()
		{
			gameManager->ExitLevel();
			this->ShowMainMenu();
			this->SetPauseMenu(false);
			this->HidePlayerUI();
		};
		mainMenuButton->SetSize(BUTTONSIZE);
		mainMenuButton->SetPos(float2{ CENTERX, CENTERY + 100 });
	}
	#pragma endregion
	pauseMenu[2] = mainMenuButton;
	#pragma endregion

	#pragma region Options_Menu
	//Main Options Text
	optionsMenu[0] = gameManager->GetUIManager()->CreateText(RECT{ 0,0,0,0 }, false, false, float2{ 0,0 }, F_ARIAL, "OPTIONS");
	optionsMenu[0]->SetPos(static_cast<LONG>(screenW * 0.5f), static_cast<LONG>(0.0f));
	
	//Volume Text
	optionsMenu[1] = gameManager->GetUIManager()->CreateText(RECT{ 0,0,0,0 }, false, false, float2{ 0,0 }, F_ARIAL, "VOLUME");
	optionsMenu[1]->SetPos(static_cast<LONG>(screenW * 0.5f), static_cast<LONG>((screenH * 0.5f) + 50));
	
	//Volume Up Button
	optionsMenu[2] = gameManager->GetUIManager()->CreateButton(RECT{ 0,0,0,0 }, false, false, float2{ 0,0 }, this->GetDevice(), F_ARIAL, "+");
	#pragma region Volume_Up_Button 
	ButtonElement* upButton = static_cast<ButtonElement*>(optionsMenu[2]);
	if (upButton)
	{
		upButton->SetDefaultTexture("DrawingStuff/ButtonDefault.dds");
		upButton->SetMouseOverTexture("DrawingStuff/ButtonMouseOver.dds");
		upButton->SetMouseClickTexture("DrawingStuff/ButtonMouseClick.dds");
		upButton->m_OnMouseClick = [this]()
		{
			this->VolumeUp();
		};
		upButton->SetSize(100, 50);
		upButton->SetPos(screenW * 0.5f, screenH * 0.5f);
	}
	#pragma endregion

	optionsMenu[3] = gameManager->GetUIManager()->CreateButton(RECT{ 0,0,0,0 }, false, false, float2{ 0,0 }, this->GetDevice(), F_ARIAL, "-");
	#pragma region Volume_Down_Button
	ButtonElement* downButton = static_cast<ButtonElement*>(optionsMenu[3]);
	if (downButton)
	{
		downButton->SetDefaultTexture("DrawingStuff/ButtonDefault.dds");
		downButton->SetMouseOverTexture("DrawingStuff/ButtonMouseOver.dds");
		downButton->SetMouseClickTexture("DrawingStuff/ButtonMouseClick.dds");
		downButton->m_OnMouseClick = [this]()
		{
			//Volume Down
			this->VolumeDown();
		};
		downButton->SetSize(100, 50);
		downButton->SetPos(screenW * 0.5f + 100, screenH  * 0.5f);
	}
	#pragma endregion

	optionsMenu[4] = gameManager->GetUIManager()->CreateButton(RECT{ 0,0,0,0 }, false, false, float2{ 0,0 }, this->GetDevice(), F_ARIAL, "Back");
	#pragma region Options_Back_Button
	ButtonElement* backButton = static_cast<ButtonElement*>(optionsMenu[4]);
	if (backButton)
	{
		backButton->SetDefaultTexture("DrawingStuff/ButtonDefault.dds");
		backButton->SetMouseOverTexture("DrawingStuff/ButtonMouseOver.dds");
		backButton->SetMouseClickTexture("DrawingStuff/ButtonMouseClick.dds");
		backButton->m_OnMouseClick = [this]()
		{
			SetOptionsMenu(false);
			//Volume Down
			if (this->mainMenu)
			{
				ShowMainMenu();
				this->mainMenuBool = false;
			}
			else if (this->pauseMenu)
			{
				SetPauseMenu(true);
				this->pauseMenuBool = false;
			}
		};
		backButton->SetSize(100, 50);
		backButton->SetPos(0.0f, screenH - static_cast<float>(backButton->GetBottom()));
	}
	#pragma endregion

	#pragma endregion

	#pragma region Player_UI
	std::string EnemyTxt = "Enemies: " + std::to_string(gameManager->GetEnemies());
	gameManager->m_scoreText = playerUI[0] = gameManager->GetUIManager()->CreateText(RECT{ 0,0,0,0 }, false, false, float2{ 0,0 }, F_COMICSANS, EnemyTxt);

	std::string healthTxt = "Health: " + std::to_string(gameManager->GetPlayer()->GetHealth());
	gameManager->m_healthText = playerUI[1] = gameManager->GetUIManager()->CreateText(RECT{ 0,0,0,0 }, false, false, float2{ 0,50 }, F_COMICSANS, healthTxt);

	playerUI[2] = gameManager->m_weapon = gameManager->GetUIManager()->CreateText(RECT{ 0,0,0,0 }, false, false, float2{ 0, 100 }, F_ARIAL, "Current Weapon: ");

	std::string timerTxt = "Total Time: ";
	playerUI[3] = gameManager->m_timerText = gameManager->GetUIManager()->CreateText(RECT{ 0,0,0,0 }, false, false, float2{ CENTERX - 20, 0 }, F_ARIAL, timerTxt);


	memset(tempT2, '\0', sizeof(tempT2));
	int tempIn = 0;
	_itoa_s(tempIn, tempT2, 65, 10);

	numToChr = std::to_string(tempIn);
	const char* tempT100 = numToChr.c_str();
	playerUI[5] = gameManager->m_timerText = gameManager->GetUIManager()->CreateText(RECT{ 0,0,0,0 }, false, false, float2{ CENTERX - 20, 0 }, F_ARIAL, tempT100);

	m_FPSText = gameManager->GetUIManager()->CreateText(RECT{ 0,0,0,0 }, true, true, float2{ 1800, 0 }, F_ARIAL, "poopoo");

	gameManager->m_damagetimerText = gameManager->GetUIManager()->CreateText(RECT{ 0,0,0,0 }, true, true, float2{ 1600, 980 }, F_ARIAL, "");

	gameManager->m_damagetimerText->SetEnabled(false);

	playerUI[6] = gameManager->m_Currency = gameManager->GetUIManager()->CreateText(RECT{ 0,0,0,0 }, false, false, float2{20, 1000}, F_ARIAL, "");
	#pragma endregion

#pragma endregion
	
	return true;
}

void MyWindow::Shutdown()
{
	//Clean up the graphics object
	gameManager->ShutDown();

	//Cleans up the window
	ShutdownWindows();
}

void MyWindow::Render()
{
	MSG msg;
	bool result;

	ZeroMemory(&msg, sizeof(MSG));
	
	m_done = false;
	while (!m_done)
	{
		//Read in any messages 
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//Make the message readable
			TranslateMessage(&msg);
			//Deploy the message 
			DispatchMessage(&msg);
		}

		//Quit message received 
		if (msg.message == WM_QUIT)
		{
			m_done = true;
		}
		else
		{
			CalcFPS();
			result = Run();
			//If something goes wrong, break out 
			if (!result)
			{
				m_done = true;
			}
		}
	}
}

LRESULT MyWindow::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	float newx = 0;
	float newy = 0;
	switch (msg)
	{
	case WM_KEYDOWN:
	{
		//Update the input object on every key press
		gameManager->SetKeyState(wparam, true);
		return 0;
	}
	case WM_KEYUP:
	{
		//Update the input object on every key release
		gameManager->SetKeyState(wparam, false);
		return 0;
	}
	case WM_MOUSEMOVE:
		gameManager->GetInputManager()->SetMousePos(POINT{ GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) });
		return 0;
	case WM_LBUTTONDOWN:
		gameManager->GetInputManager()->SetKeyState(_LMOUSE, true);
		return 0;
	case WM_LBUTTONUP:
		gameManager->GetInputManager()->SetKeyState(_LMOUSE, false);
		return 0;
	case WM_RBUTTONUP:
		gameManager->GetInputManager()->SetKeyState(_RMOUSE, true);
		return 0;
	case WM_RBUTTONDOWN:
		gameManager->GetInputManager()->SetKeyState(_RMOUSE, false);
		return 0;
	default:
	{
		//All other messages can be handled normally 
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	}

	return LRESULT();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	default:
	{
		//Pass the messages into the application 
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}

void MyWindow::Update(float delta)
{
	gameManager->Update(delta, timer.TotalTime());
}