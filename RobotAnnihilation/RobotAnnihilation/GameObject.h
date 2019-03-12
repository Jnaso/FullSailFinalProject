#pragma once
#include "header.h"

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

