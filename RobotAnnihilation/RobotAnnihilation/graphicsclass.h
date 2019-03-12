#pragma once
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <Windows.h>
#include "dxstuff.h"

//Full screen flag 
const bool FULL_SCREEN = false;
//Vsync flag 
const bool VSYNC_ENABLED = true;
//Far plane 
const float FAR_PLANE = 100.0f;
//Near plane
const float NEAR_PLANE = 0.1f;

class Graphics
{

private:
	DX *myDX;

public:
	Graphics();
	Graphics(const Graphics &other);
	~Graphics();

	bool Initialize(int windowWidth, int windowHeight, HWND window);
	void Shutdown();
	bool Render();
};

#endif // !_GRAPHICSCLASS_H_