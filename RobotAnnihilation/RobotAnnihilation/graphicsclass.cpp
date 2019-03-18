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
	Player->GetPhysicsComponent()->SetVelocity(float3{2.5, 2.5, 1.5});
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

	myCamera->SetPosition(0.0f, 0.0f, -5.0f);

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
	myDX->PassViewdMatrix(view);
	myDX->PassProjectionMatrix(projection);

	//Manipulate matricies here 
	if (myInput->GetKeyState((int)'S'))
	{
		myDX->SetViewMatrix(XMMatrixIdentity());
		myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixTranslation(0, 0, .15f));
		myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	}

	if (myInput->GetKeyState((int)'W'))
	{
		myDX->SetViewMatrix(XMMatrixIdentity());
		myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixTranslation(0, 0, -.15f));
		myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	}

	if (myInput->GetKeyState((int)'D'))
	{
		myDX->SetViewMatrix(XMMatrixIdentity());
		myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixTranslation(-.15f, 0, 0));
		myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	}

	if (myInput->GetKeyState((int)'A'))
	{
		myDX->SetViewMatrix(XMMatrixIdentity());
		myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixTranslation(.15f, 0, 0));
		myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	}

	if (myInput->GetKeyState(_SPACE))
	{
		myDX->SetViewMatrix(myDX->GetViewMatrix() = XMMatrixIdentity());
		myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixTranslation(0, .15f, 0));
		myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	}

	if (myInput->GetKeyState((int)'X'))
	{
		myDX->SetViewMatrix(XMMatrixIdentity());
		myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixTranslation(0, -.15f, 0));
		myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	}

	if (myInput->GetKeyState(_ARROWLEFT))
	{
		myDX->SetViewMatrix(XMMatrixIdentity());
		myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixRotationY(.015f));
		myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	}

	if (myInput->GetKeyState(_ARROWRIGHT))
	{
		myDX->SetViewMatrix(XMMatrixIdentity());
		myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixRotationY(-.015f));
		myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	}

	if (myInput->GetKeyState(_ARROWUP))
	{
	    myDX->SetViewMatrix(XMMatrixIdentity());
		myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixRotationX(.015f));
		myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	}

	if (myInput->GetKeyState(_ARROWDOWN))
	{
		myDX->SetViewMatrix(XMMatrixIdentity());
		myDX->SetViewMatrix(myDX->GetViewMatrix()  * XMMatrixRotationX(-.015f));
		myDX->SetViewMatrix(view * myDX->GetViewMatrix());
	}

	Player->Render(myDX->GetDeviceContext());

	world = XMMatrixTranslation(Player->GetPhysicsComponent()->GetPosition().x, Player->GetPhysicsComponent()->GetPosition().y, Player->GetPhysicsComponent()->GetPosition().z);

	result = myShaderManager->RenderAnimatedShader(myDX->GetDeviceContext(), Player->GetObjectIndices().size(), world, view, projection, Player->GetDiffuseTexture(), Player->GetNormalTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), Player->GetCurrentAnimation()->GetJoints());

	for (unsigned int i = 0; i < bullets.size(); i++)
	{
		world = XMMatrixTranslation(bullets[i]->GetPhysicsComponent()->GetPosition().x, bullets[i]->GetPhysicsComponent()->GetPosition().y, bullets[i]->GetPhysicsComponent()->GetPosition().z);
		result = myShaderManager->RenderAnimatedShader(myDX->GetDeviceContext(), bullets[i]->GetObjectIndices().size(), world, view, projection, bullets[i]->GetDiffuseTexture(), bullets[i]->GetNormalTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), Player->GetCurrentAnimation()->GetJoints());
	}

	myDX->PassWorldMatrix(world);

	Ground->Render(myDX->GetDeviceContext());

	result = myShaderManager->RenderStaticShader(myDX->GetDeviceContext(), Ground->GetObjectIndices().size(), world, view, projection, Ground->GetDiffuseTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor());

	if (!result)
	{
		return false;
	}

	//Present the swap chain 
	myDX->PresentScreen();

	return true;
}

void Graphics::Update(float delta)
{
	Player->Update(delta);
	for (unsigned int i = 0; i < bullets.size(); i++)
	{
		bullets[i]->Update(delta);
		if (bullets[i]->GetPhysicsComponent()->GetPosition().x > 10 || bullets[i]->GetPhysicsComponent()->GetPosition().z > 40)
		{
			bullets.erase(bullets.begin() + i);
		}
	}
}

void Graphics::ShootBullet(float x, float y)
{
	GameObject* newBullet = new GameObject("Assets/Sphere.mesh", myDX->GetDevice());
	newBullet->GetPhysicsComponent()->SetPosition(float3{Player->GetPhysicsComponent()->GetPosition().x, Player->GetPhysicsComponent()->GetPosition().y + 1.0f, Player->GetPhysicsComponent()->GetPosition().z});
	newBullet->GetPhysicsComponent()->SetVelocity(float3{ 0, 0, 30});
	bullets.push_back(newBullet);
}