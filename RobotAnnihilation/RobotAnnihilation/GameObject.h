#pragma once
#include "MathDefines.h"
#include "WICTextureLoader.h"
#include "Animation.h"
#include "PhysicsComponent.h"
#include "Model.h"
#include "Sound.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <dxgi1_2.h>
#include <d3d11_2.h>
#include <DirectXMath.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DXGI.lib")

using namespace DirectX;

class GameObject
{
protected:
	PhysicsComponent* ObjectPhysics;

	Animation** objectAnimations;
	int currentAnimation;
	int AnimationCount;

	Model* objectModel;

	Sound** objectSounds;
	int SoundCount;

public:
	GameObject();
	GameObject(int animationAmounts, int soundAmounts);
	~GameObject();

	virtual bool Initialize(const char* filePath, ID3D11Device* device);
	virtual void Render(ID3D11DeviceContext* context);
	virtual void Shutdown();

	virtual void Update(float delta);

	void AddAninimation(const char* filePath, ID3D11Device* device, int index);

	PhysicsComponent* GetPhysicsComponent();
	void SetPhysicsComponent(PhysicsComponent* newPhysics);
};

