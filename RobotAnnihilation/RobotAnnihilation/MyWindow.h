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

	ID3D11Device* GetDevice();

	bool paused = false;
	bool m_done = false;

	std::string numToChr;
	
	UIElement* mainMenu[5];
	UIElement* optionsMenu[5];
	UIElement* playerUI[4];
	UIElement* pauseMenu[4];

	//UI Functions For Function Pointers
private:
	

	void HideMainMenu();
	void ShowMainMenu();

	void SetPauseMenu(bool value);

	void GameIsDone();
	void ShowPlayerUI();
public:
	MyWindow();
	
	bool Initialize();
	void Shutdown();
	void Render();

	bool GetPaused() { return paused; }
	void SetPaused(bool value) { paused = value; }

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static MyWindow *ApplicationHandle = 0;

#endif // !_MYWINDOW_H_