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

	std::vector<Sphere> colliders;

	std::vector<float4x4> AnimationJoints;
	std::vector<int32_t> AnimationParents;

	float lifeTime = 0.0f;
	float totalTime = 0.0f;

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

	void AddCollider(float3 pos, float rad);

	Sphere* GetCollider(int index);
	std::vector<float4x4> GetJoints() { return AnimationJoints; };

	int GetParent(int index, std::vector<int32_t> parents);

	float4x4 RecursiveJointCalc(unsigned int currentIndex, std::vector<float4x4>& joints, std::vector<int32_t>& parents);

	std::vector<float4x4> Flatten(std::vector<float4x4> joints, std::vector<int32_t> parents);

	std::vector<Sound*>GetSounds() { return objectSounds; };
	void AddSound(Sound* newSound) { objectSounds.push_back(newSound); };

	std::vector<Sphere>GetColliders() { return colliders; }

	float GetLifeTime() { return lifeTime; }
	void SetLifeTime(float newLife) { lifeTime = newLife; }

	float GetTotalTime() { return totalTime; }
	void SetTotalTime(float newLife) { totalTime = newLife; }
};

