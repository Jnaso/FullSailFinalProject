#include "GameObject.h"

GameObject::GameObject(int animationAmounts, int soundAmounts)
{
	objectModel = new Model();
	objectAnimations = new Animation*[animationAmounts];
	objectSounds = new Sound*[soundAmounts];
	AnimationCount = animationAmounts;
	SoundCount = soundAmounts;
}

GameObject::GameObject()
{
	objectModel = new Model();
}

GameObject::~GameObject()
{
}

void GameObject::Update(float delta)
{
	objectAnimations[currentAnimation]->Update(delta);
	ObjectPhysics->Update(delta);
}

void GameObject::AddAninimation(const char * filePath, ID3D11Device * device, int index)
{
	objectAnimations[index] = new Animation(filePath, device);
}

PhysicsComponent * GameObject::GetPhysicsComponent()
{
	return ObjectPhysics;
}

void GameObject::SetPhysicsComponent(PhysicsComponent * newPhysics)
{
	ObjectPhysics = newPhysics;
}

bool GameObject::Initialize(const char* filePath, ID3D11Device* device)
{
	objectModel->Initialize(filePath, device);
}

void GameObject::Render(ID3D11DeviceContext* context)
{
	objectModel->Render(context);
}

void GameObject::Shutdown()
{
	objectModel->Shutdown();
	delete[] objectAnimations;
	delete ObjectPhysics;
}
