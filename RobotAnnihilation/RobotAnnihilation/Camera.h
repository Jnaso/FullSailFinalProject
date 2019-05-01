#pragma once
#ifndef _CAMERA_H
#define _CAMERA_H_

#include <DirectXMath.h>
#include "InputManager.h"
#include "MathDefines.h"
#include "Player.h"
using namespace DirectX;

class Camera
{
private:
	float myPosX, myPosY, myPosZ;
	float myRotX, myRotY, myRotZ;
	XMMATRIX myViewMatrix;
	XMMATRIX rotationMatrix;
	float moveLeftRight = 0.0f;
	float moveBackForawrd = 0.0f;
	float camYaw = 0.0f;
	float camPitch = 0.0f;
	XMVECTOR upVect, positionVect, lookAtVect, DefaultForward;
	XMVECTOR currCharDirection, oldCharDirection, charPosition, camRight, camforward;

	float deltax, deltay;

public:
	Camera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	void GetInput(InputManager *manager, float speed, XMMATRIX &, Player *myPlayer);


	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();
	XMVECTOR GetCharDirection();
	XMVECTOR GetCamDirection();
	XMVECTOR desiredCharDir;
	float upMovement;

	void Update(XMFLOAT3 newLookAt);
	void PassInViewMatrix(XMMATRIX&);

	void SetCharacterPosition(double time, XMVECTOR& destinationDirection, XMMATRIX& worldMatrix, bool slowed);
};

#endif // !_CAMERA_H