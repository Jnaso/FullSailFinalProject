#pragma once
#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include "MathDefines.h"
#include <DirectXMath.h>
using namespace DirectX;

class Frustum
{
private:
	XMFLOAT4 myPlanes[6];

public:
	Frustum();

	void Construct(float screenDepth, XMMATRIX projection, XMMATRIX view);
	
	bool CheckSphere(Sphere sphe);
};

#endif // !_FRUSTUM_H_
