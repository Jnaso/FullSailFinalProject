#include "Model.h"

Model::Model()
{
}


Model::~Model()
{
}

void Model::ReadMeshFile(const char * filePath, ID3D11Device* device)
{
	std::fstream file(filePath, std::ios_base::in | std::ios_base::binary);
	uint32_t Count = 0;
	file.read((char*)&Count, sizeof(uint32_t));
	ObjectVerts.resize(Count);
	float temp;
	for (uint32_t i = 0; i < Count; i++)
	{
		file.read((char*)&ObjectVerts[i].pos.x, sizeof(float));
		file.read((char*)&ObjectVerts[i].pos.y, sizeof(float));
		file.read((char*)&ObjectVerts[i].pos.z, sizeof(float));
		file.read((char*)&temp, sizeof(float));
		file.read((char*)&ObjectVerts[i].norm.x, sizeof(float));
		file.read((char*)&ObjectVerts[i].norm.y, sizeof(float));
		file.read((char*)&ObjectVerts[i].norm.z, sizeof(float));
		file.read((char*)&temp, sizeof(float));
		file.read((char*)&ObjectVerts[i].texCords.x, sizeof(float));
		file.read((char*)&ObjectVerts[i].texCords.y, sizeof(float));
		file.read((char*)&ObjectVerts[i].Tangent.x, sizeof(float));
		file.read((char*)&ObjectVerts[i].Tangent.y, sizeof(float));
		file.read((char*)&ObjectVerts[i].Tangent.z, sizeof(float));
		file.read((char*)&temp, sizeof(float));
		file.read((char*)&ObjectVerts[i].joints[0], sizeof(float));
		file.read((char*)&ObjectVerts[i].joints[1], sizeof(float));
		file.read((char*)&ObjectVerts[i].joints[2], sizeof(float));
		file.read((char*)&ObjectVerts[i].joints[3], sizeof(float));
		file.read((char*)&ObjectVerts[i].weights.x, sizeof(float));
		file.read((char*)&ObjectVerts[i].weights.y, sizeof(float));
		file.read((char*)&ObjectVerts[i].weights.z, sizeof(float));
		file.read((char*)&ObjectVerts[i].weights.w, sizeof(float));
	}

	file.read((char*)&Count, sizeof(uint32_t));
	ObjectIndices.resize(Count);
	file.read((char*)ObjectIndices.data(), sizeof(uint32_t) * Count);

	uint32_t pathsAmount;
	std::vector<file_path_t> paths;
	file.read((char*)&pathsAmount, sizeof(uint32_t));
	paths.resize(pathsAmount);
	file.read((char*)paths.data(), sizeof(file_path_t) * pathsAmount);


	uint32_t matsAmount;
	std::vector<material_t> mats;
	file.read((char*)&matsAmount, sizeof(uint32_t));
	mats.resize(matsAmount);
	file.read((char*)mats.data(), sizeof(material_t) * matsAmount);

	file.close();

	ID3D11Resource* res = {};
	for (uint32_t i = 0; i < pathsAmount; i++)
	{
		HRESULT hr;
		file_path_t newFilePath;
		newFilePath[0] = 'A';
		newFilePath[1] = 's';
		newFilePath[2] = 's';
		newFilePath[3] = 'e';
		newFilePath[4] = 't';
		newFilePath[5] = 's';
		newFilePath[6] = '/';
		for (size_t j = 7; j < 260; j++)
		{
			char temp = paths[i][j - 7];
			newFilePath[j] = temp;
		}
		wchar_t* dest = new wchar_t[newFilePath.size()];
		mbstowcs(dest, (const char*)&newFilePath, newFilePath.size());

		if (i == 0)
		{
			hr = CreateDDSTextureFromFile(device, dest, nullptr, &Diffuse);
		}
		if (i == 1)
		{
			hr = CreateDDSTextureFromFile(device, dest, nullptr, &Emissive);
		}
		if (i == 2)
		{
			hr = CreateDDSTextureFromFile(device, dest, nullptr, &Specular);
		}
		if (i == 3)
		{
			hr = CreateDDSTextureFromFile(device, dest, nullptr, &Normal);
		}
		delete dest;
	}

}

std::vector<Vertex> Model::GetObjectVerts()
{
	return ObjectVerts;
}

std::vector<uint32_t> Model::GetObjectIndices()
{
	return ObjectIndices;
}

ID3D11ShaderResourceView* Model::GetDiffuseTexture()
{
	return Diffuse;
}

ID3D11ShaderResourceView* Model::GetNormalTexture()
{
	return Normal;
}

bool Model::Initialize(const char* filePath, ID3D11Device* device)
{
	ReadMeshFile(filePath, device);
	HRESULT result;
	D3D11_BUFFER_DESC desc = {};
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(Vertex) * ObjectVerts.size();
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexData.pSysMem = ObjectVerts.data();
	result = device->CreateBuffer(&desc, &vertexData, &ObjectVBuffer);
	if (FAILED(result)) return false;
	desc.ByteWidth = sizeof(uint32_t) * ObjectIndices.size();
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexData.pSysMem = ObjectIndices.data();
	result = device->CreateBuffer(&desc, &indexData, &ObjectIndexBuffer);
	if (FAILED(result)) return false;
	return true;
}

void Model::Render(ID3D11DeviceContext* context)
{
	UINT strides[] = { sizeof(Vertex) };
	UINT offsets[] = { 0 };
	ID3D11Buffer	*vbuffer[] = { ObjectVBuffer };
	context->IASetVertexBuffers(0, 1, vbuffer, strides, offsets);
	context->IASetIndexBuffer(ObjectIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model::Shutdown()
{
	if (ObjectVBuffer) ObjectVBuffer->Release();
	if (ObjectIndexBuffer) ObjectIndexBuffer->Release();
	if (Diffuse) Diffuse->Release();
	if (Emissive) Emissive->Release();
	if (Specular) Specular->Release();
	if (Normal) Normal->Release();
}
