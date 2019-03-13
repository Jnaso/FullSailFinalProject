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
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVect, positionVect, lookAtVect;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	//Set up the up vector
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	upVect = XMLoadFloat3(&up);

	//Set up the position vector 
	position.x = myPosX;
	position.y = myPosY;
	position.z = myPosZ;

	positionVect = XMLoadFloat3(&position);

	//Set up the look at vector 
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	lookAtVect = XMLoadFloat3(&lookAt);

	//Convert rotation to degrees
	pitch = myRotX * 0.0174532925f;
	yaw = myRotY * 0.0174532925f;
	roll = myRotZ * 0.0174532925f;

	//Set the rotation matrix 
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	//Rotate the look at and up vectors 
	lookAtVect = XMVector3TransformCoord(lookAtVect, rotationMatrix);
	upVect = XMVector3TransformCoord(upVect, rotationMatrix);

	//Add the position and look at 
	lookAtVect = XMVectorAdd(positionVect, lookAtVect);

	//Set the view matrix 
	myViewMatrix = XMMatrixLookAtLH(positionVect, lookAtVect, upVect);
}

//Pass in the view matrix data into another matrix 
void Camera::PassInViewMatrix(XMMATRIX &other)
{
	other = myViewMatrix;
}


