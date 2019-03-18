#include "graphicsclass.h"

Graphics::Graphics()
{
	//Zero memory 
	myDX = nullptr;
	Player = nullptr;
	Ground = nullptr;
	myLighting = nullptr;
	myShaderManager = nullptr;
	myCamera = nullptr;
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
	Player->SetRunAnimation(Player->AddAninimation("Assets/Run.anim", myDX->GetDevice()));
	Player->SetIdleAnimation(Player->AddAninimation("Assets/Idle.anim", myDX->GetDevice()));
	Player->SetCurrentAnimation(Player->GetIdleAnimation());
	if (!Player)
	{
		return false;
	}
	Ground = new GameObject("Assets/GroundPlane.mesh", myDX->GetDevice());
	if (!Ground)
	{
		return false;
	}

	//Make sure the object initializes with no problem 
	result = Player->Initialize(myDX->GetDevice());
	if (!result)
	{
		return false;
	}

	result = Ground->Initialize(myDX->GetDevice());

	//Initialize the shader object 
	myShaderManager = new ShaderManager();
	if (!myShaderManager)
	{
		return false;
	}

	//Initialize the shaders
	result = myShaderManager->Initialize(myDX->GetDevice());
	if (!result)
	{
		return false;
	}

	//Intialize the camera
	myCamera = new Camera();
	if (!myCamera)
	{
		return false;
	}

	myCamera->SetPosition(0.0f, 3.0f, -7.0f);

	//Initialize the lighting 
	myLighting = new Lighting();
	if (!myLighting)
	{
		return false;
	}

	myLighting->SetDirectionalColor(1.0f, 0.0f, 0.0f, 1.0f);
	myLighting->SetDirection(0.0f, 0.0f, 1.0f);
	myLighting->SetSpotlightColor(1.0f, 1.0f, 1.0f, 1.0f);
	myLighting->SetSpotlightDirection(0, -1, 0, 1);
	myLighting->SetSpotlightPosition(0, 25, 0, 1);
	myLighting->SetSpotlightExtra(0.988f, 0.986f, 100, 1);

	myPosition[0] = { 0.0f, 3.0f, 10.0f, 10.0f };
	myPosition[1] = { 0.0f, 3.0f, -10.0f, 20.0f };
	myColors[0] = {0.0f, 1.0f, 0.0f, 1.0f};
	myColors[1] = {0.0f, 1.0f, 1.0f, 1.0f};

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
		myLighting = nullptr;
	}

	if (myShaderManager)
	{
		myShaderManager->Shutdown();
		delete myShaderManager;
		myShaderManager = nullptr;
	}

	if (Player)
	{
		Player->Shutdown();
		delete Player;
		Player = nullptr;
	}

	if (Ground)
	{
		Ground->Shutdown();
		delete Ground;
		Ground = nullptr;
	}

	if (myCamera)
	{
		delete myCamera;
		myCamera = nullptr;
	}

}

