#include "Player.h"



Player::Player()
{
	objectModel = new Model();
	ObjectPhysics = new PhysicsComponent();
	health = 1000;
	PrevUpperAnimation = nullptr;
	canMoveForward = true;
	canMoveBackward = true;
	canMoveLeft = true;
	canMoveRight = true;
}


Player::~Player()
{
}

//int GetParent(int index, std::vector<int32_t> parents)
//{
//	if (parents[index] == -1)
//	{
//		return -1;
//	}
//	if (parents[index] == 0)
//	{
//		return index;
//	}
//	else
//	{
//		return GetParent(parents[index], parents);
//	}
//}
//
//
//float4x4 RecursiveJointCalc(unsigned int currentIndex, std::vector<float4x4>& joints, std::vector<int32_t>& parents)
//{
//	if (parents[currentIndex] == -1)
//	{
//		return joints[currentIndex];
//	}
//	else
//	{
//		return XMMatrixToFloat4x4(XMMatrixMultiply(Float4x4ToXMMatrix(joints[currentIndex]), Float4x4ToXMMatrix(RecursiveJointCalc(parents[currentIndex], joints, parents))));
//	}
//}
//
//std::vector<float4x4> Flatten(std::vector<float4x4> joints, std::vector<int32_t> parents)
//{
//	std::vector<float4x4> NewJoints;
//	for (unsigned int i = 0; i < joints.size(); i++)
//	{
//		float4x4 joint = RecursiveJointCalc(i, joints, parents);
//		NewJoints.push_back(joint);
//	}
//	return NewJoints;
//}


bool Player::Initialize(const char * filePath, ID3D11Device * device)
{
	playerBox.center.x = ObjectPhysics->GetPosition().x;
	playerBox.center.y = ObjectPhysics->GetPosition().y + 2.0f;
	playerBox.center.z = ObjectPhysics->GetPosition().z;
	forwardArrow.start = playerBox.center;
	forwardArrow.end = GetPhysicsComponent()->GetForward() * -1.0f;
	backwardArrow.start = playerBox.center;
	backwardArrow.end = GetPhysicsComponent()->GetForward();
	leftArrow.start = playerBox.center;
	playerBox.dimensions = { 1.0f, 1.0f, 1.0f };
	return objectModel->Initialize(filePath, device);
}

void Player::Render(ID3D11DeviceContext * context)
{
	objectModel->Render(context);
}

void Player::Shutdown()
{
	GameObject::Shutdown();
}

void Player::Update(float delta)
{
	ObjectPhysics->Update(delta);
	if (currentLowerAnimation != nullptr && currentUpperAnimation != nullptr)
	{
		if (PlayOnce)
		{
			if (currentUpperAnimation->GetFrameTime() + delta > currentUpperAnimation->GetAnimationClip().duration)
			{
				currentUpperAnimation->SetFrameTime(delta);
				currentUpperAnimation = objectAnimations[1];
				PlayOnce = false;
			}
		}
		currentLowerAnimation->SetFrameTime(delta);
		currentUpperAnimation->SetFrameTime(delta);
		AnimationJoints = SetSkeletonLines(currentUpperAnimation->GetAnimationClip(), currentLowerAnimation->GetAnimationClip(), currentUpperAnimation->GetFrameTime(), currentLowerAnimation->GetFrameTime());
	}
	if (timeLeftDamage > 0)
	{
		timeLeftDamage -= delta;
	}
	if (timeLeftDamage <= 0)
	{
		timeLeftDamage = 0;
	}
	playerBox.center.x = ObjectPhysics->GetPosition().x;
	playerBox.center.y = ObjectPhysics->GetPosition().y + 2.0f;
	playerBox.center.z = ObjectPhysics->GetPosition().z;
	forwardArrow.start = playerBox.center;
	forwardArrow.end = { forwardArrow.start.x + (GetPhysicsComponent()->GetForward().x * -1.0f), GetPhysicsComponent()->GetPosition().y + 2.0f,  forwardArrow.start.z + (GetPhysicsComponent()->GetForward().z * -1.0f)};
	backwardArrow.start = playerBox.center;
	backwardArrow.end = { backwardArrow.start.x + (GetPhysicsComponent()->GetForward().x), GetPhysicsComponent()->GetPosition().y + 2.0f,  backwardArrow.start.z + (GetPhysicsComponent()->GetForward().z) };
	leftArrow.start = playerBox.center;
	float3 leftForward = VectorMatrixMultiplication(CreateRotationMatrixY(-90.0f), GetPhysicsComponent()->GetForward());
	leftArrow.end = { leftArrow.start.x - (leftForward.x), GetPhysicsComponent()->GetPosition().y + 2.0f,  leftArrow.start.z - (leftForward.z)};

	rightArrow.start = playerBox.center;
	float3 rightForward = VectorMatrixMultiplication(CreateRotationMatrixY(90.0f), GetPhysicsComponent()->GetForward());
	rightArrow.end = { rightArrow.start.x - (rightForward.x), GetPhysicsComponent()->GetPosition().y + 2.0f,  rightArrow.start.z - (rightForward.z) };
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

std::vector<float4x4> Player::SetSkeletonLines(anim_clip animationUpper, anim_clip animationLower, float timeUpper, float timeLower)
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

void Player::SetAnimationUpper(int index)
{
	currentUpperAnimation = objectAnimations[index];
}

void Player::SetAnimationLower(int index)
{
	currentLowerAnimation = objectAnimations[index];
}

Segment Player::GetForwardArrow()
{
	return forwardArrow;
}

Segment Player::GetBackwardArrow()
{
	return backwardArrow;
}

Segment Player::GetLeftArrow()
{
	return leftArrow;
}

Segment Player::GetRightArrow()
{
	return rightArrow;
}

bool Player::MoveForward()
{
	return canMoveForward;
}

bool Player::MoveBackward()
{
	return canMoveBackward;
}

bool Player::MoveLeft()
{
	return canMoveLeft;
}

bool Player::MoveRight()
{
	return canMoveRight;
}

void Player::SetForward(bool f)
{
	canMoveForward = f;
}

void Player::SetBackward(bool f)
{
	canMoveBackward = f;
}

void Player::SetLeft(bool f)
{
	canMoveLeft = f;
}

void Player::SetRight(bool f)
{
	canMoveRight = f;
}

void Player::MeleeAttack(int index)
{
	PlayOnce = true;
	PrevUpperAnimation = currentUpperAnimation;
	currentUpperAnimation = objectAnimations[index];
}
