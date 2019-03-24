#include "Bullet.h"

Bullet::Bullet(float life)
{
	//Initializing variables 
	bulletObject = nullptr;
	readyToDestroy = false;
	lifeTime = life;
}

bool Bullet::Initialize(ID3D11Device * myDevice, const char * fileName, float3 forward, float3 position)
{
	bool result;

	bulletObject = new GameObject("Assets/Sphere.mesh", myDevice);
	if (!bulletObject)
	{
		return false;
	}

	result = bulletObject->Initialize(myDevice);
	if (!result)
	{
		return false;
	}

	bulletObject->GetPhysicsComponent()->SetForward(forward);
	bulletObject->GetPhysicsComponent()->SetPosition(position);
	velocity = forward * -100.0f;
	bulletObject->GetPhysicsComponent()->SetLifeTime(lifeTime);
	bulletObject->GetPhysicsComponent()->SetVelocity(velocity);
	bulletObject->GetPhysicsComponent()->SetAccel({ 0, -1.0f, 0 });
	bulletObject->GetPhysicsComponent()->SetMass(2.0f);
	bulletObject->GetPhysicsComponent()->SetDamping(0.99f);

	return true;
}

void Bullet::Update(float delta)
{
	bulletObject->Update(delta);
	if (bulletObject->GetPhysicsComponent()->GetLifeTime() <= 0.0f)
	{
		readyToDestroy = true;
	}
}

bool Bullet::Render(ID3D11DeviceContext * myContext)
{
	bulletObject->Render(myContext);
	return true;
}

void Bullet::Shutdown()
{
	if (bulletObject)
	{
		bulletObject->Shutdown();
		delete bulletObject;
		bulletObject = nullptr;
	}
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
	return readyToDestroy
;
}

GameObject *Bullet::GetGameObject()
{
	if (bulletObject)
	{
		return bulletObject;
	}
	else
	{
		return nullptr;
	}
}

float3 Bullet::GetPosition()
{
	return bulletObject->GetPhysicsComponent()->GetPosition();
}
