#pragma once
#ifndef _DEBUGCAMERA_H
#define _DEBUGCAMERA_H_

#include <DirectXMath.h>
#include "InputManager.h"
using namespace DirectX;

class DebugCamera
{
private:
	float myPosX, myPosY, myPosZ;
	float myRotX, myRotY, myRotZ;
	XMMATRIX myViewMatrix;
	float moveLeftRight = 0.0f;
	float moveBackForawrd = 0.0f;
	float moveUpDown = 0.0f;
	float camYaw = 0.0f;
	float camPitch = 0.0f;
	XMVECTOR upVect, positionVect, lookAtVect, DefaultForward;
	XMVECTOR currCharDirection, oldCharDirection, charPosition, camRight, camforward;

public:
	DebugCamera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	void GetInput(InputManager *manager, float speed);


	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Update();
	void PassInViewMatrix(XMMATRIX&);

};

#endif // !_DEBUGCAMERA_H