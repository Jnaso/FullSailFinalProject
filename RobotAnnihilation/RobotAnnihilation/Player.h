#pragma once
#include "GameObject.h"
class Player :
	public GameObject
{
protected:
	Animation* currentUpperAnimation;
	Animation* currentLowerAnimation;

	float timeLeft = 0;
	float fireRatePistol = 0.5f;
	float fireRateMachine = 0.3f;
	float fireRateSubMachine = 0.15f;

	float currentFireRate = fireRatePistol;

public:
	Player();
	~Player();
	bool Initialize(const char* filePath, ID3D11Device* device);
	void Render(ID3D11DeviceContext* context);
	void Shutdown();
	void Update(float delta);
	std::vector<float4x4> SetSkeletonLines(anim_clip animationUpper, anim_clip animationLower, float timeUpper, float timeLower);

	void SetAnimationUpper(int index);
	void SetAnimationLower(int index);

	float getTimeLeft() { return timeLeft; };
	float GetCurrentFireRate() { return currentFireRate; };
	void SetCurrentFireRate(float FireRate) { currentFireRate = FireRate; };
	float getFireRatePistol() { return fireRatePistol; };
	float getFireRateMachine() { return fireRateMachine; };
	float getFireRateSubMachine() { return fireRateSubMachine; };

	void SubTimeLeft(float delta) { timeLeft -= delta; };

	void SetTimeLeft(float time) { timeLeft = time; };
};

