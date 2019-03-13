#include "graphicsclass.h"

Graphics::Graphics()
{
	//Zero memory 
	myDX = nullptr;
	Player = nullptr;
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
	Player = new GameObject("Assets/Run.mesh", myDX->GetDevice());
	Player->AddAninimation("Assets/Run.anim", myDX->GetDevice(), *Player->GetRunAnimation());
	if (!Player)
	{
		return false;
	}

	//Make sure the object initializes with no problem 
	result = Player->Initialize(myDX->GetDevice());
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

	myLighting->SetDirectionalColor(1.0f, 0.0f, 0.0f, 1.0f);
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

	if (Player)
	{
		Player->Shutdown();
		delete Player;
		Player = nullptr;
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
	if (GetAsyncKeyState(0x53))
	{
		myDX->SetViewMatrix(XMMatrixIdentity());
		myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixTranslation(0, 0, .15f));
		myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	}

	if (GetAsyncKeyState(0x57))
	{
		myDX->SetViewMatrix(XMMatrixIdentity());
		myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixTranslation(0, 0, -.15f));
		myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	}

	if (GetAsyncKeyState(0x44))
	{
		myDX->SetViewMatrix(XMMatrixIdentity());
		myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixTranslation(-.15f, 0, 0));
		myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	}

	if (GetAsyncKeyState(0x41))
	{
		myDX->SetViewMatrix(XMMatrixIdentity());
		myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixTranslation(.15f, 0, 0));
		myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	}

	if (GetAsyncKeyState(0x20))
	{
		myDX->SetViewMatrix(myDX->GetViewMatrix() = XMMatrixIdentity());
		myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixTranslation(0, .15f, 0));
		myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	}

	if (GetAsyncKeyState(0x58))
	{
		myDX->SetViewMatrix(XMMatrixIdentity());
		myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixTranslation(0, -.15f, 0));
		myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	}

	if (GetAsyncKeyState(VK_LEFT))
	{
		myDX->SetViewMatrix(XMMatrixIdentity());
		myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixRotationY(.015f));
		myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	}

	if (GetAsyncKeyState(VK_RIGHT))
	{
		myDX->SetViewMatrix(XMMatrixIdentity());
		myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixRotationY(-.015f));
		myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	}

	if (GetAsyncKeyState(VK_UP))
	{
		myDX->SetViewMatrix(XMMatrixIdentity());
		myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixRotationX(.015f));
		myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	}

	if (GetAsyncKeyState(VK_DOWN))
	{
		myDX->SetViewMatrix(XMMatrixIdentity());
		myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixRotationX(-.015f));
		myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	}

	Player->Render(myDX->GetDeviceContext());

	result = myShaders->Render(myDX->GetDeviceContext(), Player->GetObjectIndices().size(), world, view, projection, Player->GetDiffuseTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor());
	if (!result)
	{
		return false;
	}

	//Present the swap chain 
	myDX->PresentScreen();

	return true;
}