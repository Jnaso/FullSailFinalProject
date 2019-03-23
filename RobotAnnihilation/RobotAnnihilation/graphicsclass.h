#pragma once
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <mmsystem.h>
#include "dxstuff.h"
#include "allofthelights.h"
#include "GameObject.h"
#include "Player.h"
#include "Camera.h"
#include "InputManager.h"
#include "ShaderManager.h"
#include "DebugCamera.h"
#include "Sound.h"

//Full screen flag 
const bool FULL_SCREEN = false;
//Vsync flag 
const bool VSYNC_ENABLED = true;
//Far plane 
const float FAR_PLANE = 1000.0f;
//Near plane
const float NEAR_PLANE = 0.1f;

class Graphics
{
private:
	DX *myDX;
	Player *myPlayer;
	GameObject *Ground;
	ShaderManager *myShaderManager;
	Lighting *myLighting;
	Camera *myCamera;
	DebugCamera *myDebug;
	XMFLOAT4 myPosition[2];
	XMFLOAT4 myColors[2];
	XMMATRIX playerWorld;
	bool debugCam;
	float timeBetween;

	ID3D11BlendState*			spriteBlendState;
	ID3D11DepthStencilState*	spriteDepthState;
	ID3D11RasterizerState*		spriteRasterState;

	std::vector<GameObject*> bullets;
	vector<Sound*> myShots;

public:
	void Update(InputManager *, float delta);
	Graphics();

	void ShootBullet(HWND hwnd);

	bool Initialize(int windowWidth, int windowHeight, HWND window);

	//Image Creation
	void CreateImage(RECT dimensions, const char* filePath, float2 pos);
	void CreateImage(const char* filePath, float2 pos);

	void CreateText(const char* text, int font, float2 pos);

	void Shutdown();
	bool Render(InputManager *);
};

#endif // !_GRAPHICSCLASS_H_