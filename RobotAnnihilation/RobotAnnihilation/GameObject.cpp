#include "GameObject.h"

GameObject::GameObject()
{
	objectModel = new Model();
	ObjectPhysics = new PhysicsComponent();
}

GameObject::~GameObject()
{
}

int GameObject::GetParent(int index, std::vector<int32_t> parents)
{
	if (parents[index] == -1)
	{
		return -1;
	}
	if (parents[index] == 0)
	{
		return index;
	}
	else
	{
		return GetParent(parents[index], parents);
	}
}


float4x4 GameObject::RecursiveJointCalc(unsigned int currentIndex, std::vector<float4x4>& joints, std::vector<int32_t>& parents)
{
	if (parents[currentIndex] == -1)
	{
		return joints[currentIndex];
	}
	else
	{
		return XMMatrixToFloat4x4(XMMatrixMultiply(Float4x4ToXMMatrix(joints[currentIndex]), Float4x4ToXMMatrix(RecursiveJointCalc(parents[currentIndex], joints, parents))));
	}
}

std::vector<float4x4> GameObject::Flatten(std::vector<float4x4> joints, std::vector<int32_t> parents)
{
	std::vector<float4x4> NewJoints;
	for (unsigned int i = 0; i < joints.size(); i++)
	{
		float4x4 joint = RecursiveJointCalc(i, joints, parents);
		NewJoints.push_back(joint);
	}
	return NewJoints;
}

void GameObject::Update(float delta)
{
	ObjectPhysics->Update(delta);
	lifeTime -= delta;
	totalTime += .025f;
	if (currentAnimation)	
	{
		currentAnimation->Update(delta);
	}
}

void GameObject::AddAninimation(const char * filePath, ID3D11Device * device, int index, bool split)
{
	objectAnimations.resize(objectAnimations.size() + 1);
	objectAnimations[index] = new Animation(filePath, device, split);
	if (split)
	{
		AnimationParents.clear();
		for (size_t i = 0; i < objectAnimations[index]->GetAnimationClip().parent_indicies.size(); i++)
		{
			AnimationParents.push_back(GetParent(i, objectAnimations[index]->GetAnimationClip().parent_indicies));
		}
	}
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
	size_t i;
	objectModel->Shutdown();
	for (i = 0; i < objectAnimations.size(); i++)
	{
		Animation* temp;
		temp = objectAnimations[i];
		objectAnimations.erase(objectAnimations.begin() + i);
		delete temp;
	}
	for (i = 0; i < objectSounds.size(); i++)
	{
		Sound* temp;
		temp = objectSounds[i];
		objectSounds.erase(objectSounds.begin() + i);
		delete temp;
	}

	colliders.clear();
	delete objectModel;
	delete ObjectPhysics;
}

void GameObject::AddCollider(float3 pos, float rad)
{
	Sphere newSphere;
	newSphere.center = pos;
	newSphere.radius = rad;
	colliders.push_back(newSphere);
}

Sphere *GameObject::GetCollider(int index)
{
	if (index < 0)
	{
		return &colliders[0];
	}
	else if (index >= colliders.size())
	{
		return &colliders[colliders.size() - 1];
	}
	else
	{
		return &colliders[index];
	}
}