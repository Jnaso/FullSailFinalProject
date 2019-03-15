#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	myAnimatedShader = nullptr;
	myStaticShader = nullptr;
}

bool ShaderManager::Initialize(ID3D11Device* device)
{
	bool result;
	myAnimatedShader = new AnimatedShader();
	if (!myAnimatedShader)
	{
		return false;
	}

	result = myAnimatedShader->Initialize(device);
	if (!result)
	{
		return false;
	}

	myStaticShader = new StaticShader();
	if (!myStaticShader)
	{
		return false;
	}

	result = myStaticShader->Initialize(device);
	if (!result)
	{
		return false;
	}
}

void ShaderManager::Shutdown()
{
	if (myAnimatedShader)
	{
		myAnimatedShader->Shutdown();
		delete myAnimatedShader;
		myAnimatedShader = nullptr;
	}

	if (myStaticShader)
	{
		myStaticShader->Shutdown();
		delete myStaticShader;
		myStaticShader = nullptr;
	}
}

bool ShaderManager::RenderAnimatedShader(ID3D11DeviceContext *myContext, int indicies, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView *texture, ID3D11ShaderResourceView* normalTexture, XMFLOAT3 lightDir, XMFLOAT4 dirColor, vector<float4x4> binds)
{
	bool result;
	result = myAnimatedShader->Render(myContext, indicies, world, view, projection, texture, normalTexture, lightDir, dirColor, binds);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ShaderManager::RenderStaticShader(ID3D11DeviceContext *myContext, int indicies, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView *texture, XMFLOAT3 lightDir, XMFLOAT4 dirColor)
{
	bool result;

	result = myStaticShader->Render(myContext, indicies, world, view, projection, texture, lightDir, dirColor);
	if (!result)
	{
		return false;
	}

	return true;
}