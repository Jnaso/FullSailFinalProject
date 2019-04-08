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
public:
	enum DamageType
	{
		Gun, Melee, Count
	};
	Target();

	bool Initialize(ID3D11Device* myDevice, const char *fileName, float3 position);
	void Update(float delta, Player * myPlayer, std::vector<Bullet*> &bullets, ID3D11Device *myDevice);
	void Attack(Player *myPlayer);
	float GetHealth() { return health; }
	void SubHealth(float newHealth, DamageType dmg, HWND window);};

#endif // !_TARGET_H_
