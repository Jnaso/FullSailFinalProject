#pragma once
#include "MathDefines.h"

class PhysicsComponent
{
	float3 position;
	float3 velocity;
	float3 acceleration;
	float RotX = 0;
	float RotY = 0;
	float RotZ = 0;
	float damping;
	float inverseMass;
	float3 forceAccum;

	void clearAccumulator();

public:
	PhysicsComponent();
	~PhysicsComponent();
	float3 GetPosition();
	void SetPosition(float3 newPos);
	float3 GetVelocity();
	void SetVelocity(float3 newVel);
	float3 GetAccel();
	void SetAccel(float3 newAcc);
	void Update(float delta);
	void SetMass(float val);
	float GetMass();
	void SetInverseMass(float val);
	float GetInverseMass();
	void SetDamping(float val);
	float GetDamping();
	void AddForce(const float3& vec);
};

