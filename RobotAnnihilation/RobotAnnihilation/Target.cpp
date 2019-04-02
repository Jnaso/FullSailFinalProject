#include "Target.h"

Target::Target() 
{
	GameObject::GameObject();
	readyToDestroy = false;
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

	AddCollider(GetPhysicsComponent()->GetPosition(), 0.5f);
	//srand((unsigned int)time(NULL));

	velocity = RandomUniform();

	attacking = false;

	return true;
}

bool Target::Destroy()
{
	return readyToDestroy;
}

void Target::SetDestroy()
{
	readyToDestroy = true;
}

void Target::Update(float delta, float3 forward, Player *myPlayer)
{
	if (!attacking)
	{
		GameObject::Update(delta);
		float3 forward2 = forward - GetPhysicsComponent()->GetPosition();
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
}

AABB Target::GetAABB()
{
	return myCollision;
}

void Target::Attack(Player *myPlayer)
{
	if (timeGetTime() >= timeBetweenAttacks + 1000)
	{
		myPlayer->SetHealth(myPlayer->GetHealth() - 3.0f);
		timeBetweenAttacks = timeGetTime();
	}
}

