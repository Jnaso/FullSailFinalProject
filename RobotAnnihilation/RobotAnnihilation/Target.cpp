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

	AddCollider(GetPhysicsComponent()->GetPosition(), 1.0f);
	myCollision.center = GetPhysicsComponent()->GetPosition();
	myCollision.dimensions = { 1.0f, 1.0f, 1.0f };

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

AABB Target::GetAABB()
{
	return myCollision;
}
