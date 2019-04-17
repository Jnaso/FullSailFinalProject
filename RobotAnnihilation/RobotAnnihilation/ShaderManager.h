#pragma once
#ifndef _SHADERMANAGER_H_
#define _SHADERMANAGER_H_

#include "dxstuff.h"

#include "shaderstuff.h"
#include "StaticShader.h"
#include "MathDefines.h"
#include "SkyboxShader.h"
#include "DDSTextureLoader.h"

using namespace DirectX;

class ShaderManager
{
private:
	AnimatedShader *myAnimatedShader;
	StaticShader *myStaticShader;
	SkyboxShader *mySkyboxShader;
	ID3D11ShaderResourceView* myRealSky;

public:
	ShaderManager();

	bool Initialize(ID3D11Device *);
	void Shutdown();

	bool RenderAnimatedShader(ID3D11DeviceContext *myContext, int indicies, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView *texture, ID3D11ShaderResourceView* normalTexture, XMFLOAT3 lightDir, XMFLOAT4 dirColor, std::vector<float4x4> binds, XMFLOAT4[], XMFLOAT4[], XMFLOAT4, XMFLOAT4, XMFLOAT4, XMFLOAT4, XMFLOAT4 cam, XMFLOAT4 specCol, XMFLOAT4 specEx, bool red = false, bool bomb = false);
	bool RenderStaticShader(ID3D11DeviceContext *myContext, int indicies, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView *texture, XMFLOAT3 lightDir, XMFLOAT4 dirColor, XMFLOAT4[], XMFLOAT4[], XMFLOAT4, XMFLOAT4, XMFLOAT4, XMFLOAT4, XMFLOAT4 cam, XMFLOAT4 specCol, XMFLOAT4 specEx, bool red = false);
	bool RenderSkyboxShader(ID3D11DeviceContext *myContext, int indicies, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView *texture);
	ID3D11ShaderResourceView* GetSkyBox();
};



#endif // !_SHADERMANAGER_H_