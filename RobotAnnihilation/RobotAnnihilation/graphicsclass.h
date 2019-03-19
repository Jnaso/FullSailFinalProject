#pragma once
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <Windows.h>
#include "dxstuff.h"
#include "allofthelights.h"
#include "GameObject.h"
#include "Camera.h"
#include "InputManager.h"
#include "ShaderManager.h"

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
	GameObject *Player;
	GameObject *Ground;
	ShaderManager *myShaderManager;
	Lighting *myLighting;
	Camera *myCamera;
	XMFLOAT4 myPosition[2];
	XMFLOAT4 myColors[2];

	ID3D11BlendState*			spriteBlendState;
	ID3D11DepthStencilState*	spriteDepthState;
	ID3D11RasterizerState*		spriteRasterState;

	std::vector<GameObject*> bullets;

public:
	void Update(InputManager *, float delta);
	Graphics();

	void ShootBullet(float x, float y);

	bool Initialize(int windowWidth, int windowHeight, HWND window);
	void Shutdown();
	bool Render(InputManager *);
};

#endif // !_GRAPHICSCLASS_H_