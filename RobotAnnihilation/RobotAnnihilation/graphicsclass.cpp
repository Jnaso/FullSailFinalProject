#include "graphicsclass.h"

Graphics::Graphics()
{
	//Zero memory 
	myDX = nullptr;
	myObject = nullptr;
	myLighting = nullptr;
	myShaders = nullptr;
}

bool Graphics::Initialize(int windowWidth, int windowHeight, HWND window)
{
	bool result;

	//Initialize the direct x object 
	myDX = new DX();
	if (!myDX)
	{
		return false;
	}

	//Make sure the object initializes with no problem 
	result = myDX->Initialize(windowWidth, windowHeight, VSYNC_ENABLED, window, FULL_SCREEN, FAR_PLANE, NEAR_PLANE);
	if (!result)
	{
		return false;
	}

	//Initialize the game object 
	myObject = new GameObject();
	if (!myObject)
	{
		return false;
	}

	//Make sure the object initializes with no problem 
	result = myObject->Initialize("Assests/Run.mesh", myDX->GetDevice());
	if (!result)
	{
		return false;
	}

	//Initialize the shader object 
	myShaders = new Shaders();
	if (!myShaders)
	{
		return false;
	}

	//Initialize the shaders
	result = myShaders->Initialize(myDX->GetDevice());
	if (!result)
	{
		return false;
	}

	//Initialize the lighting 
	myLighting = new Lighting();
	if (!myLighting)
	{
		return false;
	}

	myLighting->SetDirectionalColor(1.0f, 1.0f, 1.0f, 1.0f);
	myLighting->SetDirection(0.0f, 0.0f, 1.0f);

	return true;
}

//Pointer clean up 
void Graphics::Shutdown()
{
	if (myDX)
	{
		myDX->Shutdown();
		delete myDX;
		myDX = nullptr;
	}

	if (myLighting)
	{
		delete myLighting;
		myLighting = 0;
	}

	if (myShaders)
	{
		myShaders->Shutdown();
		delete myShaders;
		myShaders = 0;
	}

	if (myObject)
	{
		myObject->Shutdown();
		delete myObject;
		myObject = nullptr;
	}

}

//Called each frame 
bool Graphics::Render()
{
	XMMATRIX world, view, projection;
	bool result;

	HRESULT hr;

	//Clear the screen 
	myDX->ClearScreen(0.0f, 1.0f, 0.0f, 1.0f);

	myDX->PassWorldMatrix(world);
	myDX->PassViewdMatrix(view);
	myDX->PassProjectionMatrix(projection);

	//Manipulate matricies here 

	myObject->Render(myDX->GetDeviceContext());

	ID3D11ShaderResourceView * myShade;
	hr = CreateWICTextureFromFile(myDX->GetDevice(), L"Assests/Run.fbm/PPG_3D_Player_D.png", nullptr, &myShade);
	result = myShaders->Render(myDX->GetDeviceContext(), myObject->GetObjectIndices().size(), world, view, projection, myShade, myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor());
	if (!result)
	{
		return false;
	}

	//Present the swap chain 
	myDX->PresentScreen();

	return true;
}