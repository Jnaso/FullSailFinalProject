#pragma once
#ifndef _MYWINDOW_H_
#define _MYWINDOW_H_

#include <Windows.h>
#include "graphicsclass.h"
#include "InputManager.h"
#include "XTime.h"
#include "GameManager.h"
#include <windowsx.h>
#include <map>

class MyWindow
{
private:
	const float DEFAULTKEYPRESST = 0.2f;
	float keyPressTimer = 0.2f;

	bool pauseMenuBool = false;
	bool mainMenuBool = true;

	LPCWSTR myAppName;
	HINSTANCE myInstance;
	HWND myWindow;
	
	GameManager* gameManager;
	XTime timer;
	
	void Update(float delta);
	bool Run();
	void CreateWindows(int&, int&);
	void ShutdownWindows();
	
	char tempT0[65];
	char tempT1[65];
	char tempT2[65];
	char tempT3[65];

	ID3D11Device* GetDevice();

	bool paused = false;
	bool m_done = false;
	bool showFPS = true;
	float FPS;
	float _frameTime;
	UIElement* m_FPSText;
	string numberToChr;

	std::string numToChr;
	
	UIElement* mainMenu[5];
	UIElement* playerUI[9];
	UIElement* pauseMenu[4];
	UIElement* optionsMenu[6];
	//0 = Main Options Text
	//1 = Volume Text
	//2 = Volume Up Button
	//3 = Volume Down Button
	//4 = Back Button
	//5 = Options Backround

	//UI Functions For Function Pointers
private:

	void HideMainMenu();
	void ShowMainMenu();

	void SetPauseMenu(bool value);

	void GameIsDone();

	void ShowPlayerUI();
	void HidePlayerUI();

	void SetOptionsMenu(bool val);
	void SetMainMenuForOptions(bool val);

	void ShowFPS();

	void VolumeUp();
	void VolumeDown();

	void CalcFPS();
public:
	MyWindow();
	
	bool Initialize();
	void Shutdown();
	void Render();

	bool GetPaused() { return gameManager->paused; }
	void SetPaused(bool value) { gameManager->paused = value; }

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static MyWindow *ApplicationHandle = 0;

#endif // !_MYWINDOW_H_