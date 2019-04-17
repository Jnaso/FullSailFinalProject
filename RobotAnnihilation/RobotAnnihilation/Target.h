#pragma once
#ifndef _TARGET_H_
#define _TARGET_H_

#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"

class Target : public Enemy
{

private:
	float timeBetweenAttacks;
	float timeBetweenDamage;
	bool attacking;
	bool pushed;

	float health = 100;

public:
	Target();

	bool Initialize(ID3D11Device* myDevice, const char *fileName, float3 position);
	void Update(float delta, Player * myPlayer, std::vector<Bullet*> &bullets, ID3D11Device *myDevice, HWND window);
	void Attack(Player *myPlayer);
	float GetHealth() { return health; }

	void ApplyForce(float3 force);
};

#endif // !_TARGET_H_
