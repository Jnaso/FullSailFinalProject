#include "DebugCamera.h"

DebugCamera::DebugCamera()
{
	//Initialize all variables
	myPosX = 0;
	myPosY = 0;
	myPosZ = 0;
	myRotX = 0;
	myRotY = 0;
	myRotZ = 0;

	positionVect = { 0, 3, -5 };
	upVect = { 0, 1, 0 };
	lookAtVect = { 0, 0, 1 };
	currCharDirection = { 0.0f, 0.0f, 0.0f };
	oldCharDirection = { 0.0f, 0.0f, 0.0f };
	charPosition = { 0.0f, 0.0f, 0.0f };
	DefaultForward = { 0.0f, 0.0f, 1.0f, 0.0f };
	camforward = { 0.0f, 0.0f, 1.0f, 0.0f };
	camRight = { 1.0f, 0.0f, 0.0f, 0.0f };
}

//Set the camera's position variables
void DebugCamera::SetPosition(float x, float y, float z)
{
	myPosX = x;
	myPosY = y;
	myPosZ = z;
}

//Set the camera's rotation variables 
void DebugCamera::SetRotation(float x, float y, float z)
{
	myRotX = x;
	myRotY = y;
	myRotZ = z;
}

//Camera position getter
XMFLOAT3 DebugCamera::GetPosition()
{
	return XMFLOAT3(myPosX, myPosY, myPosZ);
}

//Camera rotation getter
XMFLOAT3 DebugCamera::GetRotation()
{
	return XMFLOAT3(myRotX, myRotY, myRotZ);
}

//Pass in the camera's view matrix into another matrix
void DebugCamera::PassInViewMatrix(XMMATRIX &other)
{
	other = myViewMatrix;
}

//Update the camera's values over time
void DebugCamera::Update()
{
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	lookAtVect = XMVector3TransformCoord(DefaultForward, rotationMatrix);
	lookAtVect = XMVector3Normalize(lookAtVect);

	XMMATRIX tempMatrix = XMMatrixRotationY(camYaw);

	camRight = XMVector3TransformCoord(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), tempMatrix);
	upVect = XMVector3TransformCoord(upVect, tempMatrix);
	camforward = XMVector3TransformCoord(DefaultForward, tempMatrix);

	positionVect += moveLeftRight * camRight;
	positionVect += moveBackForawrd * camforward;
	positionVect += moveUpDown * upVect;

	moveLeftRight = 0.0f;
	moveBackForawrd = 0.0f;
	moveUpDown = 0.0f;

	lookAtVect = positionVect + lookAtVect;

	myViewMatrix = XMMatrixLookAtLH(positionVect, lookAtVect, upVect);
}

//Manipulate the camera with input
void DebugCamera::GetInput(InputManager *myManager, float time)
{
	float speed = 15.0f * time;

	if (myManager->GetKeyState((int)'W'))
	{
		moveBackForawrd += speed;
	}

	if (myManager->GetKeyState((int)'S'))
	{
		moveBackForawrd -= speed;
	}

	if (myManager->GetKeyState((int)'A'))
	{
		moveLeftRight -= speed;
	}

	if (myManager->GetKeyState((int)'D'))
	{
		moveLeftRight += speed;
	}

	if (myManager->GetKeyState((int)'X'))
	{
		moveUpDown += speed;
	}

	if (myManager->GetKeyState(_SPACE))
	{
		moveUpDown -= speed;
	}

	if (myManager->GetKeyState(_ARROWLEFT))
	{
		camYaw -= speed * .3f;
	}

	if (myManager->GetKeyState(_ARROWRIGHT))
	{
		camYaw += speed * .3f;
	}

	if (myManager->GetKeyState(_ARROWUP))
	{
		if (camPitch >= -.3f)
		{
			camPitch -= speed * .3f;
		}
	}

	if (myManager->GetKeyState(_ARROWDOWN))
	{
		if (camPitch <= 0.5f)
		{
			camPitch += speed * .3f;
		}
	}
}