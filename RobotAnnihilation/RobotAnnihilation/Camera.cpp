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
	currCharDirection = { 0.0f, 0.0f, -10.0f };
	oldCharDirection = { 0.0f, 0.0f, 0.0f };
	charPosition = { 0.0f, 0.0f, 0.0f };
	DefaultForward = { 0.0f, 0.0f, 1.0f, 0.0f };
	camforward = { 0.0f, 0.0f, 1.0f, 0.0f };
	camRight = { 1.0f, 0.0f, 0.0f, 0.0f };
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

//Camera Direction getter
XMVECTOR Camera::GetDirection()
{
	return currCharDirection;
}

//Update the camera position and rotation with the current data 
void Camera::Update(XMFLOAT3 newLookAt)
{
	XMVECTOR DefaultRight = { 1.0f, 0.0f, 0.0f, 0.0f };

	XMFLOAT3 up, position, lookAt;
	//float yaw, pitch, roll;
	XMMATRIX rotationMatrix;
	//XMMATRIX groundWorld;

	XMVECTOR target = { newLookAt.x, newLookAt.y, newLookAt.z, 1.0f };

	lookAtVect = target;
	lookAtVect = XMVectorSetY(lookAtVect, XMVectorGetY(lookAtVect) + 2.0f);

	rotationMatrix = XMMatrixRotationRollPitchYaw(-camPitch, camYaw, 0);
	positionVect = XMVector3TransformNormal(DefaultForward, rotationMatrix);
	positionVect = XMVector3Normalize(positionVect);

	positionVect = (positionVect * 7.0f) + lookAtVect;

	camforward = XMVector3Normalize(lookAtVect - positionVect);
	camforward = XMVectorSetY(camforward, 0.0f);
	camforward = XMVector3Normalize(camforward);

	camRight = XMVectorSet(-XMVectorGetZ(camforward), 0.0f, XMVectorGetX(camforward), 0.0f);

	upVect = XMVector3Cross(XMVector3Normalize(positionVect - lookAtVect), camRight);

	myViewMatrix = XMMatrixLookAtLH(positionVect, lookAtVect, upVect);

}

//Pass in the view matrix data into another matrix 
void Camera::PassInViewMatrix(XMMATRIX &other)
{
	other = myViewMatrix;
}

void Camera::GetInput(InputManager *myInput, float time, XMMATRIX& player)
{
	//Speed of movement
	float speed = 5.0f * time;
	bool moveChar = false;
	XMVECTOR desiredCharDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	//Update the chracter's direction and the camera's movement on each press 
	if (myInput->GetKeyState((int)'W'))
	{
		desiredCharDir = camforward;
		moveChar = true;
		moveBackForawrd -= speed;
	}

	if (myInput->GetKeyState((int)'S'))
	{
		desiredCharDir = -camforward;
		moveChar = true;
		moveBackForawrd += speed;
	}

	if (myInput->GetKeyState((int)'A'))
	{
		desiredCharDir = camRight;
		moveChar = true;
		moveLeftRight -= speed;
	}

	if (myInput->GetKeyState((int)'D'))
	{
		desiredCharDir = -camRight;
		moveChar = true;
		moveLeftRight += speed;
	}

	if (myInput->GetKeyState(_ARROWLEFT))
	{
		camYaw += speed * .5f;
	}

	if (myInput->GetKeyState(_ARROWRIGHT))
	{
		camYaw -= speed * .5f;
	}

	if (myInput->GetKeyState(_ARROWUP))
	{
		if (camPitch >= -.25f)
		{
			camPitch -= speed * .5f;
		}
	}

	if (myInput->GetKeyState(_ARROWDOWN))
	{
		if (camPitch <= 0.5f)
		{
			camPitch += speed * .5f;
		}
	}

	if (moveChar)
	{
		SetCharacterRotation(time, desiredCharDir, player);
	}

	std::cout << camPitch << " " << camYaw << std::endl;
}

void Camera::SetCharacterRotation(double time, XMVECTOR& destinationDirection, XMMATRIX& worldMatrix)
{
	destinationDirection = XMVector3Normalize(destinationDirection);

	if (XMVectorGetX(XMVector3Dot(destinationDirection, oldCharDirection)) == -1)
	{
		oldCharDirection += XMVectorSet(0.02f, 0.0f, -0.02f, 0.0f);
	}

	charPosition = { 0.0f, 0.0f, 0.0f, 0.0f };
	charPosition = XMVector3TransformCoord(charPosition, worldMatrix);

	float destDirLength = 10.0f;

	currCharDirection = -destinationDirection * destDirLength;

	float charDirAngle = XMVectorGetX(XMVector3AngleBetweenNormals(XMVector3Normalize(currCharDirection), XMVector3Normalize(DefaultForward)));

	if (XMVectorGetY(XMVector3Cross(currCharDirection, DefaultForward)) > 0.0f)
	{
		charDirAngle = -charDirAngle;
	}

	float speed = .3f;
	charPosition = charPosition + (destinationDirection * speed);

	XMMATRIX rotation;
	XMMATRIX Translation = XMMatrixTranslation(XMVectorGetX(charPosition), 0.0f, XMVectorGetZ(charPosition));
	rotation = XMMatrixRotationY(charDirAngle - 3.14159265f);

	worldMatrix = rotation * Translation;
	oldCharDirection = currCharDirection;
}


