#pragma once
#ifndef _TARGET_H_
#define _TARGET_H_

#include "GameObject.h"
#include "Player.h"

class Target : public GameObject
{
private:
	bool readyToDestroy;
	AABB myCollision;
	float velocity;
	float timeBetweenAttacks;
	float timeBetweenDamage;
	bool attacking;

	float health = 100;

public:
	enum DamageType
	{
		Gun, Melee, Count
	};
	Target();

	bool Initialize(ID3D11Device* myDevice, const char *fileName, float3 position);
	bool Destroy();
	void SetDestroy();
	void Update(float delta, float3 forward, Player *myPlayer);
	AABB GetAABB();
	void Attack(Player *myPlayer);

	float GetHealth() { return health; }
	void SubHealth(float newHealth, DamageType dmg, HWND window);
};

#endif // !_TARGET_H_
