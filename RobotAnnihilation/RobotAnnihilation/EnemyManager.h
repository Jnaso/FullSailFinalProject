#pragma once
#ifndef _ENEMYMANAGER_H_
#define _ENEMYMANAGER_H_

#include <vector>
#include "Target.h"
#include "Player.h"
#include "RangedEnemy.h"

using namespace std;

class EnemyManager
{
private:
	vector<Enemy *> myEnemies;
	unsigned int enemyCount;
	unsigned int currentEnemies = 0;
	float timeBetween = 0.0f;
	float CohesionStrength;
	float SeperationStrength;
	float SeperationObstacleStrength;
	float3 AveragePosition;
	float3 AverageForward;
	float flockRadius;

	const float safeRadius = 1.0f;
	const float safeObstacleRadius = 10.0f;

	float3 SpawnPoints[4];

	ID3D11Device* myDevice;
	unsigned int TotalEnemiesSpawned = 0;

	std::vector<Sound*> EnemiesSounds;

public:
	EnemyManager();

	void Initialize(ID3D11Device *myDevice);
	void Shutdown();
	void Update(float delta, Player *myPlayer, vector<GameObject*> obstacles, std::vector<Bullet*> &bullets, ID3D11Device *myDevice, HWND window);

	vector<Enemy *> GetEnemies();
	unsigned int GetEnemyCount();
	float3 CalculateCohesion(Target *myT);
	float3 CalculateSeperation(Target &myT);
	float3 CalculateObstacleSeperation(Target &myT, vector<GameObject*> obstacles);

	void StartNewRound();

	std::vector<Sound*>GetSounds() { return EnemiesSounds; };
	void AddSound(Sound* newSound) { EnemiesSounds.push_back(newSound); };
};

#endif // !_ENEMYMANAGER_H_
