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

using namespace DirectX;

class GameObject
{
	//Conversions converter;

	std::vector<Vertex> ObjectVerts;
	std::vector<uint32_t> ObjectIndices;

	anim_clip ObjectAnimation;

	// Storage for reading in function
	ID3D11ShaderResourceView* Diffuse;
	ID3D11ShaderResourceView* Emissive;
	ID3D11ShaderResourceView* Specular;
	ID3D11ShaderResourceView* Normal;

	ID3D11Buffer* ObjectVBuffer = nullptr;
	ID3D11Buffer* ObjectIndexBuffer = nullptr;

	float frametime;

public:
	GameObject();
	GameObject(const char* filePath, ID3D11Device* device);
	~GameObject();

	bool Initialize(ID3D11Device* device);
	void Render(ID3D11DeviceContext* context);
	void Shutdown();

	void ReadBinFile(const char* filePath, ID3D11Device* device);

	void Update(float delta);

	std::vector<Vertex> GetObjectVerts();
	std::vector<uint32_t> GetObjectIndices();
	ID3D11ShaderResourceView* GetDiffuseTexture();
	float4x4* SetJoints();
};

