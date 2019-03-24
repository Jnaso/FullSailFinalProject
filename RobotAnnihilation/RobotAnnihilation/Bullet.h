#pragma once
#ifndef _BULLET_H_
#define _BULLET_H_

#include "GameObject.h"

class Bullet
{
private:
	GameObject *bulletObject;
	float3 velocity;
	float lifeTime;
	bool readyToDestroy;

public:
	Bullet(float life = 2.0f);

	bool Initialize(ID3D11Device* myDevice, const char *fileName, float3 forward, float3 position);
	void Update(float delta);
	bool Render(ID3D11DeviceContext* myContext);
	void Shutdown();
	void SetVelocity(float3 vel);
	float3 GetVelocity();
	float3 GetPosition();
	bool Destroy();
	GameObject *GetGameObject();
};

#endif // !_BULLET_H_
