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
	UIElement* optionsMenu[5];
	UIElement* playerUI[6];
	UIElement* pauseMenu[3];

	//UI Functions For Function Pointers
private:
	

	void HideMainMenu();
	void ShowMainMenu();

	void SetPauseMenu(bool value);

	void GameIsDone();
	void ShowPlayerUI();
	void ShowFPS();

	void CalcFPS();
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