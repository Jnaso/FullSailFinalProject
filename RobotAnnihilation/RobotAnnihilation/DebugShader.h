#pragma once
#ifndef _DEBUGSHADER_H_
#define _DEBUGSHADER_H_

#pragma comment(lib, "D3DCompiler.lib")

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h> 
#include <fstream>
#include "debug_renderer.h"

using namespace std;
using namespace DirectX;

class DebugShader
{
private:
	struct ConstantBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	ID3D11VertexShader *myVertexShader;
	ID3D11PixelShader *myPixelShader;
	ID3D11InputLayout *myLayout;
	ID3D11Buffer *myConstantBuffer;

	bool CompileShaders(ID3D11Device* myDevice, WCHAR *vsFile, WCHAR* psFile);
	void DestroyShaders();
	void ProcessShaderErrors(ID3D10Blob *error);

	bool UpdateShaderBuffers(ID3D11DeviceContext *myContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection);
	void DrawShaders(ID3D11DeviceContext *myContext);

public:
	DebugShader();
	bool Initialize(ID3D11Device *myDevice);
	void Shutdown();
	bool Render(ID3D11DeviceContext *myContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection);
};

#endif // !_DEBUGSHADER_H_
