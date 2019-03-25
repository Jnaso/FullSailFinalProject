#include "Bullet.h"

Bullet::Bullet(float life)
{
	//Initializing variables 
	readyToDestroy = false;
	lifeTime = life;
}

bool Bullet::Initialize(ID3D11Device * myDevice, const char * fileName, float3 forward, float3 position)
{

	GetModelComponent()->Initialize("Assets/Sphere.mesh", myDevice);
	if (!GetModelComponent())
	{
		return false;
	}

	GetPhysicsComponent()->SetForward(forward);
	GetPhysicsComponent()->SetPosition(position);
	velocity = forward * -100.0f;
	GetPhysicsComponent()->SetLifeTime(lifeTime);
	GetPhysicsComponent()->SetVelocity(velocity);
	GetPhysicsComponent()->SetAccel({ 0, -1.0f, 0 });
	GetPhysicsComponent()->SetMass(2.0f);
	GetPhysicsComponent()->SetDamping(0.99f);

	AddCollider(GetPhysicsComponent()->GetPosition(), 0.8f);

	return true;
}

void Bullet::Update(float delta)
{
	GameObject::Update(delta);
	if (GetPhysicsComponent()->GetLifeTime() <= 0.0f)
	{
		readyToDestroy = true;
	}

	GetCollider(0).center = GetPhysicsComponent()->GetPosition();
}

void Bullet::SetVelocity(float3 vel)
{
	velocity = vel;
}

float3 Bullet::GetVelocity()
{
	return velocity;
}

bool Bullet::Destroy()
{
	return readyToDestroy;
}

void Bullet::SetDestroy()
{
	readyToDestroy = true;
}
