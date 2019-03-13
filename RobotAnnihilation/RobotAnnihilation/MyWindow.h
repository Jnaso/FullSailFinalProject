#pragma once
#ifndef _MYWINDOW_H_
#define _MYWINDOW_H_

#include <Windows.h>
#include "graphicsclass.h"
#include "InputManager.h"

class MyWindow
{
private:
	LPCWSTR myAppName;
	HINSTANCE myInstance;
	HWND myWindow;

	Graphics *myGraphics;
	InputManager *myInput;

	bool Run();
	void CreateWindows(int&, int&);
	void ShutdownWindows();

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