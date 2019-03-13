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
public:
	Animation();
	Animation(const char* filePath, ID3D11Device* device);
	~Animation();
	void ReadAnimFile(const char * filePath, ID3D11Device * device);
	float4x4* SetJoints(float frametime);
	void Update(float update);
};

