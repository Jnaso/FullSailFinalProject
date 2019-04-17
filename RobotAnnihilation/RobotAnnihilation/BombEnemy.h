#pragma once
#ifndef _BOMBENEMY_H_
#define _BOMBENEMY_H_
#include "Enemy.h"
#include "Sound.h"

class BombEnemy : public Enemy
{
private:
	bool attacking;
	float health = 50;
	float bombTimer;

public:
	BombEnemy();

	bool Initialize(ID3D11Device* myDevice, const char *fileName, float3 position);
	void Update(float delta, Player * myPlayer, std::vector<Bullet*> &bullets, ID3D11Device *myDevice, std::vector<Enemy*> &myEnemies, HWND window);
	void Attack(Player *myPlayer, std::vector<Enemy*> &myEnemies, HWND window);
	float GetHealth() { return health; }
};

#endif // !_BOMBENEMY_H_
