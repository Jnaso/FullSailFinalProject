#pragma once
#include "MathDefines.h"
#include "WICTextureLoader.h"
#include "Animation.h"
#include "PhysicsComponent.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <dxgi1_2.h>
#include <d3d11_2.h>
#include <DirectXMath.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DXGI.lib")

using namespace DirectX;

class GameObject
{
protected:
	PhysicsComponent* ObjectPhysics;

	std::vector<Vertex> ObjectVerts;
	std::vector<uint32_t> ObjectIndices;

	ID3D11Buffer* ObjectVBuffer = nullptr;
	ID3D11Buffer* ObjectIndexBuffer = nullptr;

	// Storage for reading in function
	ID3D11ShaderResourceView* Diffuse;
	ID3D11ShaderResourceView* Emissive;
	ID3D11ShaderResourceView* Specular;
	ID3D11ShaderResourceView* Normal;

	void ReadMeshFile(const char* filePath, ID3D11Device* device);

public:
	GameObject();
	GameObject(const char* filePath, ID3D11Device* device);
	~GameObject();

	virtual bool Initialize(ID3D11Device* device);
	virtual void Render(ID3D11DeviceContext* context);
	virtual void Shutdown();
	
	virtual void Update(float delta);

	Animation* AddAninimation(const char* filePath, ID3D11Device* device);

	std::vector<Vertex> GetObjectVerts();
	std::vector<uint32_t> GetObjectIndices();
	ID3D11ShaderResourceView* GetDiffuseTexture();
	ID3D11ShaderResourceView* GetNormalTexture();

	PhysicsComponent* GetPhysicsComponent();
	void SetPhysicsComponent(PhysicsComponent* newPhysics);
};

