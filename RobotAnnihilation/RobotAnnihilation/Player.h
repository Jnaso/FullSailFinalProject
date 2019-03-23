#pragma once
#include "GameObject.h"
#include "MathDefines.h"
class Player : public GameObject
{
	Animation* CurrentAnimation;
	Animation* RunAnimation;
	Animation* IdleAnimation;

public:
	Player();
	Player(const char* filePath, ID3D11Device* device);
	~Player();

	void Update(float delta);

	Animation* GetRunAnimation();

	void SetRunAnimation(Animation* anim);

	Animation* GetIdleAnimation();

	void SetIdleAnimation(Animation* anim);

	Animation* GetCurrentAnimation();

	void SetCurrentAnimation(Animation* anim);
};

