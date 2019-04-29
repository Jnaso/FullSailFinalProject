#pragma once
#ifndef _RANGEDENEMY_H_
#define _RANGEDENEMY_H_

#include "Player.h"
#include "Enemy.h"

class RangedEnemy : public Enemy
{
private:
	float timeBetweenAttacks;
	float startTime;
	bool PlayOnce = false;
	bool Dead = false;
	Animation* PrevAnimation;

public:
	RangedEnemy();

	bool Initialize(ID3D11Device* myDevice, const char *fileName, float3 position);
	void Update(float delta, Player *myPlayer, std::vector<Bullet*> &bullets, ID3D11Device *myDevice, HWND window);
	void Attack(Player *myPlayer, std::vector<Bullet*> &bullets, ID3D11Device *myDevice, HWND window);
	void Death();
	bool isDead() { return Dead; }
};

#endif // !_RANGEDENEMY_H_
