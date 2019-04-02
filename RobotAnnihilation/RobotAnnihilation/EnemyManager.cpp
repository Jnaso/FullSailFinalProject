#include "EnemyManager.h"

EnemyManager::EnemyManager()
{
	SeperationStrength = 1000.0f;
}

void EnemyManager::Initialize(ID3D11Device *myDevice)
{
	srand((unsigned int)time(NULL));
	enemyCount = rand() % 25 + 10;
	for (unsigned int i = 0; i < enemyCount; i++)
	{
		myEnemies.push_back(new Target());
		myEnemies[i]->Initialize(myDevice, "Assets/Sphere.mesh", float3{ (((float)rand() - (float)rand()) / RAND_MAX) * 100.0f, 2.0f, ((((float)rand() - (float)rand()) / RAND_MAX) * 100.0f) + 5.0f });
	}
}

void EnemyManager::Shutdown()
{
	for (unsigned int i = 0; i < myEnemies.size(); i++)
	{
		myEnemies[i]->Shutdown();
		delete myEnemies[i];
		myEnemies[i] = nullptr;
	}
}

void EnemyManager::Update(float delta, Player *myPlayer)
{
	float3 accel;
	float accelMulti = 0;
	for (unsigned int i = 0; i < myEnemies.size(); i++)
	{
		accel = CalculateSeperation(*myEnemies[i]);
		accel *= delta;
		myEnemies[i]->GetPhysicsComponent()->SetVelocity({ myEnemies[i]->GetPhysicsComponent()->GetVelocity().x + accel.x,  myEnemies[i]->GetPhysicsComponent()->GetVelocity().y + accel.y,  myEnemies[i]->GetPhysicsComponent()->GetVelocity().z + accel.z});

		myEnemies[i]->Update(delta, myPlayer->GetPhysicsComponent()->GetPosition(), myPlayer);
		if (myEnemies[i]->Destroy())
		{
			Target *temp;
			myEnemies[i]->Shutdown();
			temp = myEnemies[i];
			myEnemies.erase(myEnemies.begin() + i);
			delete temp;
			break;
		}

		if (SphereToAABB(*myEnemies[i]->GetCollider(0), myPlayer->GetAABB()))
		{
			myPlayer->SetHealth(myPlayer->GetHealth() - 3);
			std::cout << myPlayer->GetHealth() << std::endl;
		}
	}

	enemyCount = myEnemies.size();
}

vector<Target*> EnemyManager::GetEnemies()
{
	return myEnemies;
}

unsigned int EnemyManager::GetEnemyCount()
{
	return enemyCount;
}

float3 EnemyManager::CalculateCohesion(Target * myT)
{
	float3 myV = AveragePosition - myT->GetPhysicsComponent()->GetPosition();

	float distance = sqrtf(pow(myV.x, 2) + pow(myV.y, 2) + pow(myV.z, 2));

	myV.x /= distance;
	myV.y /= distance;
	myV.z /= distance;

	if (distance < flockRadius)
	{
		myV.x *= distance / flockRadius;
		myV.y *= distance / flockRadius;
		myV.z *= distance / flockRadius;
	}

	return myV * CohesionStrength;
}

float3 EnemyManager::CalculateSeperation(Target &myT)
{
	float3 sum = { 0, 0, 0 };

	for (unsigned int i = 0; i < myEnemies.size(); i++)
	{
		if (&myT == myEnemies[i])
		{
			continue;
		}

		float3 myV;
		myV.x = myT.GetPhysicsComponent()->GetPosition().x - myEnemies[i]->GetPhysicsComponent()->GetPosition().x;
		myV.y = myT.GetPhysicsComponent()->GetPosition().y - myEnemies[i]->GetPhysicsComponent()->GetPosition().y;
		myV.z = myT.GetPhysicsComponent()->GetPosition().z - myEnemies[i]->GetPhysicsComponent()->GetPosition().z;
		float distance = sqrtf(pow(myV.x, 2) + pow(myV.y, 2) + pow(myV.z, 2));
		float safeDistance = safeRadius + safeRadius;

		if (distance < safeDistance)
		{
			myV.x /= distance;
			myV.y /= distance;
			myV.z /= distance;

			float multiple = (safeDistance - distance) / safeDistance;

			myV.x *= multiple;
			myV.y *= multiple;
			myV.z *= multiple;
			sum += myV;
		}
	}

	float distanceSum = sqrtf(pow(sum.x, 2) + pow(sum.y, 2) + pow(sum.z, 2));

	if (distanceSum > 1.0f)
	{
		sum.x /= distanceSum;
		sum.y /= distanceSum;
		sum.z /= distanceSum;
	}

	return sum * SeperationStrength;
}

