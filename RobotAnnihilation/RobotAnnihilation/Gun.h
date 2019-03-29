#pragma once
#include "Model.h"
class Gun
{
	float FireRate;
	float DamageAmount;

	Model* gunModel;

public:
	virtual bool Initialize(const char* filePath, ID3D11Device* device);
	virtual void Render(ID3D11DeviceContext* context);
	virtual void Shutdown();

	virtual void Update(float delta);
	Gun();
	~Gun();
};

