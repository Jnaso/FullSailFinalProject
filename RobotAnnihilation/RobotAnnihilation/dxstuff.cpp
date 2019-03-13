#include "dxstuff.h"

//Default contructor 
DX::DX()
{
	//Initialize every thing to null
	mySwapChain = nullptr;
	myDevice = nullptr;
	myDeviceContext = nullptr;
	myRenderTargetView = nullptr;
	myDepthStencilBuffer = nullptr;
	myDepthStencilState = nullptr;
	myDepthStencilView = nullptr;
	myRasterState = nullptr;
}

//Copy constructor 
DX::DX(const DX &other)
{

}

//Destructor
DX::~DX()
{

}

//Initialize all directX objects for drawing the screen 
bool DX::Initialize(int windowWidth, int windowHeight, bool myVsync, HWND window, bool full, float farPlane, float nearPlane)
{
	HRESULT result;
	DXGI_SWAP_CHAIN_DESC mySwapChainDesc;
	ID3D11Texture2D *backBuffer;
	D3D11_TEXTURE2D_DESC myDepthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC myDepthStencilDescription;
	D3D11_DEPTH_STENCIL_VIEW_DESC myDepthStencilViewDesc;
	D3D11_RASTERIZER_DESC myRasterDesc;
	D3D11_VIEWPORT myViewport;
	float fieldOfView, aspectRatio;
	IDXGIFactory *myFactory;
	IDXGIAdapter *myAdapter;
	IDXGIOutput *myOutput;
	unsigned int numModes, numerator, denminator;
	DXGI_MODE_DESC *displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	size_t stringLength;

	vsync = myVsync;

	//Creates the DirectX graphics interface factory
	//__uuidof = returns the GUID of a data type
	//GUID = Unique identifier for a data type 
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&myFactory);
	if (FAILED(result))
	{
		return false;
	}

	//Use the factory to initialize the adapter  
	result = myFactory->EnumAdapters(0, &myAdapter);
	if (FAILED(result))
	{
		return false;
	}

	//Use the adapter to create the output 
	result = myAdapter->EnumOutputs(0, &myOutput);
	if (FAILED(result))
	{
		return false;
	}

	//Get the number of modes that fit the adapter output display 
	result = myOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	//Resize our mode array to the number of modes 
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	//Fill in the array with the display modes 
	result = myOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	//Find the display mode that matches our screen's dimensions and store the numerator and denominator
	for (unsigned int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)windowWidth)
		{
			if (displayModeList[i].Height == (unsigned int)windowHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denminator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	//Get the video card description 
	result = myAdapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	//Store the video card memory 
	videoCard = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	//Error check to make sure the video card description has good data
	error = wcstombs_s(&stringLength, videoCardDesc, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	//All video card info is retrieved, clean up all objects we used
	delete[] displayModeList;
	displayModeList = nullptr;
	myOutput->Release();
	myOutput = nullptr;
	myAdapter->Release();
	myAdapter = nullptr;
	myFactory->Release();
	myFactory = nullptr;

	//Zero memory for the swap chain description
	ZeroMemory(&mySwapChainDesc, sizeof(mySwapChainDesc));

	//Amount of back buffers
	mySwapChainDesc.BufferCount = 1;
	//Screen dimensions for the buffer 
	mySwapChainDesc.BufferDesc.Width = windowWidth;
	mySwapChainDesc.BufferDesc.Height = windowHeight;
	//Buffer display format 
	mySwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	if (vsync)
	{
		mySwapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		mySwapChainDesc.BufferDesc.RefreshRate.Denominator = denminator;
	}
	else
	{
		mySwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		mySwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	mySwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	mySwapChainDesc.OutputWindow = window;

	//Used for multisampling 
	//Count = number of multisamples per pixel
	mySwapChainDesc.SampleDesc.Count = 1;
	//Quality = The image quality level 
	mySwapChainDesc.SampleDesc.Quality = 0;
	//Change windowed mode based on whether full screen is enabled or not
	if (full)
	{
		mySwapChainDesc.Windowed = false;
	}
	else
	{
		mySwapChainDesc.Windowed = true;
	}

	//Determining how the raster creates an image  
	mySwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	//How an image will be stretched to match the resolution
	mySwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	//Discards the buffer after it has been presented 
	mySwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	mySwapChainDesc.Flags = 0;

	//Feature level must be a reference to be passed into the create function
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	//Create the Swap Chain, Device, and Device Context
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &mySwapChainDesc, &mySwapChain, &myDevice, NULL, &myDeviceContext);
	if (FAILED(result))
	{
		return false;
	}

	//Get the back buffer data 
	result = mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Create the render target view using the back buffer data 
	result = myDevice->CreateRenderTargetView(backBuffer, NULL, &myRenderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	//No need for the back buffer any more
	backBuffer->Release();
	backBuffer = nullptr;

	//Zero memory for the depth buffer decription 
	ZeroMemory(&myDepthBufferDesc, sizeof(myDepthBufferDesc));
	//Set the window size
	myDepthBufferDesc.Width = windowWidth;
	myDepthBufferDesc.Height = windowHeight;
	//For mipmapping, the maximum number of mipmap levels 
	myDepthBufferDesc.MipLevels = 1;
	//Number of textures 
	myDepthBufferDesc.ArraySize = 1;
	//The texture format 
	myDepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//Used for multisampling 
	//Count = number of multisamples per pixel
	myDepthBufferDesc.SampleDesc.Count = 1;
	//Quality = The image quality level 
	myDepthBufferDesc.SampleDesc.Quality = 0;
	//How the texture is read and written
	myDepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	myDepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	myDepthBufferDesc.CPUAccessFlags = 0;
	myDepthBufferDesc.MiscFlags = 0;

	//Create the depth stencil with the depth buffer 
	result = myDevice->CreateTexture2D(&myDepthBufferDesc, NULL, &myDepthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Zero memory depth stencil description
	ZeroMemory(&myDepthStencilDescription, sizeof(myDepthStencilDescription));
	//Enable depth testing 
	myDepthStencilDescription.DepthEnable = true;
	//Portion of the depth stencil that can be modified with depth 
	myDepthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//Compares depth data with existing depth data 
	myDepthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS;
	//Enable stencil testing 
	myDepthStencilDescription.StencilEnable = true;
	//Portion of the depth stencil that reads stencil data 
	myDepthStencilDescription.StencilReadMask = 0xFF;
	//Portion of the depth stencil that writes stencil data 
	myDepthStencilDescription.StencilWriteMask = 0xFF;
	//Front face culling info 
	myDepthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	myDepthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	myDepthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	myDepthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	//Back face culling info 
	myDepthStencilDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	myDepthStencilDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	myDepthStencilDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	myDepthStencilDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//Create the depth stencil state
	result = myDevice->CreateDepthStencilState(&myDepthStencilDescription, &myDepthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	//Set the depth stencil state so it takes effect
	myDeviceContext->OMSetDepthStencilState(myDepthStencilState, 1);

	//Zero memory the depth stencil view description
	ZeroMemory(&myDepthStencilViewDesc, sizeof(myDepthStencilViewDesc));
	//Resource format 
	myDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//Type of resource 
	myDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//1D texture resource 
	myDepthStencilViewDesc.Texture2D.MipSlice = 0;

	//Create the depth stencil view 
	result = myDevice->CreateDepthStencilView(myDepthStencilBuffer, &myDepthStencilViewDesc, &myDepthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	//Set the depth stencil view 
	myDeviceContext->OMSetRenderTargets(1, &myRenderTargetView, myDepthStencilView);

	//Initialize the raster state 
	myRasterDesc.AntialiasedLineEnable = false;
	myRasterDesc.CullMode = D3D11_CULL_BACK;
	myRasterDesc.DepthBias = 0;
	myRasterDesc.DepthBiasClamp = 0.0f;
	myRasterDesc.DepthBiasClamp = true;
	myRasterDesc.FillMode = D3D11_FILL_SOLID;
	myRasterDesc.FrontCounterClockwise = false;
	myRasterDesc.MultisampleEnable = false;
	myRasterDesc.ScissorEnable = false;
	myRasterDesc.SlopeScaledDepthBias = 0.0f;

	//Create the raster state 
	result = myDevice->CreateRasterizerState(&myRasterDesc, &myRasterState);
	if (FAILED(result))
	{
		return false;
	}

	//Set the raster state 
	myDeviceContext->RSSetState(myRasterState);

	//Initialize viewport data 
	myViewport.Width = (float)windowWidth;
	myViewport.Height = (float)windowHeight;
	myViewport.MinDepth = 0.0f;
	myViewport.MaxDepth = 1.0f;
	myViewport.TopLeftX = 0.0f;
	myViewport.TopLeftY = 0.0f;

	//Set the viewport 
	myDeviceContext->RSSetViewports(1, &myViewport);

	//Initialize the projection matrix
	fieldOfView = (float)XM_PI / 4.0f;
	aspectRatio = (float)windowWidth / (float)windowHeight;
	myProjection = XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearPlane, farPlane);

	//Initialize world matrix
	myWorld = XMMatrixIdentity();

	//Initialize the view matrix
	XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -13.0f, 1.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0, 0.0f);
	myView = XMMatrixLookAtLH(Eye, At, Up);

	return true;
}

//Cleans up all object's memory 
void DX::Shutdown()
{
	if (mySwapChain)
	{
		//Bad things happen if you try to release in fullscreen mode  
		mySwapChain->SetFullscreenState(false, NULL);
	}

	if (myDevice)
	{
		myDevice->Release();
		myDevice = nullptr;
	}

	if (myDeviceContext)
	{
		myDeviceContext->Release();
		myDeviceContext = nullptr;
	}

	if (myDepthStencilBuffer)
	{
		myDepthStencilBuffer->Release();
		myDepthStencilBuffer = nullptr;
	}

	if (myDepthStencilState)
	{
		myDepthStencilState->Release();
		myDepthStencilState = nullptr;
	}

	if (myDepthStencilView)
	{
		myDepthStencilView->Release();
		myDepthStencilView = nullptr;
	}

	if (myRenderTargetView)
	{
		myRenderTargetView->Release();
		myRenderTargetView = nullptr;
	}

	if (myRasterState)
	{
		myRasterState->Release();
		myRasterState = nullptr;
	}

	if (mySwapChain)
	{
		mySwapChain->Release();
		mySwapChain = nullptr;
	}
}

//Clears the render target view to a color, and clears the depth stencil view.
//Parameters: The RGBA values of the color to clear the render target view to.
void DX::ClearScreen(float red, float green, float blue, float alpha)
{
	//Create the color
	float color[] = { red, green, blue, alpha };

	//Clear the render target view using the color
	myDeviceContext->ClearRenderTargetView(myRenderTargetView, color);

	//Clear the depth stencil view
	myDeviceContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

//Present with the swap chain 
void DX::PresentScreen()
{
	if (vsync)
	{
		mySwapChain->Present(1, 0);
	}
	else
	{
		//Draw to the screen as fast as possible
		mySwapChain->Present(0, 0);
	}
}

//Device getter 
ID3D11Device *DX::GetDevice()
{
	return myDevice;

}

//Device Context getter
ID3D11DeviceContext *DX::GetDeviceContext()
{
	return myDeviceContext;

}

//Passes the projection matrix into another matrix
void DX::PassProjectionMatrix(XMMATRIX &other)
{
	other = myProjection;
}

//Passes the world matrix into another matrix
void DX::PassWorldMatrix(XMMATRIX &other)
{
	other = myWorld;
}

//Passes the view matrix into another matrix
void DX::PassViewdMatrix(XMMATRIX &other)
{
	other = myView;
}

void DX::SetViewMatrix(XMMATRIX other)
{
	myView = other;
}

XMMATRIX DX::GetViewMatrix()
{
	return myView;
}