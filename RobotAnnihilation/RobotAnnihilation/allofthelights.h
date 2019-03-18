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
	XMFLOAT4 SpotlightColor;
	XMFLOAT4 SpotlightDirection;
	XMFLOAT4 SpotlightPosition;
	XMFLOAT4 SpotlightExtra;

public:
	Lighting();

	void SetDirectionalColor(float r, float g, float b, float a);
	void SetDirection(float x, float y, float z);
	void SetSpotlightColor(float r, float g, float b, float a);
	void SetSpotlightDirection(float x, float y, float z, float w);
	void SetSpotlightPosition(float x, float y, float z, float w);
	void SetSpotlightExtra(float a, float b, float c, float d);

	XMFLOAT4 GetDirectionalColor();
	XMFLOAT3 GetDirectionalDirection();
	XMFLOAT4 GetSpotlightColor();
	XMFLOAT4 GetSpotlightDirection();
	XMFLOAT4 GetSpotlightPosition();
	XMFLOAT4 GetSpotlightExtra();
};

#endif // !_ALLOFTHELIGHTS_H_
