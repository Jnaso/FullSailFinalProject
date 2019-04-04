#pragma once
#ifndef _ENEMYMANAGER_H_
#define _ENEMYMANAGER_H_

#include <vector>
#include "Target.h"
#include "Player.h"

using namespace std;

class EnemyManager
{
private:
	vector<Target *> myEnemies;
	unsigned int enemyCount;
	unsigned int currentEnemies = 0;
	float timeBetween = 0.0f;
	float CohesionStrength;
	float SeperationStrength;
	float3 AveragePosition;
	float3 AverageForward;
	float flockRadius;

	const float safeRadius = 1.0f;

	float3 SpawnPoints[4];

	ID3D11Device* myDevice;
	unsigned int TotalEnemiesSpawned = 0;

public:
	EnemyManager();

	void Initialize(ID3D11Device *myDevice);
	void Shutdown();
	void Update(float delta, Player *myPlayer);

	vector<Target *> GetEnemies();
	unsigned int GetEnemyCount();
	float3 CalculateCohesion(Target *myT);
	float3 CalculateSeperation(Target &myT);
};

#endif // !_ENEMYMANAGER_H_
