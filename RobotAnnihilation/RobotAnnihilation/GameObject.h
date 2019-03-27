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
	Animation* currentUpperAnimation;
	Animation* currentLowerAnimation;

	Model* objectModel;

	std::vector<Sound*> objectSounds;

	std::vector<Sphere> colliders;

	std::vector<float4x4> AnimationJoints;
	std::vector<int32_t> AnimationParents;

	float timeLeft = 0;
	float fireRatePistol = 0.5f;
	float fireRateMachine = 0.3f;
	float fireRateSubMachine = 0.15f;

	float currentFireRate = fireRatePistol;

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

	void SetAnimationUpper(int index);
	void SetAnimationLower(int index);

	Animation* GetCurrentAnimationUpper();
	Animation* GetCurrentAnimationLower();

	void AddCollider(float3 pos, float rad);

	Sphere GetCollider(int index);

	std::vector<float4x4> SetSkeletonLines(anim_clip animationUpper, anim_clip animationLower, float timeUpper, float timeLower);

	std::vector<float4x4> GetJoints() { return AnimationJoints; };

	float getTimeLeft() { return timeLeft; };
	float GetCurrentFireRate() { return currentFireRate; };
	void SetCurrentFireRate(float FireRate) { currentFireRate = FireRate; };
	float getFireRatePistol() { return fireRatePistol; };
	float getFireRateMachine() { return fireRateMachine; };
	float getFireRateSubMachine() { return fireRateSubMachine; };

	void SubTimeLeft(float delta) { timeLeft -= delta; };

	void SetTimeLeft(float time) { timeLeft = time; };
};

