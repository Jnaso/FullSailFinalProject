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
#include "Gun.h"
#include "Camera.h"
#include "InputManager.h"
#include "ShaderManager.h"
#include "DebugCamera.h"
#include "Sound.h"
#include "UIManager.h"
#include "Bullet.h"
#include "Target.h"
#include <string.h>

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
	UIManager* myUI;
	DX *myDX;
	GameObject *Ground;
	GameObject *Skybox;
	//GameObject *target;
	ShaderManager *myShaderManager;
	Lighting *myLighting;
	Camera *myCamera;
	DebugCamera *myDebug;
	XMFLOAT4 myPosition[2];
	XMFLOAT4 myColors[2];
	XMMATRIX playerWorld;
	bool debugCam;
	float timeBetween;
	XMFLOAT4 camPosition;

	ID3D11BlendState*			spriteBlendState;
	ID3D11DepthStencilState*	spriteDepthState;
	ID3D11RasterizerState*		spriteRasterState;
	//2D Graphics Variables
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	//Temporary (Hoping to make a function that adds to an array of unique_ptrs<>)
	std::unique_ptr<DirectX::SpriteFont> m_arialFont;
	std::unique_ptr<DirectX::SpriteFont> m_comicSansFont;

	vector<GameObject*> modelList;

public:
	void Update(InputManager *, float delta);
	Graphics(InputManager* input);

	void ShootBullet(HWND hwnd);

	bool Initialize(int windowWidth, int windowHeight, HWND window, InputManager *myInput);

	//UI Manager Function Pass Throughs
	void CreateText(RECT srcRect, bool interactable, bool enabled, float2 pos, int font, const char* text);
	void CreateImage(RECT srcRect, bool interactable, bool enabled, float2 pos, const char * filePath);

	void Shutdown();
	bool Render(InputManager *, Player* myPlayer, std::vector<Bullet*> bullets, vector<Target*> myTargets, vector<GameObject*> Obstacles);
	void Update();

	DX* GetGraphicsEngine() { return myDX; }
	UIManager* GetUIManager() { return myUI; }
	Camera* GetCamera() { return myCamera; };
};

#endif // !_GRAPHICSCLASS_H_