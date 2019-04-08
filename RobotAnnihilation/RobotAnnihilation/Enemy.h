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

public:
	Enemy();
	bool Destroy();
	void SetDestroy();
	virtual bool Initialize(ID3D11Device* myDevice, const char *fileName, float3 position) { return true; };
	virtual void Update(float delta, Player *myPlayer, std::vector<Bullet*> &bullets, ID3D11Device *myDevice) {};


	float GetHealth() { return health; }
	void SubHealth(float newHealth) { health -= newHealth; }
};

#endif // !_ENEMY_H_
