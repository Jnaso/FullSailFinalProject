#pragma once
#include "MathDefines.h"

class PhysicsComponent
{
	float3 position;
	float3 velocity;
	float3 acceleration;
	float damping;
	float inverseMass;
	float3 forceAccum;
	float3 forward;
	float3x3 rotation;

	float lifeTime;

	void clearAccumulator();

public:
	PhysicsComponent();
	~PhysicsComponent();
	float3 GetPosition();
	void SetPosition(float3 newPos);
	float3x3 GetRotation();
	void SetRotation(float3x3 newRot);
	void AddPosition(float3 newPos);
	float3 GetVelocity();
	void SetVelocity(float3 newVel);
	void AddVelocity(float3 newVel);
	float3 GetForward();
	void SetForward(float3 newFor);
	float3 GetAccel();
	void SetAccel(float3 newAcc);
	void Update(float delta);
	void SetMass(float val);
	float GetMass();
	void SetInverseMass(float val);
	float GetInverseMass();
	void SetLifeTime(float val);
	float GetLifeTime();
	void SetDamping(float val);
	float GetDamping();
	void AddForce(const float3& vec);
};

