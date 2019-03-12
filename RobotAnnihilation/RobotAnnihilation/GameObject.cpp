#include "GameObject.h"



GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

void GameObject::ReadBinFile(const char * filePath, ID3D11Device* device)
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
		file.read((char*)&ObjectVerts[i].weights.z, sizeof(float));
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

	uint32_t framesAmount;
	file.read((char*)&framesAmount, sizeof(uint32_t));
	//Read animation data here
	ObjectAnimation.frames.resize(framesAmount);
	for (uint32_t i = 0; i < framesAmount; i++)
	{
		file.read((char*)&ObjectAnimation.frames[i].time, sizeof(double));
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
			ObjectAnimation.frames[i].joints.push_back(data);
		}
	}
	double time = 0;
	file.read((char*)&time, sizeof(double));
	ObjectAnimation.bindPose.time = 0;
	ObjectAnimation.bindPose.joints.resize(ObjectAnimation.frames[0].joints.size());
	for (uint32_t i = 0; i < ObjectAnimation.frames[0].joints.size(); i++)
	{
		file.read((char*)&ObjectAnimation.bindPose.joints[i][0][0], sizeof(float));
		file.read((char*)&ObjectAnimation.bindPose.joints[i][0][1], sizeof(float));
		file.read((char*)&ObjectAnimation.bindPose.joints[i][0][2], sizeof(float));
		file.read((char*)&ObjectAnimation.bindPose.joints[i][0][3], sizeof(float));
		file.read((char*)&ObjectAnimation.bindPose.joints[i][1][0], sizeof(float));
		file.read((char*)&ObjectAnimation.bindPose.joints[i][1][1], sizeof(float));
		file.read((char*)&ObjectAnimation.bindPose.joints[i][1][2], sizeof(float));
		file.read((char*)&ObjectAnimation.bindPose.joints[i][1][3], sizeof(float));
		file.read((char*)&ObjectAnimation.bindPose.joints[i][2][0], sizeof(float));
		file.read((char*)&ObjectAnimation.bindPose.joints[i][2][1], sizeof(float));
		file.read((char*)&ObjectAnimation.bindPose.joints[i][2][2], sizeof(float));
		file.read((char*)&ObjectAnimation.bindPose.joints[i][2][3], sizeof(float));
		file.read((char*)&ObjectAnimation.bindPose.joints[i][3][0], sizeof(float));
		file.read((char*)&ObjectAnimation.bindPose.joints[i][3][1], sizeof(float));
		file.read((char*)&ObjectAnimation.bindPose.joints[i][3][2], sizeof(float));
		file.read((char*)&ObjectAnimation.bindPose.joints[i][3][3], sizeof(float));
	}
	for (unsigned int i = 0; i < ObjectAnimation.bindPose.joints.size(); i++)
	{
		ObjectAnimation.bindPose.joints[i] = XMMatrixToFloat4x4(XMMatrixInverse(nullptr, Float4x4ToXMMatrix(ObjectAnimation.bindPose.joints[i])));
	}
	for (unsigned int i = 0; i < ObjectAnimation.frames[0].joints.size(); i++)
	{
		int index;
		file.read((char*)&index, sizeof(int32_t));
		ObjectAnimation.parent_indicies.push_back(index);
	}
	file.read((char*)&ObjectAnimation.duration, sizeof(double));
	file.close();
	ID3D11Resource* res = {};
	for (uint32_t i = 0; i < pathsAmount; i++)
	{
		wchar_t* dest = new wchar_t[paths[i].size()];
		mbstowcs(dest, (const char*)&paths[i], paths[i].size());
		//HRESULT hr = CreateWICTextureFromFile(device, dest, &res, &srv);

		if (i == 0)
		{
			if (CreateWICTextureFromFile(device, dest, &res, &Diffuse) == S_OK)
			{
				res->Release();
				continue;
			}
		}
		if (i == 1)
		{
			if (CreateWICTextureFromFile(device, dest, &res, &Emissive) == S_OK)
			{
				res->Release();
				continue;
			}
		}
		if (i == 2)
		{
			if (CreateWICTextureFromFile(device, dest, &res, &Normal) == S_OK)
			{
				res->Release();
				continue;
			}
		}
		if (i == 3)
		{
			if (CreateWICTextureFromFile(device, dest, &res, &Specular) == S_OK)
			{
				res->Release();
				continue;
			}
		}
		delete dest;
	}
	if (Diffuse)
	{
		Textures.push_back(Diffuse);
	}
	if (Emissive)
	{
		Textures.push_back(Emissive);
	}
	if (Normal)
	{
		Textures.push_back(Normal);
	}
	if (Specular)
	{
		Textures.push_back(Specular);
	}

}

std::vector<Vertex> GameObject::GetObjectVerts()
{
	return ObjectVerts;
}

std::vector<uint32_t> GameObject::GetObjectIndices()
{
	return ObjectIndices;
}

std::vector<ID3D11ShaderResourceView*> GameObject::GetObjectTextures()
{
	return Textures;
}
