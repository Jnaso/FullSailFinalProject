#pragma once

#include "MathDefines.h"
#include "WICTextureLoader.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <dxgi1_2.h>
#include <d3d11_2.h>
#include <DirectXMath.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <vector>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DXGI.lib")
class Animation
{
	float frameTime = 0;
	anim_clip ObjAnim;

	std::vector<float4x4> Joints;
public:
	Animation();
	Animation(const char* filePath, ID3D11Device* device);
	~Animation();
	void ReadAnimFile(const char * filePath, ID3D11Device * device);
	std::vector<float4x4> LerpJoints(std::vector<float4x4>frame1, std::vector<float4x4>frame2, float ratio);
	void SetJoints(float frametime);
	void Update(float update);
	std::vector<float4x4> GetJoints();
	void SetJoints(std::vector<float4x4> newJ);
	std::vector<int32_t> GetParents();

	float GetFrameTime();
	void SetFrameTime(float delta);

	anim_clip GetAnimationClip() { return ObjAnim; }

	std::vector<float4x4> Flatten(std::vector<float4x4> joints, std::vector<int32_t> parents);
	float4x4 RecursiveJointCalc(unsigned int currentIndex, std::vector<float4x4>& joints, std::vector<int32_t>& parents);
};

