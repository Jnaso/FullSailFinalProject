#pragma once
#include "Model.h"
class Gun
{
protected:
	float FireRate;
	float DamageAmount;

	float3 ShootPos;

	Model* gunModel;

public:
	Gun();
	~Gun();
	virtual bool Initialize(const char* filePath, ID3D11Device* device);
	virtual void Render(ID3D11DeviceContext* context);
	virtual void Shutdown();

	virtual void Update(float delta);

	float GetFireRate() { return FireRate; };
	void SetFireRate(float FireRate) { FireRate = FireRate; };

	float GetDamageAmount() { return FireRate; };
	void SetDamageAmount(float damage) { DamageAmount = damage; };

	float3 GetShootPos() { return ShootPos; };
	void SetShootPos(float3 newPos) { ShootPos = newPos; };
};

