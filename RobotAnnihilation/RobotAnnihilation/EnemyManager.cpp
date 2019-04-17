#include "EnemyManager.h"

EnemyManager::EnemyManager()
{
	SeperationStrength = 200.0f;
	SeperationObstacleStrength = 800.0f;
}

void EnemyManager::Initialize(ID3D11Device *myDevice)
{
	srand((unsigned int)time(NULL));
	enemyCount = 15;

	for (unsigned int i = 0; i < 4; i++)
	{
		SpawnPoints[i] = float3{ (((float)rand() - (float)rand()) / RAND_MAX) * 60.0f, 0.0f, ((((float)rand() - (float)rand()) / RAND_MAX) * 60.0f) + 5.0f };
	}

	for (unsigned int i = 0; i < 4; i++)
	{
		
		if (rand() % 8 == 0)
		{
			myEnemies.push_back(new BombEnemy());
			myEnemies[myEnemies.size() - 1]->Initialize(myDevice, "Assets/BombEnemy.mesh", SpawnPoints[rand() % 4]);// { (((float)rand() - (float)rand()) / RAND_MAX) * 60.0f, 2.0f, ((((float)rand() - (float)rand()) / RAND_MAX) * 60.0f) + 5.0f });
			myEnemies[myEnemies.size() - 1]->AddAninimation("Assets/BombEnemy.anim", myDevice, 0);
			myEnemies[myEnemies.size() - 1]->SetAnimation(0);
		}
		else if(rand() % 5 == 0)
		{
			myEnemies.push_back(new RangedEnemy());
			myEnemies[myEnemies.size() - 1]->Initialize(myDevice, "Assets/RangedAttack.mesh", SpawnPoints[rand() % 4]);// { (((float)rand() - (float)rand()) / RAND_MAX) * 60.0f, 2.0f, ((((float)rand() - (float)rand()) / RAND_MAX) * 60.0f) + 5.0f });
			myEnemies[myEnemies.size() - 1]->AddAninimation("Assets/RangedAttack.anim", myDevice, 0);
			myEnemies[myEnemies.size() - 1]->SetAnimation(0);
		}
		else
		{
			myEnemies.push_back(new Target());
			myEnemies[myEnemies.size() - 1]->Initialize(myDevice, "Assets/RobotAttack.mesh", SpawnPoints[rand() % 4]);
			myEnemies[myEnemies.size() - 1]->AddAninimation("Assets/RobotAttack.anim", myDevice, 0);
			myEnemies[myEnemies.size() - 1]->AddAninimation("Assets/RobotMove.anim", myDevice, 1);
			myEnemies[myEnemies.size() - 1]->SetAnimation(1);
		}
		myEnemies[myEnemies.size() - 1]->GetPhysicsComponent()->SetAccel({0, -1, 0});
		myEnemies[myEnemies.size() - 1]->GetPhysicsComponent()->SetDamping(.99);
		myEnemies[myEnemies.size() - 1]->GetPhysicsComponent()->SetMass(10);
		myEnemies[myEnemies.size() - 1]->SetCurrency(10);
		//currentEnemies++;
		TotalEnemiesSpawned++;
	}

	this->myDevice = myDevice;
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


void EnemyManager::Update(float delta, Player *myPlayer, vector<GameObject*> obstacles, std::vector<Bullet*> &bullets, ID3D11Device *myDevice, HWND window)
{
	float3 accel;
	float accelMulti = 0;
	if (timeBetween > .25f && enemyCount > TotalEnemiesSpawned)
	{
		if (rand() % 8 == 0)
		{
			myEnemies.push_back(new BombEnemy());
			myEnemies[myEnemies.size() - 1]->Initialize(myDevice, "Assets/BombEnemy.mesh", SpawnPoints[rand() % 4]);
			myEnemies[myEnemies.size() - 1]->AddAninimation("Assets/BombEnemy.anim", myDevice, 0);
			myEnemies[myEnemies.size() - 1]->SetAnimation(0);
		}
		else if (rand() % 5 == 0)
		{
			myEnemies.push_back(new RangedEnemy());
			myEnemies[myEnemies.size() - 1]->Initialize(myDevice, "Assets/RangedAttack.mesh", SpawnPoints[rand() % 4]);
			myEnemies[myEnemies.size() - 1]->AddAninimation("Assets/RangedAttack.anim", myDevice, 0);
			myEnemies[myEnemies.size() - 1]->SetAnimation(0);
		}
		else
		{
			myEnemies.push_back(new Target());
			myEnemies[myEnemies.size() - 1]->Initialize(myDevice, "Assets/RobotAttack.mesh", SpawnPoints[rand() % 4]);
			myEnemies[myEnemies.size() - 1]->AddAninimation("Assets/RobotAttack.anim", myDevice, 0);
			myEnemies[myEnemies.size() - 1]->AddAninimation("Assets/RobotMove.anim", myDevice, 1);
			myEnemies[myEnemies.size() - 1]->SetAnimation(1);
		}
		myEnemies[myEnemies.size() - 1]->GetPhysicsComponent()->SetAccel({ 0, -1, 0 });
		myEnemies[myEnemies.size() - 1]->GetPhysicsComponent()->SetDamping(.99);
		myEnemies[myEnemies.size() - 1]->GetPhysicsComponent()->SetMass(10);
		myEnemies[myEnemies.size() - 1]->SetCurrency(10);
		TotalEnemiesSpawned++;
		timeBetween = 0;
	}
	else
	{
		timeBetween += delta;
	}
	Target *currEnemy;
	BombEnemy *currBomb;
	for (unsigned int i = 0; i < myEnemies.size(); i++)
	{
		currEnemy = dynamic_cast<Target*>(myEnemies[i]);
		if (currEnemy)
		{
			accel = CalculateSeperation(*currEnemy);
			accel += CalculateObstacleSeperation(*currEnemy, obstacles);
			accel *= delta;
			myEnemies[i]->GetPhysicsComponent()->SetVelocity({ myEnemies[i]->GetPhysicsComponent()->GetVelocity().x + accel.x,  myEnemies[i]->GetPhysicsComponent()->GetVelocity().y + accel.y,  myEnemies[i]->GetPhysicsComponent()->GetVelocity().z + accel.z });
			//myEnemies[i]->GetPhysicsComponent()->AddForce({ accel.x, accel.y, accel.z });
		}



		currBomb = dynamic_cast<BombEnemy*>(myEnemies[i]);
		if (currBomb)
		{
			accel = CalculateSeperation(*currBomb);
			accel += CalculateObstacleSeperation(*currBomb, obstacles);
			accel *= delta;
			myEnemies[i]->GetPhysicsComponent()->SetVelocity({ myEnemies[i]->GetPhysicsComponent()->GetVelocity().x + accel.x,  myEnemies[i]->GetPhysicsComponent()->GetVelocity().y + accel.y,  myEnemies[i]->GetPhysicsComponent()->GetVelocity().z + accel.z });
			//myEnemies[i]->GetPhysicsComponent()->AddForce({ accel.x, accel.y, accel.z });
		}

		if (currBomb)
		{
			currBomb->Update(delta, myPlayer, bullets, myDevice, myEnemies, window);
		}
		else
		{
			myEnemies[i]->Update(delta, myPlayer, bullets, myDevice, window);
		}

		if (myEnemies[i]->Destroy())
		{
			if (currBomb)
			{
				EnemiesSounds.push_back(new Sound((char*)"Assets/BombExplosion.wav", 0));
				EnemiesSounds[EnemiesSounds.size() - 1]->Initialize(window);
				EnemiesSounds[EnemiesSounds.size() - 1]->PlayWaveFile();
			}

			Enemy *temp;
			myEnemies[i]->Shutdown();
			temp = myEnemies[i];
			myEnemies.erase(myEnemies.begin() + i);
			delete temp;
			break;
		}

		if (isnan(myEnemies[i]->GetPhysicsComponent()->GetPosition().x) || isnan(myEnemies[i]->GetPhysicsComponent()->GetPosition().z))
		{
			float3 newPos = SpawnPoints[rand() % 4];
			myEnemies[i]->GetPhysicsComponent()->SetPosition({ newPos.x, newPos.y + 2.0f, newPos.z });
		}

		if (SphereToAABB(*myEnemies[i]->GetCollider(0), myPlayer->GetAABB()))
		{
			myPlayer->SetHealth(myPlayer->GetHealth() - 3);
#ifdef DEBUG
			std::cout << myPlayer->GetHealth() << std::endl;
#endif // DEBUG

		}
	}
	if (myEnemies.size() <= 1)
		cout << endl;
		

	currentEnemies = myEnemies.size();
}


vector<Enemy*> &EnemyManager::GetEnemies()
{
	return myEnemies;
}

unsigned int EnemyManager::GetEnemyCount()
{
	return currentEnemies;
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


float3 EnemyManager::CalculateSeperation(Enemy &myT)
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

float3 EnemyManager::CalculateObstacleSeperation(Enemy & myT, vector<GameObject*> obstacles)
{
	float3 sum = { 0, 0, 0 };

	for (unsigned int i = 0; i < obstacles.size(); i++)
	{
		float3 myV;
		myV.x = myT.GetPhysicsComponent()->GetPosition().x - obstacles[i]->GetPhysicsComponent()->GetPosition().x;
		myV.y = myT.GetPhysicsComponent()->GetPosition().y - obstacles[i]->GetPhysicsComponent()->GetPosition().y;
		myV.z = myT.GetPhysicsComponent()->GetPosition().z - obstacles[i]->GetPhysicsComponent()->GetPosition().z;
		float distance = sqrtf(pow(myV.x, 2) + pow(myV.y, 2) + pow(myV.z, 2));
		float safeDistance = safeRadius + safeObstacleRadius;

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

	return sum * SeperationObstacleStrength;
}

void EnemyManager::StartNewRound()
{
	enemyCount += (rand() % 5 + 1);
	TotalEnemiesSpawned = 0;
}



