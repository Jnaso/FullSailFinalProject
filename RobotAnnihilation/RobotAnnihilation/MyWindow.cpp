#include "MyWindow.h"

MyWindow::MyWindow()
{
	gameManager = nullptr;
}

bool MyWindow::Run()
{
	bool result;

	//If user presses escape, close the window 
	if (gameManager->GetKeyState(_ESCAPE))
	{
		return false;
	}

	


	if (gameManager->GetKeyState((int)'P')&1)
	{
		paused = !paused;
	}

	if (gameManager->GetInputManager()->GetCurrMouseState().rgbButtons[0])
	{
		gameManager->GetGraphicsManager()->ShootBullet(myWindow);
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
		screenW = 1280;
		screenH = 720;
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
	UIElement* startText = gameManager->GetUIManager()->CreateText(RECT{ 0,0,0,0 }, false, true, float2{ 50, 50 }, F_ARIAL, "Start");
	UIElement* mainMenuText = gameManager->GetUIManager()->CreateText(RECT{ 0,0,0,0 }, false, true, float2{ 50, 50 }, F_COMICSANS, "Main Menu");
	UIElement* UIButtonImage = gameManager->GetUIManager()->CreateImage(RECT{ 0,0,0,0 }, true, true, float2{ 0, 0 }, "DrawingStuff/UIButton1.dds", gameManager->GetGraphicsManager()->GetGraphicsEngine()->GetDevice());
	UIElement* mainMenuBkrnd = gameManager->GetUIManager()->CreateImage(RECT{ 0,0,0,0 }, false, true, float2{ 0,0 }, "DrawingStuff/MainMenu.dds", gameManager->GetGraphicsManager()->GetGraphicsEngine()->GetDevice());

	//UIElement Adjustments
	mainMenuBkrnd->SetSize(float2{ 1920, 1080 });
	mainMenuBkrnd->SetPos(float2{ screenW * 0.5f, screenH * 0.5f });
	
	UIButtonImage->SetSize(float2{ 215,71 });
	UIButtonImage->SetPos(float2{ screenW * 0.5f, screenH * 0.5f });
	
	startText->SetPos(float2{ UIButtonImage->m_pos.x,UIButtonImage->m_pos.y });
	mainMenuText->SetPos(float2{ (screenW * 0.5f) - 50, mainMenuText->m_pos.y });
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
	bool done, result;

	ZeroMemory(&msg, sizeof(MSG));
	
	done = false;
	while (!done)
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
			done = true;
		}
		else
		{
			timer.Signal();
			if (!paused)
			{
				Update(timer.Delta());
			}
			result = Run();
			//If something goes wrong, break out 
			if (!result)
			{
				done = true;
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

		return 0;
	case WM_RBUTTONDOWN:

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
	gameManager->Update(delta);
}