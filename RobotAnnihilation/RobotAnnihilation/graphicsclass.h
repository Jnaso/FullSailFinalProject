#pragma once
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <Windows.h>
#include "dxstuff.h"
#include "shaderstuff.h"
#include "allofthelights.h"
#include "GameObject.h"

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
	GameObject *Player;
	Shaders *myShaders;
	Lighting *myLighting;

public:
	Graphics();

	bool Initialize(int windowWidth, int windowHeight, HWND window);
	void Shutdown();
	bool Render();
};

#endif // !_GRAPHICSCLASS_H_