#pragma once
#include "MathDefines.h"
#include "DDSTextureLoader.h"
#include <fstream>
class Model
{
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
	Model();
	~Model();

	std::vector<Vertex> GetObjectVerts();
	std::vector<uint32_t> GetObjectIndices();
	ID3D11ShaderResourceView* GetDiffuseTexture();
	ID3D11ShaderResourceView* GetNormalTexture();

	bool Initialize(const char* filePath, ID3D11Device* device);

	void Render(ID3D11DeviceContext* context);

	void Shutdown();
};

