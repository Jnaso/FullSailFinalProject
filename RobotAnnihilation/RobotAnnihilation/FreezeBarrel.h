#pragma once
#ifndef _FREEZEBARREL_H_
#define _FREEZEBARREL_H_

#include "GameObject.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

class FreezeBarrel : public GameObject
{
private:
	bool readyToDestroy;

public:
	FreezeBarrel();

	bool Initialize(ID3D11Device* myDevice, const char *fileName, float3 position);
	void Destroy(Player *myPlayer, std::vector<Enemy*> &myEnemies, HWND window);

	bool GetDestroy() { return readyToDestroy; }
	void SetDestroy() { readyToDestroy = true; }
};

#endif // !_FREEZEBARREL_H_