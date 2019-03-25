#include "GameObject.h"

GameObject::GameObject()
{
	objectModel = new Model();
	ObjectPhysics = new PhysicsComponent();
}

GameObject::~GameObject()
{
}

void GameObject::Update(float delta)
{
	if (TimeLeft >= 0)
	{
		TimeLeft -= delta;
	}
	currentAnimation->Update(delta);
	ObjectPhysics->Update(delta);
}

void GameObject::AddAninimation(const char * filePath, ID3D11Device * device, int index)
{
	objectAnimations.resize(objectAnimations.size() + 1);
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

Model * GameObject::GetModelComponent()
{
	return objectModel;
}

void GameObject::SetAnimation(int index)
{
	currentAnimation = objectAnimations[index];
}

Animation * GameObject::GetCurrentAnimation()
{
	return currentAnimation;
}

bool GameObject::Initialize(const char* filePath, ID3D11Device* device)
{
	return objectModel->Initialize(filePath, device);
}

void GameObject::Render(ID3D11DeviceContext* context)
{
	objectModel->Render(context);
}

void GameObject::Shutdown()
{
	objectModel->Shutdown();
	for (size_t i = 0; i < objectAnimations.size(); i++)
	{
		Animation* temp;
		temp = objectAnimations[i];
		objectAnimations.erase(objectAnimations.begin() + i);
		delete temp;
	}
	for (size_t i = 0; i < objectSounds.size(); i++)
	{
		Sound* temp;
		temp = objectSounds[i];
		objectSounds.erase(objectSounds.begin() + i);
		delete temp;
	}
	delete objectModel;
	delete ObjectPhysics;
}
