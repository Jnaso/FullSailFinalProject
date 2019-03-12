#pragma once
#ifndef _ALLOFTHELIGHTS_H_
#define _ALLOFTHELIGHTS_H_

#include <DirectXMath.h>

using namespace DirectX;

class Lighting
{
private:
	XMFLOAT4 DirectionalColor;
	XMFLOAT3 DirectionalDirection;

public:
	Lighting();

	void SetDirectionalColor(float r, float g, float b, float a);
	void SetDirection(float x, float y, float z);

	XMFLOAT4 GetDirectionalColor();
	XMFLOAT3 GetDirectionalDirection();
};

#endif // !_ALLOFTHELIGHTS_H_
