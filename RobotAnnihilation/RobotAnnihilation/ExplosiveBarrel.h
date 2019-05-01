#pragma once
#ifndef _EXPLOSIVEBARREL_H_
#define _EXPLOSIVEBARREL_H_

#include "GameObject.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

class ExplosiveBarrel : public GameObject
{
private: 
	bool readyToDestroy;

public:
	ExplosiveBarrel();

	bool Initialize(ID3D11Device* myDevice, const char *fileName, float3 position);
	void Update(float delta, Player * myPlayer, std::vector<Bullet*> &bullets, ID3D11Device *myDevice, HWND window);
	void Destroy(Player *myPlayer, std::vector<Enemy*> &myEnemies, HWND window);

	bool GetDestroy() { return readyToDestroy; }
	void SetDestroy() { readyToDestroy = true; }
};

#endif // !_EXPLOSIVEBARREL_H_

