#include "header.h"
#include "Camera.h"


Camera::Camera()
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
	lookAtVect = {0, 0, 1};
}

//Sets the position variables 
void Camera::SetPosition(float x, float y, float z)
{
	myPosX = x;
	myPosY = y;
	myPosZ = z;
}

//Set the rotation variables 
void Camera::SetRotation(float x, float y, float z)
{
	myRotX = x;
	myRotY = y;
	myRotZ = z;
}

//Camera position getter
XMFLOAT3 Camera::GetPosition()
{
	return XMFLOAT3(myPosX, myPosY, myPosZ);
}

//Camera rotation getter
XMFLOAT3 Camera::GetRotation()
{
	return XMFLOAT3(myRotX, myRotY, myRotZ);
}

//Update the camera position and rotation with the current data 
void Camera::Update()
{
	XMVECTOR DefaultForward = { 0.0f, 0.0f, 1.0f, 0.0f };
	XMVECTOR DefaultRight = { 1.0f, 0.0f, 0.0f, 0.0f };
	XMVECTOR camforward = { 0.0f, 0.0f, 1.0f, 0.0f };
	XMVECTOR camRight = { 1.0f, 0.0f, 0.0f, 0.0f };


	XMFLOAT3 up, position, lookAt;
	//float yaw, pitch, roll;
	XMMATRIX rotationMatrix;
	//XMMATRIX groundWorld;

	upVect = { 0.0f, 1.0f, 0.0f, 0.0f };
	////Set up the up vector
	//up.x = 0.0f;
	//up.y = 1.0f;
	//up.z = 0.0f;

	//upVect = XMLoadFloat3(&up);

	////Set up the position vector 
	//position.x = myPosX;
	//position.y = myPosY;
	//position.z = myPosZ;

	//positionVect = XMLoadFloat3(&position);

	////Set up the look at vector 
	//lookAt.x = 0.0f;
	//lookAt.y = 0.0f;
	//lookAt.z = 1.0f;

	//lookAtVect = XMLoadFloat3(&lookAt);

	////Convert rotation to degrees
	//pitch = myRotX * 0.0174532925f;
	//yaw = myRotY * 0.0174532925f;
	//roll = myRotZ * 0.0174532925f;

	//Set the rotation matrix 
	rotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	lookAtVect = XMVector3TransformCoord(DefaultForward, rotationMatrix);
	lookAtVect = XMVector3Normalize(lookAtVect);

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(camYaw);

	camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
	upVect = XMVector3TransformCoord(upVect, RotateYTempMatrix);
	camforward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);

	positionVect += moveLeftRight * camRight;
	positionVect += moveBackForawrd * camforward;

	moveLeftRight = 0.0f;
	moveBackForawrd = 0.0f;

	lookAtVect = positionVect + lookAtVect;

	myViewMatrix = XMMatrixLookAtLH(positionVect, lookAtVect, upVect);

	////Rotate the look at and up vectors 
	//lookAtVect = XMVector3TransformCoord(lookAtVect, rotationMatrix);
	//upVect = XMVector3TransformCoord(upVect, rotationMatrix);

	////Add the position and look at 
	//lookAtVect = XMVectorAdd(positionVect, lookAtVect);

	////Set the view matrix 
	//myViewMatrix = XMMatrixLookAtLH(positionVect, lookAtVect, upVect);
}

//Pass in the view matrix data into another matrix 
void Camera::PassInViewMatrix(XMMATRIX &other)
{
	other = myViewMatrix;
}

void Camera::GetInput(InputManager *myInput, float time)
{
	float speed = 5.0f * time;

	if (myInput->GetKeyState((int)'W'))
	{
		moveBackForawrd += speed;
	}

	if (myInput->GetKeyState((int)'S'))
	{
		moveBackForawrd -= speed;
	}

	if (myInput->GetKeyState((int)'A'))
	{
		moveLeftRight -= speed;
	}

	if (myInput->GetKeyState((int)'D'))
	{
		moveLeftRight += speed;
	}

	if (myInput->GetKeyState(_ARROWLEFT))
	{
		camYaw -= speed * .5f;
	}

	if (myInput->GetKeyState(_ARROWRIGHT))
	{
		camYaw += speed * .5f;
	}

	if (myInput->GetKeyState(_ARROWUP))
	{
		camPitch -= speed * .5f;
	}

	if (myInput->GetKeyState(_ARROWDOWN))
	{
		camPitch += speed * .5f;
	}
}


