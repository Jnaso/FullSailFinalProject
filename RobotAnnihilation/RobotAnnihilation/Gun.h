#pragma once
#include "Model.h"
class Gun
{
	float FireRate;
	float DamageAmount;

	float3 ShootPos;

	Model* gunModel;

public:
	virtual bool Initialize(const char* filePath, ID3D11Device* device);
	virtual void Render(ID3D11DeviceContext* context);
	virtual void Shutdown();

	virtual void Update(float delta);
	Gun();
	~Gun();

	float GetFireRate() { return FireRate; };
	void SetFireRate(float FireRate) { FireRate = FireRate; };

	float GetDamageAmonut() { return FireRate; };
	void SetDamageAmonut(float damage) { DamageAmount = damage; };
};

