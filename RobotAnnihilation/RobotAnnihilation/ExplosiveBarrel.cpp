#include "ExplosiveBarrel.h"

ExplosiveBarrel::ExplosiveBarrel()
{
	readyToDestroy = false;
}

bool ExplosiveBarrel::Initialize(ID3D11Device * myDevice, const char * fileName, float3 position)
{
	GetModelComponent()->Initialize(fileName, myDevice);
	if (!GetModelComponent())
	{
		return false;
	}

	GetPhysicsComponent()->SetPosition(position);

	//AddCollider({ position.x, position.y + 1.0f, position.z }, 2.3f);

	return true;
}

void ExplosiveBarrel::Destroy(Player * myPlayer, std::vector<Enemy*> &myEnemies, HWND window)
{
	if (DitanceFloat3(GetPhysicsComponent()->GetPosition(), myPlayer->GetPhysicsComponent()->GetPosition()) <= 10.0f)
	{
		myPlayer->SetHealth(myPlayer->GetHealth() - 30);
	}

	for (unsigned int i = 0; i < myEnemies.size(); i++)
	{
		if (DitanceFloat3(GetPhysicsComponent()->GetPosition(), myEnemies[i]->GetPhysicsComponent()->GetPosition()) <= 10.0f)
		{
			float3 force = GetPhysicsComponent()->GetPosition() - myEnemies[i]->GetPhysicsComponent()->GetPosition();
			myEnemies[i]->GetPhysicsComponent()->SetPosition({ (myEnemies[i]->GetPhysicsComponent()->GetPosition().x + force.x * -1.0f), myEnemies[i]->GetPhysicsComponent()->GetPosition().y, (myEnemies[i]->GetPhysicsComponent()->GetPosition().z + force.z * -1.0f) });
			//myEnemies[i]->GetPhysicsComponent()->AddVelocity({ force.x, force.y, force.z });
			myEnemies[i]->SubHealth(10, Enemy::DamageType::Gun, window);
		}
	}

	readyToDestroy = true;
}
