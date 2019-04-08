#pragma once
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "GameObject.h"
#include "Player.h"
#include "Bullet.h"

class Enemy : public GameObject
{
protected:
	bool readyToDestroy;
	float velocity;
	bool attacking;
	float health = 100;
	float timeBetweenAttacks;
	float timeBetweenDamage;

	unsigned int CurrencyGiven;

public:
	enum DamageType
	{
		Gun, Melee, Count
	};
	Enemy();
	bool Destroy();
	void SetDestroy();
	virtual bool Initialize(ID3D11Device* myDevice, const char *fileName, float3 position) { return true; };
	virtual void Update(float delta, Player *myPlayer, std::vector<Bullet*> &bullets, ID3D11Device *myDevice) {};


	float GetHealth() { return health; }
	void SubHealth(float newHealth, DamageType dmg, HWND window);

	float GetCurrency() { return CurrencyGiven; }
	void SetCurrency(unsigned int newCurr) { CurrencyGiven = newCurr; };
};


#endif // !_ENEMY_H_
