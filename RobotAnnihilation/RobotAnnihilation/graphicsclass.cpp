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
	myDebug = nullptr;
	debugCam = false;
	Target = nullptr;
	timeBetween = timeGetTime();
	playerWorld = XMMatrixIdentity();
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

	//Create Turtle Image
	//myDX->CreateImage("DrawingStuff/turtle.dds", DirectX::SimpleMath::Vector2(0,0));

	//Initialize the game object 
	Player = new GameObject("Assets/Run.mesh", myDX->GetDevice());
	Player->SetRunAnimation(Player->AddAninimation("Assets/Run.anim", myDX->GetDevice()));
	Player->SetIdleAnimation(Player->AddAninimation("Assets/Idle.anim", myDX->GetDevice()));
	Player->SetCurrentAnimation(Player->GetIdleAnimation());
	Player->GetPhysicsComponent()->SetVelocity(float3{ 0, 1.5, 0 });
	Player->GetPhysicsComponent()->SetAccel(float3{0, -0.50, 0});
	Player->GetPhysicsComponent()->SetMass(50);
	Player->GetPhysicsComponent()->SetDamping(.99f);
	if (!Player)
	{
		return false;
	}

	PlayerSphere.center.x = Player->GetPhysicsComponent()->GetPosition().x;
	PlayerSphere.center.y = Player->GetPhysicsComponent()->GetPosition().y + 2.0f;
	PlayerSphere.center.z = Player->GetPhysicsComponent()->GetPosition().z;
	PlayerSphere.radius = 0.8f;

	Ground = new GameObject("Assets/GroundPlane.mesh", myDX->GetDevice());
	if (!Ground)
	{
		return false;
	}

	Target = new GameObject("Assets/Sphere.mesh", myDX->GetDevice());
	if (!Target)
	{
		return false;
	}

	Target->GetPhysicsComponent()->SetPosition(float3{ 0.0f, 2.0f, -20.0f });

	TargetSphe.center = Target->GetPhysicsComponent()->GetPosition();

	TargetSphe.radius = 0.8f;

	result = Target->Initialize(myDX->GetDevice());
	if (!result)
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

	myDebug = new DebugCamera();
	if (!myDebug)
	{
		return false;
	}

	myDebug->SetPosition(0.0f, 2.0f, -5.0f);

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
	//myLighting->SetSpecularColor(1, 0, 1, 1);
	//myLighting->SetSpecularExtra(50, 1.0f, 0, 0);

	myPosition[0] = { 0.0f, 3.0f, 10.0f, 10.0f };
	myPosition[1] = { 0.0f, 3.0f, -10.0f, 20.0f };
	myColors[0] = {0.0f, 1.0f, 0.0f, 1.0f};
	myColors[1] = {0.0f, 1.0f, 1.0f, 1.0f};

	CD3D11_RASTERIZER_DESC rdesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT());
	myDX->GetDevice()->CreateRasterizerState(&rdesc, &spriteRasterState);

	CD3D11_BLEND_DESC bdesc = CD3D11_BLEND_DESC(CD3D11_DEFAULT());
	bdesc.AlphaToCoverageEnable = true;
	myDX->GetDevice()->CreateBlendState(&bdesc, &spriteBlendState);

	CD3D11_DEPTH_STENCIL_DESC sdesc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());
	myDX->GetDevice()->CreateDepthStencilState(&sdesc, &spriteDepthState);

	return true;
}

