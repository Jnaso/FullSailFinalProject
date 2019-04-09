#pragma once
#include "GameObject.h"
#include "Gun.h"
class Player :
	public GameObject
{
protected:
	Animation* currentUpperAnimation;
	Animation* PrevUpperAnimation;
	Animation* currentLowerAnimation;

	Gun* CurrentGun;
	std::vector<Gun*> CurrentGuns;
	float timeLeft = 0.0f;
	float timeLeftDamage = 0.0f;
	AABB playerBox;
	unsigned int health;

	unsigned int CurrentPoints = 0;

	bool PlayOnce = false;

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

	void SubTimeLeft(float delta) { timeLeft -= delta; };

	void SetTimeLeft(float time) { timeLeft = time; };

	void AddGun(Gun* newGun) { CurrentGuns.push_back(newGun); };
	void ReplaceGun(Gun* newGun, int index) { CurrentGuns[index] = newGun; };
	void SetCurrentGun(int index) { CurrentGun = CurrentGuns[index]; };
	Gun* GetCurrentGun() { return CurrentGun; };

	AABB GetAABB() { return playerBox; };

	unsigned int GetHealth() { return health; };
	void SetHealth(unsigned int newHeath) { health = newHeath; }

	void MeleeAttack(int index);

	bool isAttacking() { return PlayOnce; }

	unsigned int GetPoints() { return CurrentPoints; }
	void AddCurrency(unsigned int amount) { CurrentPoints += amount; }

	void SetTimeDamage(float damage) { timeLeftDamage = damage; }
	float GetTimeDamage() { return timeLeftDamage; }
	void SubTimeDamage(float damage) { timeLeftDamage -= damage; }
};

