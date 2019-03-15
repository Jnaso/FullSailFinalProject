#pragma once
#ifndef _SHADERMANAGER_H_
#define _SHADERMANAGER_H_

#include "dxstuff.h"

#include "shaderstuff.h"
#include "StaticShader.h"
#include "MathDefines.h"

class ShaderManager
{
private:
	AnimatedShader *myAnimatedShader;
	StaticShader *myStaticShader;

public:
	ShaderManager();

	bool Initialize(ID3D11Device *);
	void Shutdown();

	bool RenderAnimatedShader(ID3D11DeviceContext *myContext, int indicies, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView *texture, ID3D11ShaderResourceView* normalTexture, XMFLOAT3 lightDir, XMFLOAT4 dirColor, std::vector<float4x4> binds);
	bool RenderStaticShader(ID3D11DeviceContext *myContext, int indicies, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView *texture, XMFLOAT3 lightDir, XMFLOAT4 dirColor);
};



#endif // !_SHADERMANAGER_H_