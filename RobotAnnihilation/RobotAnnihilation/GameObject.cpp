#include "GameObject.h"

GameObject::GameObject()
{
	objectModel = new Model();
	ObjectPhysics = new PhysicsComponent();
}

GameObject::~GameObject()
{
}

int GetParent(int index, std::vector<int32_t> parents)
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


float4x4 RecursiveJointCalc(unsigned int currentIndex, std::vector<float4x4>& joints, std::vector<int32_t>& parents)
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

std::vector<float4x4> Flatten(std::vector<float4x4> joints, std::vector<int32_t> parents)
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
	if (currentLowerAnimation != nullptr && currentUpperAnimation != nullptr)
	{
		currentLowerAnimation->SetFrameTime(delta);
		currentUpperAnimation->SetFrameTime(delta);
		AnimationJoints = SetSkeletonLines(currentUpperAnimation->GetAnimationClip(), currentLowerAnimation->GetAnimationClip(), currentUpperAnimation->GetFrameTime(), currentLowerAnimation->GetFrameTime());
	}
}

void GameObject::AddAninimation(const char * filePath, ID3D11Device * device, int index)
{
	objectAnimations.resize(objectAnimations.size() + 1);
	objectAnimations[index] = new Animation(filePath, device);
	AnimationParents.clear();
	for (size_t i = 0; i < objectAnimations[index]->GetAnimationClip().parent_indicies.size(); i++)
	{
		AnimationParents.push_back(GetParent(i, objectAnimations[index]->GetAnimationClip().parent_indicies));
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

void GameObject::SetAnimationUpper(int index)
{
	currentUpperAnimation = objectAnimations[index];
}

void GameObject::SetAnimationLower(int index)
{
	currentLowerAnimation = objectAnimations[index];
}

Animation * GameObject::GetCurrentAnimationUpper()
{
	return currentUpperAnimation;
}

Animation * GameObject::GetCurrentAnimationLower()
{
	return currentLowerAnimation;
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

float4x4 LerpJoint(float4x4 frame1, float4x4 frame2, float ratio)
{
	XMVECTOR result = XMQuaternionSlerp(XMQuaternionRotationMatrix(Float4x4ToXMMatrix(frame1)), XMQuaternionRotationMatrix(Float4x4ToXMMatrix(frame2)), ratio);
	XMMATRIX back = XMMatrixRotationQuaternion(result);
	//((p2 - p1) * ratio + p1);
	XMVECTOR pos;
	pos.m128_f32[0] = ((frame2[3].x - frame1[3].x) * ratio + frame1[3].x);
	pos.m128_f32[1] = ((frame2[3].y - frame1[3].y) * ratio + frame1[3].y);
	pos.m128_f32[2] = ((frame2[3].z - frame1[3].z) * ratio + frame1[3].z);
	pos.m128_f32[3] = ((frame2[3].w - frame1[3].w) * ratio + frame1[3].w);
	back.r[3].m128_f32[0] = pos.m128_f32[0];
	back.r[3].m128_f32[1] = pos.m128_f32[1];
	back.r[3].m128_f32[2] = pos.m128_f32[2];
	back.r[3].m128_f32[3] = pos.m128_f32[3];
	return XMMatrixToFloat4x4(back);
}

std::vector<float4x4> GameObject::SetSkeletonLines(anim_clip animationUpper, anim_clip animationLower, float timeUpper, float timeLower)
{
	std::vector<float4x4> NewJoints;
	
		//float t = time / animation.duration;
		uint32_t frame1Upper = 0;
		uint32_t frame2Upper = 0;
		// Finds what frames the time falls between
		for (uint32_t i = 0; i < animationUpper.frames.size(); i++)
		{
			// Checks if time is before the first keyframe
			if (i == 0 && timeUpper < animationUpper.frames[i].time)
			{
				frame1Upper = animationUpper.frames.size() - 1;
				frame2Upper = i;
				break;
			}
			/*if (timeUpper > animationUpper->GetAnimationClip().frames[i].time && i == animationUpper->GetAnimationClip().frames.size() - 1)
			{
				frame1Upper = i;
				frame2Upper = 0;
				break;
			}*/
			// Checks if time falls between current and next keyframe
			if (timeUpper > animationUpper.frames[i].time && timeUpper < animationUpper.frames[i + 1].time)
			{
				frame1Upper = i;
				frame2Upper = i + 1;
				break;
			}
		}
		float ratioUpper = 0;
		if (frame2Upper == 0)
			ratioUpper = (timeUpper - (animationUpper.frames[frame1Upper].time - animationUpper.duration)) / (animationUpper.frames[frame2Upper].time - (animationUpper.frames[frame1Upper].time - animationUpper.duration));
		else
			ratioUpper = (timeUpper - animationUpper.frames[frame1Upper].time) / (animationUpper.frames[frame2Upper].time - animationUpper.frames[frame1Upper].time);
		uint32_t frame1Lower = 0;
		uint32_t frame2Lower = 0;
		// Finds what frames the time falls between
		for (uint32_t i = 0; i < animationLower.frames.size(); i++)
		{
			// Checks if time is before the first keyframe
			if (i == 0 && timeLower < animationLower.frames[i].time)
			{
				frame1Lower = animationLower.frames.size() - 1;
				frame2Lower = i;
				break;
			}
			/*if (timeLower > animationLower->GetAnimationClip().frames[i].time && i == animationLower->GetAnimationClip().frames.size() - 1)
			{
				frame1Lower = i;
				frame2Lower = 0;
				break;
			}*/
			// Checks if time falls between current and next keyframe
			if (timeLower > animationLower.frames[i].time && timeLower < animationLower.frames[i + 1].time)
			{
				frame1Lower = i;
				frame2Lower = i + 1;
				break;
			}
		}
		float ratioLower = 0;
		if (frame2Lower == 0)
			ratioLower = (timeLower - (animationLower.frames[frame1Lower].time - animationLower.duration)) / (animationLower.frames[frame2Lower].time - (animationLower.frames[frame1Lower].time - animationLower.duration));
		else
			ratioLower = (timeLower - animationLower.frames[frame1Lower].time) / (animationLower.frames[frame2Lower].time - animationLower.frames[frame1Lower].time);
		// Calculate lerp between keyframes using ratio calculated above
		std::vector<float4x4> Bindpose = animationLower.bindPose.joints;
		std::vector<float4x4> Frame1UpperJoints = animationUpper.frames[frame1Upper].joints;
		std::vector<float4x4> Frame2UpperJoints = animationUpper.frames[frame2Upper].joints;
		std::vector<float4x4> Frame1LowerJoints = animationLower.frames[frame1Lower].joints;
		std::vector<float4x4> Frame2LowerJoints = animationLower.frames[frame2Lower].joints;
		float4x4 LerpRootUpper = LerpJoint(Frame1UpperJoints[0], Frame2UpperJoints[0], ratioUpper);
		float4x4 LerpRootLower = LerpJoint(Frame1LowerJoints[0], Frame2LowerJoints[0], ratioLower);
		NewJoints.push_back(XMMatrixToFloat4x4(XMMatrixTranspose(XMMatrixMultiply(Float4x4ToXMMatrix(Bindpose[0]), Float4x4ToXMMatrix(LerpJoint(LerpRootUpper, LerpRootLower, .5f))))));
		LerpRootUpper = LerpJoint(Frame1UpperJoints[1], Frame2UpperJoints[1], ratioUpper);
		LerpRootLower = LerpJoint(Frame1LowerJoints[1], Frame2LowerJoints[1], ratioLower);
		NewJoints.push_back(XMMatrixToFloat4x4(XMMatrixTranspose(XMMatrixMultiply(Float4x4ToXMMatrix(Bindpose[1]), Float4x4ToXMMatrix(LerpJoint(LerpRootUpper, LerpRootLower, .25f))))));
		LerpRootUpper = LerpJoint(Frame1UpperJoints[2], Frame2UpperJoints[2], ratioUpper);
		LerpRootLower = LerpJoint(Frame1LowerJoints[2], Frame2LowerJoints[2], ratioLower);
		NewJoints.push_back(XMMatrixToFloat4x4(XMMatrixTranspose(XMMatrixMultiply(Float4x4ToXMMatrix(Bindpose[2]), Float4x4ToXMMatrix(LerpJoint(LerpRootUpper, LerpRootLower, .75f))))));
		LerpRootUpper = LerpJoint(Frame1UpperJoints[3], Frame2UpperJoints[3], ratioUpper);
		LerpRootLower = LerpJoint(Frame1LowerJoints[3], Frame2LowerJoints[3], ratioLower);
		NewJoints.push_back(XMMatrixToFloat4x4(XMMatrixTranspose(XMMatrixMultiply(Float4x4ToXMMatrix(Bindpose[3]), Float4x4ToXMMatrix(LerpJoint(LerpRootUpper, LerpRootLower, .75f))))));
		for (unsigned int i = 4; i < Frame1LowerJoints.size(); i++)
		{
			if (AnimationParents[i] == 1)// GetParent(i, animationLower->GetAnimationClip().parent_indicies) == 1)// animationLower.parent_indicies[i] == 1)
			{
				NewJoints.push_back(XMMatrixToFloat4x4(XMMatrixTranspose(XMMatrixMultiply(Float4x4ToXMMatrix(Bindpose[i]), Float4x4ToXMMatrix(LerpJoint(Frame1UpperJoints[i], Frame2UpperJoints[i], ratioUpper))))));
			}
			else
			{
				NewJoints.push_back(XMMatrixToFloat4x4(XMMatrixTranspose(XMMatrixMultiply(Float4x4ToXMMatrix(Bindpose[i]), Float4x4ToXMMatrix(LerpJoint(Frame1LowerJoints[i], Frame2LowerJoints[i], ratioUpper))))));
			}
		}
	return NewJoints;
}
