#pragma once
#ifndef _TARGET_H_
#define _TARGET_H_

#include "GameObject.h"

class Target : public GameObject
{
private:
	bool readyToDestroy;
	AABB myCollision;
	float velocity;

public:
	Target();

	bool Initialize(ID3D11Device* myDevice, const char *fileName, float3 position);
	bool Destroy();
	void SetDestroy();
	void Update(float delta, float3 forward);
	AABB GetAABB();
};

#endif // !_TARGET_H_
