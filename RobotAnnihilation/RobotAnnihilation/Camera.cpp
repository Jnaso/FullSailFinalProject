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

	//Make the camera look at the target(player)
	lookAtVect = target;
	//Make the camera loom over the player 
	lookAtVect = XMVectorSetY(lookAtVect, XMVectorGetY(lookAtVect) + 2.0f);

	//Rotate the camera by desired rotation 
	rotationMatrix = XMMatrixRotationRollPitchYaw(-camPitch, camYaw, 0);
	//Move the camer by its forward 
	positionVect = XMVector3TransformNormal(DefaultForward, rotationMatrix);
	positionVect = XMVector3Normalize(positionVect);

	//Distance the camera from the target 
	positionVect = (positionVect * 7.0f) + lookAtVect;

	//Constantly update the camera forward to be in local space 
	camforward = XMVector3Normalize(lookAtVect - positionVect);
	camforward = XMVectorSetY(camforward, 0.0f);
	camforward = XMVector3Normalize(camforward);

	//Constantly update the camera right to be in local space 
	camRight = XMVectorSet(-XMVectorGetZ(camforward), 0.0f, XMVectorGetX(camforward), 0.0f);

	//Constantly update the camera up to be in local space 
	upVect = XMVector3Cross(XMVector3Normalize(positionVect - lookAtVect), camRight);

	//Set the view 
	myViewMatrix = XMMatrixLookAtLH(positionVect, lookAtVect, upVect);

}

//Pass in the view matrix data into another matrix 
void Camera::PassInViewMatrix(XMMATRIX &other)
{
	other = myViewMatrix;
}

//Move the camera based on input 
void Camera::GetInput(InputManager *myInput, float time, XMMATRIX& player)
{
	//Speed of movement
	float speed = 5.0f * time;
	bool moveChar = false;
	XMVECTOR desiredCharDir; 

	//Update the chracter's direction and the camera's movement on each press 
	if (myInput->GetKeyState((int)'W'))
	{
		desiredCharDir = camforward;
		moveChar = true;
	}

	if (myInput->GetKeyState((int)'S'))
	{
		desiredCharDir = -camforward;
		moveChar = true;
	}

	if (myInput->GetKeyState((int)'A'))
	{
		desiredCharDir = camRight;
		moveChar = true;
	}

	if (myInput->GetKeyState((int)'D'))
	{
		desiredCharDir = -camRight;
		moveChar = true;
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
		//Only move the player if they've moved 
		SetCharacterRotation(time, desiredCharDir, player);
	}
}

//Move the player's based 
void Camera::SetCharacterRotation(double time, XMVECTOR& destinationDirection, XMMATRIX& worldMatrix)
{
	//Translate the chracter 
	charPosition = { 0.0f, 0.0f, 0.0f, 0.0f };
	charPosition = XMVector3TransformCoord(charPosition, worldMatrix);

	//Set the desired direction to the direction of movement 
	currCharDirection = -destinationDirection;

	//Calculate the angle the character needs to rotate 
	float charDirAngle = XMVectorGetX(XMVector3AngleBetweenNormals(XMVector3Normalize(currCharDirection), XMVector3Normalize(DefaultForward)));

	//Negate the character's direction if their y vector is positive 
	if (XMVectorGetY(XMVector3Cross(currCharDirection, DefaultForward)) > 0.0f)
	{
		charDirAngle = -charDirAngle;
	}

	//Move the character by a speed 
	float speed = .4f;
	charPosition = charPosition + (destinationDirection * speed);

	//Translate the character 
	XMMATRIX Translation = XMMatrixTranslation(XMVectorGetX(charPosition), 0.0f, XMVectorGetZ(charPosition));
	//Rotate the character(subtract by pi so player doesn't run backwards)
	XMMATRIX rotation = XMMatrixRotationY(charDirAngle - 3.14159265f);

	//Update player's world matrix
	worldMatrix = rotation * Translation;
}


