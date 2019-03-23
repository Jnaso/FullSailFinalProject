#pragma once
#ifndef _MYWINDOW_H_
#define _MYWINDOW_H_

#include <Windows.h>
#include "graphicsclass.h"
#include "InputManager.h"
#include "UIManager.h"
#include "XTime.h"
#include "GameManager.h"
#include <windowsx.h>

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

	bool paused = false;

public:
	MyWindow();

	bool Initialize();
	void Shutdown();
	void Render();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static MyWindow *ApplicationHandle = 0;

#endif // !_MYWINDOW_H_