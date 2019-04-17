#include "Animation.h"

Animation::Animation()
{
}

float4x4 Animation::RecursiveJointCalc(unsigned int currentIndex, std::vector<float4x4>& joints, std::vector<int32_t>& parents)
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

std::vector<float4x4> Animation::Flatten(std::vector<float4x4> joints, std::vector<int32_t> parents)
{
	std::vector<float4x4> NewJoints;
	for (unsigned int i = 0; i < joints.size(); i++)
	{
		float4x4 joint = RecursiveJointCalc(i, joints, parents);
		NewJoints.push_back(joint);
	}
	return NewJoints;
}

Animation::Animation(const char* filePath, ID3D11Device* device)
{
	ReadAnimFile(filePath, device);
	for (size_t i = 0; i < ObjAnim.frames.size(); i++)
	{
		std::vector<float4x4> flattenedjoints = Flatten(ObjAnim.frames[i].joints, ObjAnim.parent_indicies);
		ObjAnim.frames[i].joints = flattenedjoints;
	}
	ObjAnim.bindPose.joints = Flatten(ObjAnim.bindPose.joints, ObjAnim.parent_indicies);
}


Animation::~Animation()
{
}


void Animation::ReadAnimFile(const char* filePath, ID3D11Device* device)
{
	std::fstream file(filePath, std::ios_base::in | std::ios_base::binary);
	if (file.is_open())
	{
		std::fstream file(filePath, std::ios_base::in | std::ios_base::binary);
		//Read animation data here
		uint32_t framesAmount;
		file.read((char*)&framesAmount, sizeof(uint32_t));
		ObjAnim.frames.resize(framesAmount);
		for (uint32_t i = 0; i < framesAmount; i++)
		{
			file.read((char*)&ObjAnim.frames[i].time, sizeof(double));
			uint32_t framesAmount;
			file.read((char*)&framesAmount, sizeof(uint32_t));
			for (uint32_t j = 0; j < framesAmount; j++)
			{
				float4x4 data;
				file.read((char*)&data[0].x, sizeof(float));
				file.read((char*)&data[0].y, sizeof(float));
				file.read((char*)&data[0].z, sizeof(float));
				file.read((char*)&data[0].w, sizeof(float));
				file.read((char*)&data[1].x, sizeof(float));
				file.read((char*)&data[1].y, sizeof(float));
				file.read((char*)&data[1].z, sizeof(float));
				file.read((char*)&data[1].w, sizeof(float));
				file.read((char*)&data[2].x, sizeof(float));
				file.read((char*)&data[2].y, sizeof(float));
				file.read((char*)&data[2].z, sizeof(float));
				file.read((char*)&data[2].w, sizeof(float));
				file.read((char*)&data[3].x, sizeof(float));
				file.read((char*)&data[3].y, sizeof(float));
				file.read((char*)&data[3].z, sizeof(float));
				file.read((char*)&data[3].w, sizeof(float));
				ObjAnim.frames[i].joints.push_back(data);
			}
		}
		double time = 0;
		file.read((char*)&time, sizeof(double));
		ObjAnim.bindPose.time = 0;
		ObjAnim.bindPose.joints.resize(ObjAnim.frames[0].joints.size());
		for (uint32_t i = 0; i < ObjAnim.frames[0].joints.size(); i++)
		{
			file.read((char*)&ObjAnim.bindPose.joints[i][0][0], sizeof(float));
			file.read((char*)&ObjAnim.bindPose.joints[i][0][1], sizeof(float));
			file.read((char*)&ObjAnim.bindPose.joints[i][0][2], sizeof(float));
			file.read((char*)&ObjAnim.bindPose.joints[i][0][3], sizeof(float));
			file.read((char*)&ObjAnim.bindPose.joints[i][1][0], sizeof(float));
			file.read((char*)&ObjAnim.bindPose.joints[i][1][1], sizeof(float));
			file.read((char*)&ObjAnim.bindPose.joints[i][1][2], sizeof(float));
			file.read((char*)&ObjAnim.bindPose.joints[i][1][3], sizeof(float));
			file.read((char*)&ObjAnim.bindPose.joints[i][2][0], sizeof(float));
			file.read((char*)&ObjAnim.bindPose.joints[i][2][1], sizeof(float));
			file.read((char*)&ObjAnim.bindPose.joints[i][2][2], sizeof(float));
			file.read((char*)&ObjAnim.bindPose.joints[i][2][3], sizeof(float));
			file.read((char*)&ObjAnim.bindPose.joints[i][3][0], sizeof(float));
			file.read((char*)&ObjAnim.bindPose.joints[i][3][1], sizeof(float));
			file.read((char*)&ObjAnim.bindPose.joints[i][3][2], sizeof(float));
			file.read((char*)&ObjAnim.bindPose.joints[i][3][3], sizeof(float));
		}
		for (unsigned int i = 0; i < ObjAnim.bindPose.joints.size(); i++)
		{
			ObjAnim.bindPose.joints[i] = XMMatrixToFloat4x4(XMMatrixInverse(nullptr, Float4x4ToXMMatrix(ObjAnim.bindPose.joints[i])));
		}
		for (unsigned int i = 0; i < ObjAnim.frames[0].joints.size(); i++)
		{
			int index;
			file.read((char*)&index, sizeof(int32_t));
			ObjAnim.parent_indicies.push_back(index);
		}
		file.read((char*)&ObjAnim.duration, sizeof(double));
		file.close();
	}
}

