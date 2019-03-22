#include "MyWindow.h"

MyWindow::MyWindow()
{
	myGraphics = nullptr;
	myInput = nullptr;
	myUiManager = nullptr;
}

bool MyWindow::Run()
{
	bool result;

	//If user presses escape, close the window 
	if (myInput->GetKeyState(_ESCAPE))
	{
		return false;
	}

	//Render every frame and stop if anything goes wrong 
	result = myGraphics->Render(myInput);
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

	//Show/Hide cursor 
	ShowCursor(true);
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

	myInput = new InputManager();
	if (!myInput)
	{
		return false;
	}

	//Set up for graphics object
	myGraphics = new Graphics();
	if (!myGraphics)
	{
		return false;
	}

	result = myGraphics->Initialize(screenW, screenH, myWindow);
	if (!result)
	{
		return false;
	}


	myUiManager = new UI::UIManager(myGraphics, myInput);
	if (!myUiManager)
	{
		return false;
	}

	RECT tempR = RECT{ 1,1,1,1 };

	float2 tempPos;
	tempPos.x = 0;
	tempPos.y = 0;

	myUiManager->CreateImage(tempR, false, "DrawingStuff/turtle.dds",  tempPos);

	myUiManager->CreateText("Hello World", false, F_ARIAL, tempPos);
	return true;
}

void MyWindow::Shutdown()
{
	//Clean up the graphics object
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

	if (myUiManager)
	{
		delete myUiManager;
		myUiManager = 0;
	}

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
			Update(timer.Delta());
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
		myInput->SetKeyState(wparam, true);
		return 0;
	}
	case WM_KEYUP:
	{
		//Update the input object on every key release
		myInput->SetKeyState(wparam, false);
		return 0;
	}
	case WM_MOUSEMOVE:
		myInput->SetMousePos(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
		//SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
		
		break;
	case WM_LBUTTONDOWN:
		// Call bullet function here
		myInput->SetKeyState(_LMOUSE, true);
		newx = myInput->GetMousePos().x;
		newy = myInput->GetMousePos().y;
		myGraphics->ShootBullet(newx, newy, myWindow);
		break;
	case WM_LBUTTONUP:
		myInput->SetKeyState(_LMOUSE, false);
		break;
	case WM_RBUTTONDOWN:
		myInput->SetKeyState(_RMOUSE, true);
		break;
	case WM_RBUTTONUP:
		myInput->SetKeyState(_RMOUSE, false);
		break;
	case WM_MOUSEWHEEL:
		if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
		{
			std::cout << "Scroll Up" << std::endl;
		}
		else if (GET_WHEEL_DELTA_WPARAM(wparam) < 0)
		{
			std::cout << "Scroll Down" << std::endl;
		}
		// Negative = Scroll Down | Positive = Scroll Up
		break;
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
	myGraphics->Update(myInput, delta);
}