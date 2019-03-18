#include "PhysicsComponent.h"



PhysicsComponent::PhysicsComponent()
{
	position = float3{ 0, 0, 0 };
	velocity = float3{ 0, 0, 0 };
	acceleration = float3{ 0, 0, 0 };
	RotX = 0;
	RotY = 0;
	RotZ = 0;
	inverseMass = 0;
	clearAccumulator();
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
	if (inverseMass == 0.0f) return;

	position.addScaledVec(velocity, delta);

	float3 resultingAcceleration = acceleration;
	resultingAcceleration.addScaledVec(forceAccum, inverseMass);

	velocity.addScaledVec(resultingAcceleration, delta);

	velocity = velocity * powf(damping, delta);
	clearAccumulator();
	if (position.y < 0)
	{
		position.y = 0;
	}
}

void PhysicsComponent::SetMass(float val)
{
	inverseMass = 1.0f / val;
}

float PhysicsComponent::GetMass()
{
	return 1.0f / inverseMass;
}

void PhysicsComponent::SetInverseMass(float val)
{
	inverseMass = val;
}

float PhysicsComponent::GetInverseMass()
{
	return inverseMass;
}

void PhysicsComponent::SetDamping(float val)
{
	damping = val;
}

float PhysicsComponent::GetDamping()
{
	return damping;
}

void PhysicsComponent::AddForce(const float3 & vec)
{
	forceAccum += vec;
}

void PhysicsComponent::clearAccumulator()
{
	forceAccum = float3{0, 0, 0};
}
