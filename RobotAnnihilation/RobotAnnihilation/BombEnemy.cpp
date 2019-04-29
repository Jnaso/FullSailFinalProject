#include "BombEnemy.h"

BombEnemy::BombEnemy()
{
	Enemy::Enemy();
	bombTimer = 2.5f;
}

bool BombEnemy::Initialize(ID3D11Device * myDevice, const char * fileName, float3 position)
{
	GetModelComponent()->Initialize(fileName, myDevice);
	if (!GetModelComponent())
	{
		return false;
	}

	GetPhysicsComponent()->SetPosition(position);
	GetPhysicsComponent()->SetMass(5.0f);
	GetPhysicsComponent()->SetDamping(0.99f);
	GetPhysicsComponent()->SetVelocity({ 1, 1, 1 });

	AddCollider(GetPhysicsComponent()->GetPosition(), 0.7f);

	velocity = 1.0f;

	attacking = false;

	return true;
}

void BombEnemy::Update(float delta, Player * myPlayer, std::vector<Bullet*> &bullets, ID3D11Device * myDevice, std::vector<Enemy*> &myEnemies, HWND window)
{
	if (!attacking)
	{
		GameObject::Update(delta);
		float3 forward2 = myPlayer->GetPhysicsComponent()->GetPosition() - GetPhysicsComponent()->GetPosition();
		GetPhysicsComponent()->SetForward(forward2);
		GetPhysicsComponent()->SetVelocity(forward2 * velocity);
		GetPhysicsComponent()->SetPosition({ GetPhysicsComponent()->GetPosition().x, 2.0f, GetPhysicsComponent()->GetPosition().z });
		GetCollider(0)->center = { GetPhysicsComponent()->GetPosition().x, GetPhysicsComponent()->GetPosition().y, GetPhysicsComponent()->GetPosition().z };
	}

	if (DitanceFloat3(GetPhysicsComponent()->GetPosition(), myPlayer->GetPhysicsComponent()->GetPosition()) <= 3.0f)
	{
		attacking = true;
		ImHurt = true;
	}
	

	if (attacking)
	{
		bombTimer -= delta;
		if (bombTimer <= 0)
		{
			Attack(myPlayer, myEnemies, window);
		}
	}
}

void BombEnemy::Attack(Player * myPlayer, std::vector<Enemy*> &myEnemies, HWND window)
{
	if (DitanceFloat3(GetPhysicsComponent()->GetPosition(), myPlayer->GetPhysicsComponent()->GetPosition()) <= 10.0f)
	{
		myPlayer->SetHealth(myPlayer->GetHealth() - 20);
	}

	for (unsigned int i = 0; i < myEnemies.size(); i++)
	{
		if (myEnemies[i] == this)
		{
			continue;
		}
		else if(DitanceFloat3(GetPhysicsComponent()->GetPosition(), myEnemies[i]->GetPhysicsComponent()->GetPosition()) <= 5.0f)
		{
			float3 force = GetPhysicsComponent()->GetPosition() - myEnemies[i]->GetPhysicsComponent()->GetPosition();
			myEnemies[i]->GetPhysicsComponent()->SetPosition({ (myEnemies[i]->GetPhysicsComponent()->GetPosition().x + force.x * -1.0f), myEnemies[i]->GetPhysicsComponent()->GetPosition().y, (myEnemies[i]->GetPhysicsComponent()->GetPosition().z + force.z * -1.0f)});
			//myEnemies[i]->GetPhysicsComponent()->AddVelocity({ force.x, force.y, force.z });
			myEnemies[i]->SubHealth(10, Enemy::DamageType::Gun, window);
		}
	}

	readyToDestroy = true;
}
