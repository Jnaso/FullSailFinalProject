#pragma once
#ifndef _CAMERA_H
#define _CAMERA_H_

#include <DirectXMath.h>
using namespace DirectX;

class Camera
{
private:
	float myPosX, myPosY, myPosZ;
	float myRotX, myRotY, myRotZ;
	XMMATRIX myViewMatrix;

public:
	Camera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Update();
	void PassInViewMatrix(XMMATRIX&);
};

#endif // !_CAMERA_H