#pragma once
#ifndef _DXSTUFF_H_
#define _DXSTUFF_H_

#include "GeneralIncludes.h"

#define IMAGEVEC myDX->ImagesToRender[i]

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
	ID3D11RasterizerState *mySkyboxRasterState;
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
	void PassViewdMatrix(XMMATRIX &view);

	void SetViewMatrix(XMMATRIX);
	void SetWorldMatrix(XMMATRIX);
	XMMATRIX GetViewMatrix();
	XMMATRIX GetWorldMatrix();
	void SetRegularRaster();
	void SetSkyboxRaster();

};

#endif // !_DXSTUFF_H_