void Graphics::CreateImage(RECT dimensions, const char * filePath, float2 pos)
{
	DirectX::SimpleMath::Vector2 temp(pos.x, pos.y);
	myDX->CreateImage(dimensions, filePath, temp);
}
void Graphics::CreateImage(const char * filePath, float2 pos)
{
	DirectX::SimpleMath::Vector2 temp(pos.x, pos.y);
	myDX->CreateImage(filePath, temp);
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

	if (Target)
	{
		Target->Shutdown();
		delete Target;
		Target = nullptr;
	}

	if (myCamera)
	{
		delete myCamera;
		myCamera = nullptr;
	}

	if (spriteRasterState)
	{
		spriteRasterState->Release();
		spriteRasterState = nullptr;
	}

	if (myDebug)
	{
		delete myDebug;
		myDebug = nullptr;
	}

	for (int i = 0; i < myShots.size(); i++)
	{
		myShots[i]->Shutdown();
		delete myShots[i];
		myShots[i] = nullptr;
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

	myDX->PassWorldMatrix(world);
	if (debugCam)
	{
		myDebug->PassInViewMatrix(view);
	}
	else
	{
		myCamera->PassInViewMatrix(view);
	}
	myDX->PassProjectionMatrix(projection);

	////Manipulate matricies here
	world = playerWorld;

	camPosition = { view.r[3].m128_f32[0],  view.r[3].m128_f32[1], view.r[3].m128_f32[2], view.r[3].m128_f32[3]};

	//world = XMMatrixTranslation(Player->GetPhysicsComponent()->GetPosition().x, Player->GetPhysicsComponent()->GetPosition().y, Player->GetPhysicsComponent()->GetPosition().z);
	Player->Render(myDX->GetDeviceContext());
	if (debugCam)
	{
		myDebug->Update();
	}
	else
	{
		myCamera->Update({ world.r[3].m128_f32[0],  world.r[3].m128_f32[1],  world.r[3].m128_f32[2] });
	}
	Player->Render(myDX->GetDeviceContext());
	Player->GetPhysicsComponent()->SetPosition({ world.r[3].m128_f32[0],  world.r[3].m128_f32[1],  world.r[3].m128_f32[2] });
	//result = myShaderManager->RenderAnimatedShader(myDX->GetDeviceContext(), Player->GetObjectIndices().size(), world, view, projection, Player->GetDiffuseTexture(), Player->GetNormalTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), Player->GetCurrentAnimation()->GetJoints(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra());
	result = myShaderManager->RenderAnimatedShader(myDX->GetDeviceContext(), Player->GetObjectIndices().size(), world, view, projection, Player->GetDiffuseTexture(), Player->GetNormalTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), Player->GetCurrentAnimation()->GetJoints(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra(), camPosition, myLighting->GetSpecularColor(), myLighting->GetSpecularExtra());

	myDX->PassWorldMatrix(world);
	for (unsigned int i = 0; i < bullets.size(); i++)
	{
		bullets[i]->Render(myDX->GetDeviceContext());
		world = XMMatrixMultiply(XMMatrixScaling(.25f, .25f, .25f), XMMatrixTranslation(bullets[i]->GetPosition().x, bullets[i]->GetPosition().y, bullets[i]->GetPosition().z));
		//result = myShaderManager->RenderStaticShader(myDX->GetDeviceContext(), bullets[i]->GetObjectIndices().size(), world, view, projection, bullets[i]->GetDiffuseTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra());
		result = myShaderManager->RenderStaticShader(myDX->GetDeviceContext(), bullets[i]->GetGameObject()->GetObjectIndices().size(), world, view, projection, bullets[i]->GetGameObject()->GetDiffuseTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra(), camPosition, myLighting->GetSpecularColor(), myLighting->GetSpecularExtra());
	}	
	world = XMMatrixIdentity();

	Ground->Render(myDX->GetDeviceContext());

	//result = myShaderManager->RenderStaticShader(myDX->GetDeviceContext(), Ground->GetObjectIndices().size(), world, view, projection, Ground->GetDiffuseTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra());
	result = myShaderManager->RenderStaticShader(myDX->GetDeviceContext(), Ground->GetObjectIndices().size(), world, view, projection, Ground->GetDiffuseTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra(), camPosition, myLighting->GetSpecularColor(), myLighting->GetSpecularExtra());

	world = XMMatrixMultiply(XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixTranslation(Target->GetPhysicsComponent()->GetPosition().x, Target->GetPhysicsComponent()->GetPosition().y, Target->GetPhysicsComponent()->GetPosition().z));

	Target->Render(myDX->GetDeviceContext());

	result = myShaderManager->RenderStaticShader(myDX->GetDeviceContext(), Target->GetObjectIndices().size(), world, view, projection, Target->GetDiffuseTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra(), camPosition, myLighting->GetSpecularColor(), myLighting->GetSpecularExtra());
	if (!debugCam)
	{
		myDX->GetDeviceContext()->RSSetState(spriteRasterState);

		myDX->spriteBatch->Begin(SpriteSortMode::SpriteSortMode_Immediate, spriteBlendState, nullptr, spriteDepthState, spriteRasterState);

		myDX->GetDeviceContext()->RSSetState(spriteRasterState);
		for (unsigned int i = 0; i < myDX->ImagesToRender.size(); i++)
		{
			myDX->spriteBatch->Draw(myDX->ImagesToRender[i].shaderRes, myDX->ImagesToRender[i].pos);
		}

		myDX->spriteBatch->End();
	}
	
	//Present the swap chain 
	myDX->PresentScreen();

	

	return true;
}

void Graphics::Update(InputManager *myInput, float delta)
{
	Player->Update(delta);

	Player->GetPhysicsComponent()->SetForward(float3{ myCamera->GetDirection().m128_f32[0], myCamera->GetDirection().m128_f32[1], myCamera->GetDirection().m128_f32[2] });

	if(debugCam)	
	{
		myDebug->GetInput(myInput, delta);
	}
	else
	{
		myCamera->GetInput(myInput, delta, playerWorld);
	}

	if (myInput->GetKeyState((int)'O'))
	{
		if (timeGetTime() >= timeBetween + 300)
		{
			debugCam = !debugCam;
			timeBetween = timeGetTime();
		}
	}

	for (unsigned int i = 0; i < bullets.size(); i++)
	{
		bullets[i]->Update(delta);
		if (bullets[i]->Destroy())
		{
			Bullet *temp;
			bullets[i]->Shutdown();
			temp = bullets[i];
			bullets.erase(bullets.begin() + i);
			delete temp;
		}
	}

	PlayerSphere.center.x = Player->GetPhysicsComponent()->GetPosition().x;
	PlayerSphere.center.y = Player->GetPhysicsComponent()->GetPosition().y + 2.0f;
	PlayerSphere.center.z = Player->GetPhysicsComponent()->GetPosition().z;

	if (MovingSphereToSphere(PlayerSphere, Player->GetPhysicsComponent()->GetVelocity(), TargetSphe, delta))
	{
		std::cout << "Boom, Collision!" << std::endl;
	}

	system("CLS");
}

void Graphics::ShootBullet(float x, float y, HWND hwnd)
{
	float3 forward = float3{ myCamera->GetDirection().m128_f32[0], myCamera->GetDirection().m128_f32[1], myCamera->GetDirection().m128_f32[2] };
	float3 playerPos = { Player->GetPhysicsComponent()->GetPosition().x, Player->GetPhysicsComponent()->GetPosition().y + 2.0f, Player->GetPhysicsComponent()->GetPosition().z };
	bullets.push_back(new Bullet());
	bullets[bullets.size() - 1]->Initialize(myDX->GetDevice(), "Assets/Sphere.mesh", forward, playerPos);
	myShots.push_back(new Sound((char*)"Gunshot.wav"));
	myShots[myShots.size() - 1]->Initialize(hwnd);
	myShots[myShots.size() - 1]->PlayWaveFile();
}