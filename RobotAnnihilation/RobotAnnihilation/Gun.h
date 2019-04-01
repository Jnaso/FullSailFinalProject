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
	bool Initialize(const char* filePath, ID3D11Device* device);
	void Render(ID3D11DeviceContext* context);
	void Shutdown();

	void Update(float delta);

	float GetFireRate() { return FireRate; };
	void SetFireRate(float NewFireRate) { FireRate = NewFireRate; };

	float GetDamageAmount() { return FireRate; };
	void SetDamageAmount(float damage) { DamageAmount = damage; };

	float3 GetShootPos() { return ShootPos; };
	void SetShootPos(float3 newPos) { ShootPos = newPos; };
};

