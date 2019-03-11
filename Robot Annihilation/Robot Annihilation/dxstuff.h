#pragma once
#ifndef _DXSTUFF_H_
#define _DXSTUFF_H_

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#include <windows.h>
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class DX
{
private:
	//DirectX initializer variables 
	bool vsync;
	int videoCard;
	char videoCardDesc[128];
	IDXGISwapChain *mySwapChain;
	ID3D11Device *myDevice;
	ID3D11DeviceContext *myDeviceContext;
	ID3D11RenderTargetView *myRenderTargetView;
	ID3D11Texture2D *myDepthStencilBuffer;
	ID3D11DepthStencilState *myDepthStencilState;
	ID3D11DepthStencilView *myDepthStencilView;
	ID3D11RasterizerState *myRasterState;
	XMMATRIX myProjection;
	XMMATRIX myWorld;
	XMMATRIX myView;

public:
	DX();
	DX(const DX &other);
	~DX();

	bool Initialize(int width, int height, bool myVsync, HWND window, bool full, float farPlane, float nearPlane);
	void Shutdown();

	void ClearScreen(float red, float green, float blue, float alpha);
	void PresentScreen();

	ID3D11Device *GetDevice();
	ID3D11DeviceContext *GetDeviceContext();

	void PassProjectionMatrix(XMMATRIX &proj);
	void PassWorldMatrix(XMMATRIX &world);

	XMMATRIX GetView();
	void SetView(XMMATRIX newView);

};

#endif // !_DXSTUFF_H_
