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
	int health = 1000;
	int MaxHealth = 1000;
	Segment forwardArrow;
	Segment backwardArrow;
	Segment leftArrow;
	Segment rightArrow;
	bool canMoveForward;
	bool canMoveBackward;
	bool canMoveLeft;
	bool canMoveRight;
	bool collidingF;
	bool collidingB;
	bool collidingL;
	bool collidingR;
	bool Jumping = false;

	unsigned int CurrentPoints = 0;

	bool PlayOnce = false;
	bool isInvincible = false;

public:
	Player();
	~Player();
	bool Initialize(const char* filePath, ID3D11Device* device);
	void Render(ID3D11DeviceContext* context);
	void Shutdown();
	void CheckMovement();
	void Update(float delta);
	std::vector<float4x4> SetSkeletonLines(anim_clip animationUpper, anim_clip animationLower, float timeUpper, float timeLower);

	void SetAnimationUpper(int index);
	void SetAnimationLower(int index);

	float getTimeLeft() { return timeLeft; };

	void SubTimeLeft(float delta) { timeLeft -= delta; };

	void SetTimeLeft(float time) { timeLeft = time; };

	void AddGun(Gun* newGun) { CurrentGuns.push_back(newGun); };
	void ReplaceGun(Gun* newGun, int index) { CurrentGuns[index] = newGun; };
	void SetCurrentGun(int index) { if(index < CurrentGuns.size())CurrentGun = CurrentGuns[index]; };
	Gun* GetCurrentGun() { return CurrentGun; };
	std::vector<Gun*> GetCurrentGuns() { return CurrentGuns; };

	Segment GetForwardArrow();
	Segment GetBackwardArrow();
	Segment GetLeftArrow();
	Segment GetRightArrow();
	bool MoveForward();
	bool MoveBackward();
	bool MoveLeft();
	bool MoveRight();
	bool isJumping() { return Jumping; }
	void setJumping(bool jump) { Jumping = jump; }

	void SetForward(bool f);
	void SetBackward(bool f);
	void SetLeft(bool f);
	void SetRight(bool f);

	AABB GetAABB() { return playerBox; };

	int GetHealth() { return health; };
	int GetMaxHealth() { return MaxHealth; };
	void SetHealth(int newHeath) { health = newHeath; }
	void Damage(int amount) { health -= amount; }

	void MeleeAttack(int index);

	bool isAttacking() { return PlayOnce; }

	unsigned int GetPoints() { return CurrentPoints; }
	void AddCurrency(unsigned int amount) { CurrentPoints += amount; }

	void SetTimeDamage(float damage) { timeLeftDamage = damage; }
	float GetTimeDamage() { return timeLeftDamage; }
	void SubTimeDamage(float damage) { timeLeftDamage -= damage; }

	void FlipInvincible() { isInvincible = !isInvincible; }
	bool GetInvincible() { return isInvincible; }
};

