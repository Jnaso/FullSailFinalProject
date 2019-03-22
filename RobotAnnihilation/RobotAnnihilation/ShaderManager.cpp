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

bool ShaderManager::RenderAnimatedShader(ID3D11DeviceContext *myContext, int indicies, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView *texture, ID3D11ShaderResourceView* normalTexture, XMFLOAT3 lightDir, XMFLOAT4 dirColor, vector<float4x4> binds, XMFLOAT4 myPos[], XMFLOAT4 myCol[], XMFLOAT4 spotCol, XMFLOAT4 spotDir, XMFLOAT4 spotPos, XMFLOAT4 spotEx, XMFLOAT4 cam, XMFLOAT4 specCol, XMFLOAT4 specEx)
{
	bool result;
	result = myAnimatedShader->Render(myContext, indicies, world, view, projection, texture, normalTexture, lightDir, dirColor, binds, myPos, myCol, spotCol, spotDir, spotPos, spotEx, cam, specCol, specEx);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ShaderManager::RenderStaticShader(ID3D11DeviceContext *myContext, int indicies, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView *texture, XMFLOAT3 lightDir, XMFLOAT4 dirColor, XMFLOAT4 myPos[], XMFLOAT4 myCol[], XMFLOAT4 spotCol, XMFLOAT4 spotDir, XMFLOAT4 spotPos, XMFLOAT4 spotEx, XMFLOAT4 cam, XMFLOAT4 specCol, XMFLOAT4 specEx)
{
	bool result;

	result = myStaticShader->Render(myContext, indicies, world, view, projection, texture, lightDir, dirColor, myPos, myCol, spotCol, spotDir, spotPos, spotEx, cam, specCol, specEx);
	if (!result)
	{
		return false;
	}

	return true;
}