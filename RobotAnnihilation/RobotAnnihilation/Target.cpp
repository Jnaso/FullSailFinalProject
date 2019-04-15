#include "Target.h"

Target::Target() 
{
	Enemy::Enemy();
	timeBetweenAttacks = 3.0f;
	pushed = false;
}

bool Target::Initialize(ID3D11Device * myDevice, const char * fileName, float3 position)
{
	GetModelComponent()->Initialize(fileName, myDevice);
	if (!GetModelComponent())
	{
		return false;
	}

	GetPhysicsComponent()->SetPosition(position);
	GetPhysicsComponent()->SetMass(5.0f);
	GetPhysicsComponent()->SetDamping(0.99f);

	AddCollider(GetPhysicsComponent()->GetPosition(), 0.7f);
	//srand((unsigned int)time(NULL));

	velocity = RandomUniform();

	attacking = false;

	return true;
}

void Target::Update(float delta, Player *myPlayer, std::vector<Bullet*> &bullets, ID3D11Device *myDevice)
{
	if (!pushed)
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
		}
		else
		{
			attacking = false;
		}

		if (attacking)
		{
			Attack(myPlayer);
		}
		if (timeBetweenDamage > 0)
		{
			timeBetweenDamage -= delta;
		}

		if (timeGetTime() >= HurtTime + 200)
		{
			ImHurt = false;
			HurtTime = 0.8f;
		}
	}
}

void Target::Attack(Player *myPlayer)
{
	if (timeGetTime() >= timeBetweenAttacks + 1000)
	{
		if (!myPlayer->GetInvincible())
		{
			myPlayer->SetHealth(myPlayer->GetHealth() - 3.0f);
		}
		timeBetweenAttacks = timeGetTime();
	}
}

void Target::ApplyForce(float3 force)
{
	float3 forward = GetPhysicsComponent()->GetPosition() - force;
}