void Animation::Update(float delta)
{
	if (this)
	{
		SetFrameTime(delta);
		SetJoints(frameTime);
	}
}

std::vector<float4x4> Animation::LerpJoints(std::vector<float4x4>frame1, std::vector<float4x4>frame2, float ratio)
{
	// Calculate lerp between keyframes using ratio calculated above
	std::vector<float4x4> newJoints;
	for (uint32_t i = 0; i < frame1.size(); i++)
	{
		XMVECTOR result = XMQuaternionSlerp(XMQuaternionRotationMatrix(Float4x4ToXMMatrix(frame1[i])), XMQuaternionRotationMatrix(Float4x4ToXMMatrix(frame2[i])), ratio);
		XMMATRIX back = XMMatrixRotationQuaternion(result);
		//((p2 - p1) * ratio + p1);
		XMVECTOR pos;
		pos.m128_f32[0] = ((frame2[i][3].x - frame1[i][3].x) * ratio + frame1[i][3].x);
		pos.m128_f32[1] = ((frame2[i][3].y - frame1[i][3].y) * ratio + frame1[i][3].y);
		pos.m128_f32[2] = ((frame2[i][3].z - frame1[i][3].z) * ratio + frame1[i][3].z);
		pos.m128_f32[3] = ((frame2[i][3].w - frame1[i][3].w) * ratio + frame1[i][3].w);
		back.r[3].m128_f32[0] = pos.m128_f32[0];
		back.r[3].m128_f32[1] = pos.m128_f32[1];
		back.r[3].m128_f32[2] = pos.m128_f32[2];
		back.r[3].m128_f32[3] = pos.m128_f32[3];
		newJoints.push_back(XMMatrixToFloat4x4(back));
	}
	return newJoints;
}

void Animation::SetJoints(float frametime)
{
	std::vector<float4x4> newJoints;
	uint32_t frame1 = 0;
	uint32_t frame2 = 0;
	for (size_t i = 0; i < ObjAnim.frames.size(); i++)
	{
		// Checks if time is before the first keyframe
		if (i == 0 && frametime < ObjAnim.frames[i].time)
		{
			frame1 = ObjAnim.frames.size() - 1;
			frame2 = i;
			break;
		}
		if (frametime > ObjAnim.frames[i].time && i == ObjAnim.frames.size() - 1)
		{
			frame1 = i;
			frame2 = 0;
			break;
		}
		// Checks if time falls between current and next keyframe
		if (frametime > ObjAnim.frames[i].time && frametime < ObjAnim.frames[i + 1].time)
		{
			frame1 = i;
			frame2 = i + 1;
			break;
		}
	}
	float ratio = 0;
	if (frame2 == 0)
		ratio = (frametime - (ObjAnim.frames[frame1].time - ObjAnim.duration)) / (ObjAnim.frames[frame2].time - (ObjAnim.frames[frame1].time - ObjAnim.duration));
	else
		ratio = (frametime - ObjAnim.frames[frame1].time) / (ObjAnim.frames[frame2].time - ObjAnim.frames[frame1].time);
	newJoints = LerpJoints(ObjAnim.frames[frame1].joints, ObjAnim.frames[frame2].joints, ratio);
	Joints.resize(newJoints.size());
	for (size_t i = 0; i < ObjAnim.frames[0].joints.size(); i++)
	{
		Joints[i] = XMMatrixToFloat4x4(XMMatrixTranspose(XMMatrixMultiply(Float4x4ToXMMatrix(ObjAnim.bindPose.joints[i]), Float4x4ToXMMatrix(newJoints[i]))));
	}
}

std::vector<float4x4> Animation::GetJoints()
{
	return Joints;
}

void Animation::SetJoints(std::vector<float4x4> newJ)
{
	Joints = newJ;
}

std::vector<int32_t> Animation::GetParents()
{
	return ObjAnim.parent_indicies;
}

float Animation::GetFrameTime()
{
	return frameTime;
}

void Animation::SetFrameTime(float delta)
{
	frameTime += delta;
	while (frameTime > ObjAnim.duration)
	{
		frameTime -= ObjAnim.duration;
	}
}
