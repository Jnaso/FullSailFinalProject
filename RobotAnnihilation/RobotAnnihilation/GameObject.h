#pragma once
#include "MathDefines.h"
#include "WICTextureLoader.h"
#include <fstream>
#include <vector>
#include <dxgi1_2.h>
#include <d3d11_2.h>
#include <DirectXMath.h>

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

	// What will be passed in to shader
	std::vector<ID3D11ShaderResourceView*> Textures;
public:
	GameObject();
	~GameObject();

	void ReadBinFile(const char* filePath, ID3D11Device* device);

	std::vector<Vertex> GetObjectVerts();
	std::vector<uint32_t> GetObjectIndices();
	std::vector<ID3D11ShaderResourceView*> GetObjectTextures();
};

