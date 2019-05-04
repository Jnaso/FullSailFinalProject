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
	bool isFrozen;
	float velocity;
	bool attacking;
	float health = 100;
	float timeBetweenAttacks;
	float timeBetweenDamage = 0.0f;
	float HurtTime = 0.8f;
	bool ImHurt;
	float freezeTime;

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
	virtual void Update(float delta, Player *myPlayer, std::vector<Bullet*> &bullets, ID3D11Device *myDevice, HWND window);


	float GetHealth() { return health; }
	void SubHealth(float newHealth, DamageType dmg, HWND window);

	unsigned int GetCurrency() { return CurrencyGiven; }
	void SetCurrency(unsigned int newCurr) { CurrencyGiven = newCurr; };

	void SetHurt();
	bool GetHurt();

	void SetFrozen() { isFrozen = true; }
	bool GetFrozen() { return isFrozen; }
};


#endif // !_ENEMY_H_