//Called each frame 
bool Graphics::Render(InputManager *myInput)
{
	bool moving = false;
	if (myInput->GetKeyState((int)'W') || myInput->GetKeyState((int)'A') || myInput->GetKeyState((int)'S') || myInput->GetKeyState((int)'D'))
	{
		moving = true;
	}
	if (moving)
	{
		Player->SetCurrentAnimation(Player->GetRunAnimation());
	}
	else
	{
		Player->SetCurrentAnimation(Player->GetIdleAnimation());
	}
	XMMATRIX world, view, projection;
	bool result;

	HRESULT hr;

	//Clear the screen 
	myDX->ClearScreen(0.0f, 1.0f, 0.0f, 1.0f);

	myCamera->Update();

	myDX->PassWorldMatrix(world);
	myCamera->PassInViewMatrix(view);
	myDX->PassProjectionMatrix(projection);

	////Manipulate matricies here 
	//if (myInput->GetKeyState((int)'S'))
	//{
	//	myDX->SetViewMatrix(XMMatrixIdentity());
	//	myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixTranslation(0, 0, .15f));
	//	myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	//	myDX->SetWorldMatrix(XMMatrixIdentity());
	//	myDX->SetWorldMatrix(myDX->GetWorldMatrix() * XMMatrixTranslation(0, 0, -.15f));
	//	myDX->SetWorldMatrix(world * myDX->GetWorldMatrix());
	//}

	//if (myInput->GetKeyState((int)'W'))
	//{
	//	myDX->SetViewMatrix(XMMatrixIdentity());
	//	myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixTranslation(0, 0, -.15f));
	//	myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	//	myDX->SetWorldMatrix(XMMatrixIdentity());
	//	myDX->SetWorldMatrix(myDX->GetWorldMatrix() * XMMatrixTranslation(0, 0, .15f));
	//	myDX->SetWorldMatrix(world * myDX->GetWorldMatrix());
	//}

	//if (myInput->GetKeyState((int)'D'))
	//{
	//	myDX->SetViewMatrix(XMMatrixIdentity());
	//	myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixTranslation(-.15f, 0, 0));
	//	myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	//	myDX->SetWorldMatrix(XMMatrixIdentity());
	//	myDX->SetWorldMatrix(myDX->GetWorldMatrix() * XMMatrixTranslation(.15f, 0, 0));
	//	myDX->SetWorldMatrix(world * myDX->GetWorldMatrix());
	//}

	//if (myInput->GetKeyState((int)'A'))
	//{
	//	myDX->SetViewMatrix(XMMatrixIdentity());
	//	myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixTranslation(.15f, 0, 0));
	//	myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	//	myDX->SetWorldMatrix(XMMatrixIdentity());
	//	myDX->SetWorldMatrix(myDX->GetWorldMatrix() * XMMatrixTranslation(-.15f, 0, 0));
	//	myDX->SetWorldMatrix(world * myDX->GetWorldMatrix());
	//}

	////if (myInput->GetKeyState(_SPACE))
	////{
	////	myDX->SetViewMatrix(myDX->GetViewMatrix() = XMMatrixIdentity());
	////	myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixTranslation(0, .15f, 0));
	////	myDX->SetViewMatrix(view * myDX->GetViewMatrix());

	////}

	////if (myInput->GetKeyState((int)'X'))
	////{
	////	myDX->SetViewMatrix(XMMatrixIdentity());
	////	myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixTranslation(0, -.15f, 0));
	////	myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	////}

	//if (myInput->GetKeyState(_ARROWLEFT))
	//{
	//	myDX->SetViewMatrix(XMMatrixIdentity());
	//	myDX->SetViewMatrix(XMMatrixRotationY(.015f) * world);
	//	myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	//}

	//if (myInput->GetKeyState(_ARROWRIGHT))
	//{
	//	myDX->SetViewMatrix(XMMatrixIdentity());
	//	myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixRotationY(-.015f));
	//	myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	//}

	//if (myInput->GetKeyState(_ARROWUP))
	//{
	//    myDX->SetViewMatrix(XMMatrixIdentity());
	//	myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixRotationX(.015f));
	//	myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	//}

	//if (myInput->GetKeyState(_ARROWDOWN))
	//{
	//	myDX->SetViewMatrix(XMMatrixIdentity());
	//	myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixRotationX(-.015f));
	//	myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	//}

	Player->Render(myDX->GetDeviceContext());

	result = myShaderManager->RenderAnimatedShader(myDX->GetDeviceContext(), Player->GetObjectIndices().size(), world, view, projection, Player->GetDiffuseTexture(), Player->GetNormalTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), Player->GetCurrentAnimation()->GetJoints(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra());

	world = XMMatrixIdentity();

	Ground->Render(myDX->GetDeviceContext());

	result = myShaderManager->RenderStaticShader(myDX->GetDeviceContext(), Ground->GetObjectIndices().size(), world, view, projection, Ground->GetDiffuseTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra());

	if (!result)
	{
		return false;
	}

	//Present the swap chain 
	myDX->PresentScreen();

	return true;
}

void Graphics::Update(InputManager *myInput, float delta)
{
	Player->Update(delta);
	myCamera->GetInput(myInput, delta);
}