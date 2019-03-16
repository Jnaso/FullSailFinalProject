#include "PhysicsComponent.h"



PhysicsComponent::PhysicsComponent()
{
	position = float3{ 0, 0, 0 };
	velocity = float3{ 0, 0, 0 };
	acceleration = float3{ 0, 0, 0 };
	RotX = 0;
	RotY = 0;
	RotZ = 0;
}


PhysicsComponent::~PhysicsComponent()
{
}

float3 PhysicsComponent::GetPosition()
{
	return position;
}

void PhysicsComponent::SetPosition(float3 newPos)
{
	position = newPos;
}

float3 PhysicsComponent::GetVelocity()
{
	return velocity;
}

void PhysicsComponent::SetVelocity(float3 newVel)
{
	velocity = newVel;
}

float3 PhysicsComponent::GetAccel()
{
	return acceleration;
}

void PhysicsComponent::SetAccel(float3 newAcc)
{
	acceleration = newAcc;
}

void PhysicsComponent::Update(float delta)
{
	float3 velocitydelta;
	velocitydelta.x = velocity.x * delta;
	velocitydelta.y = velocity.y * delta;
	velocitydelta.z = velocity.z * delta;
	position += velocitydelta;
	if (velocity.x >= 0)
	{
		velocity.x -= delta;
	}
	velocity.y -= delta;
	if (velocity.z >= 0)
	{
		velocity.z -= delta;
	}
	//velocity -= delta;
	if (position.y < 0)
	{
		position.y = 0;
	}
}
