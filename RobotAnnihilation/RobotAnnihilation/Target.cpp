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

void Target::Update(float delta, float3 forward)
{
	GameObject::Update(delta);
	float3 forward2 = forward - GetPhysicsComponent()->GetPosition();
	GetPhysicsComponent()->SetForward(forward2);
	GetPhysicsComponent()->SetVelocity(forward2 * 0.3f);
	GetPhysicsComponent()->SetPosition({ GetPhysicsComponent()->GetPosition().x, 2.0f, GetPhysicsComponent()->GetPosition().z});
	GetCollider(0)->center = { GetPhysicsComponent()->GetPosition().x, GetPhysicsComponent()->GetPosition().y, GetPhysicsComponent()->GetPosition().z};
	//std::cout << GetCollider(0).center.x << " " << GetCollider(0).center.y << " " << GetCollider(0).center.z << " " << std::endl;
}

AABB Target::GetAABB()
{
	return myCollision;
}
