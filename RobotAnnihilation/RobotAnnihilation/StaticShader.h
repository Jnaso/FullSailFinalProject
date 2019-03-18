#pragma once
#ifndef _STATICSHADER_H_
#define _STATICSHADER_H_

#pragma comment(lib, "D3DCompiler.lib")

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h> 
#include <fstream>
#include "MathDefines.h"

using namespace std;
using namespace DirectX;

class StaticShader
{
private:
	struct ConstantBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct ConstantLightBuffer
	{
		XMFLOAT4 directionColor;
		XMFLOAT3 lightDirection;
		float padding;
		XMFLOAT4 positions[NUM_LIGHTS];
		XMFLOAT4 colors[NUM_LIGHTS];
		XMFLOAT4 spotLightColor;
		XMFLOAT4 spotLightDirection;
		XMFLOAT4 spotLightPosition;
		XMFLOAT4 spotLightExtra;
	};

	ID3D11VertexShader *myVertexShader;
	ID3D11PixelShader *myPixelShader;
	ID3D11InputLayout *myLayout;
	ID3D11Buffer *myConstantBuffer, *myConstantLightBuffer;
	ID3D11SamplerState *mySampler;

	bool CompileShaders(ID3D11Device* myDevice, WCHAR *vsFile, WCHAR* psFile);
	void DestroyShaders();
	void ProcessShaderErrors(ID3D10Blob *error);

	bool UpdateShaderBuffers(ID3D11DeviceContext *myContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView *texture, XMFLOAT3 lightDirection, XMFLOAT4 lightColor, XMFLOAT4[], XMFLOAT4[], XMFLOAT4, XMFLOAT4, XMFLOAT4, XMFLOAT4);
	void DrawShaders(ID3D11DeviceContext *myContext, int indicies);

public:
	StaticShader();

	bool Initialize(ID3D11Device *myDevice);
	void Shutdown();
	bool Render(ID3D11DeviceContext *myContext, int indicies, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView *texture, XMFLOAT3 lightDirection, XMFLOAT4 lightColor, XMFLOAT4[], XMFLOAT4[], XMFLOAT4, XMFLOAT4, XMFLOAT4, XMFLOAT4);
};

#endif // !_STATICSHADER_H_
