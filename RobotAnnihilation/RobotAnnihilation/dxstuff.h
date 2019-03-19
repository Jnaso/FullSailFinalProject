#pragma once
#ifndef _DXSTUFF_H_
#define _DXSTUFF_H_

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#include <windows.h>
#include <memory>
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "SimpleMath.h"
#include "DDSTextureLoader.h"
#include <vector>

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

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> ArialFont;
	std::unique_ptr<DirectX::SpriteFont> ComicSansFont;

	struct Image
	{
		ID3D11ShaderResourceView* shaderRes = nullptr;
		DirectX::SimpleMath::Vector2 pos;
		Image(const char* dir, ID3D11Device* device, DirectX::SimpleMath::Vector2 newPos)
		{
			wchar_t* temp = new wchar_t[301];
			mbstowcs(temp, dir, 301);
			pos = newPos;
			HRESULT hr = DirectX::CreateDDSTextureFromFile(device, temp, nullptr, &shaderRes);
			delete temp;
		}
	};
	
	std::vector<Image> ImagesToRender;

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

	void CreateImage(const char* dir, DirectX::SimpleMath::Vector2 pos);
	void CreateImage(char* dir, DirectX::SimpleMath::Vector2 pos);
	void CreateText(std::unique_ptr<DirectX::SpriteFont>& font, const char* text, DirectX::SimpleMath::Vector2 pos);
	void CreateText(std::unique_ptr<DirectX::SpriteFont>& font, char* text, DirectX::SimpleMath::Vector2 pos);

	void SetViewMatrix(XMMATRIX);
	void SetWorldMatrix(XMMATRIX);
	XMMATRIX GetViewMatrix();
	XMMATRIX GetWorldMatrix();

};

#endif // !_DXSTUFF_H_