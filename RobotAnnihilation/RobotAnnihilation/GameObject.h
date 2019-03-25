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

	std::vector<Animation*> objectAnimations;
	Animation* currentAnimation;

	Model* objectModel;

	std::vector<Sound*> objectSounds;

	float fireRate = .5f;
	float TimeLeft = 0.0f;

public:
	GameObject();
	~GameObject();

	virtual bool Initialize(const char* filePath, ID3D11Device* device);
	virtual void Render(ID3D11DeviceContext* context);
	virtual void Shutdown();

	virtual void Update(float delta);

	void AddAninimation(const char* filePath, ID3D11Device* device, int index);

	PhysicsComponent* GetPhysicsComponent();
	void SetPhysicsComponent(PhysicsComponent* newPhysics);

	Model* GetModelComponent();

	void SetAnimation(int index);

	Animation* GetCurrentAnimation();

	inline float getFireRate() { return fireRate; };
	inline void SetFireRate(float fire) { fireRate = fire; };

	inline float getTimeLeft() { return TimeLeft; };
	inline void SubTimeLeft(float fire) { TimeLeft -= fire; };
	inline void SetTimeLeft(float fire) { TimeLeft = fire; };
};

