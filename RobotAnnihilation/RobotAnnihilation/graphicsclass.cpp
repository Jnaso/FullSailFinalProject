#include "graphicsclass.h"

Graphics::Graphics()
{
	//Zero memory 
	myDX = nullptr;
	myPlayer = nullptr;
	Ground = nullptr;
	myLighting = nullptr;
	myShaderManager = nullptr;
	myCamera = nullptr;
	myDebug = nullptr;
	debugCam = false;
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
	myPlayer = new Player("Assets/Run.mesh", myDX->GetDevice());
	myPlayer->SetRunAnimation(myPlayer->AddAninimation("Assets/Run.anim", myDX->GetDevice()));
	myPlayer->SetIdleAnimation(myPlayer->AddAninimation("Assets/Idle.anim", myDX->GetDevice()));
	myPlayer->SetCurrentAnimation(myPlayer->GetIdleAnimation());
	myPlayer->GetPhysicsComponent()->SetVelocity(float3{ 0, 1.5, 0 });
	myPlayer->GetPhysicsComponent()->SetAccel(float3{0, -0.50, 0});
	myPlayer->GetPhysicsComponent()->SetMass(50);
	myPlayer->GetPhysicsComponent()->SetDamping(.99f);
	if (!myPlayer)
	{
		return false;
	}
	Ground = new GameObject("Assets/GroundPlane.mesh", myDX->GetDevice());
	if (!Ground)
	{
		return false;
	}

	//Make sure the object initializes with no problem 
	result = myPlayer->Initialize(myDX->GetDevice());
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

void Graphics::CreateText(const char * text, int font, float2 pos)
{
	DirectX::SimpleMath::Vector2 temp(pos.x, pos.y);
	myDX->CreateText(text, font, temp);
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

	if (myPlayer)
	{
		myPlayer->Shutdown();
		delete myPlayer;
		myPlayer = nullptr;
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
		myPlayer->SetCurrentAnimation(myPlayer->GetRunAnimation());
	}
	else
	{
		myPlayer->SetCurrentAnimation(myPlayer->GetIdleAnimation());
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


	//world = XMMatrixTranslation(Player->GetPhysicsComponent()->GetPosition().x, Player->GetPhysicsComponent()->GetPosition().y, Player->GetPhysicsComponent()->GetPosition().z);
	myPlayer->Render(myDX->GetDeviceContext());
	if (debugCam)
	{
		myDebug->Update();
	}
	else
	{
		myCamera->Update({ world.r[3].m128_f32[0],  world.r[3].m128_f32[1],  world.r[3].m128_f32[2] });
	}
	myPlayer->Render(myDX->GetDeviceContext());
	myPlayer->GetPhysicsComponent()->SetPosition({ world.r[3].m128_f32[0],  world.r[3].m128_f32[1],  world.r[3].m128_f32[2] });
	result = myShaderManager->RenderAnimatedShader(myDX->GetDeviceContext(), myPlayer->GetObjectIndices().size(), world, view, projection, myPlayer->GetDiffuseTexture(), myPlayer->GetNormalTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), myPlayer->GetCurrentAnimation()->GetJoints(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra());

	myDX->PassWorldMatrix(world);
	for (unsigned int i = 0; i < bullets.size(); i++)
	{
		bullets[i]->Render(myDX->GetDeviceContext());
		world = XMMatrixMultiply(XMMatrixScaling(.25f, .25f, .25f), XMMatrixTranslation(bullets[i]->GetPhysicsComponent()->GetPosition().x, bullets[i]->GetPhysicsComponent()->GetPosition().y, bullets[i]->GetPhysicsComponent()->GetPosition().z));
		result = myShaderManager->RenderStaticShader(myDX->GetDeviceContext(), bullets[i]->GetObjectIndices().size(), world, view, projection, bullets[i]->GetDiffuseTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra());
	}	
	world = XMMatrixIdentity();

	Ground->Render(myDX->GetDeviceContext());

	result = myShaderManager->RenderStaticShader(myDX->GetDeviceContext(), Ground->GetObjectIndices().size(), world, view, projection, Ground->GetDiffuseTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra());

	if (!debugCam)
	{
		myDX->GetDeviceContext()->RSSetState(spriteRasterState);

		myDX->spriteBatch->Begin(SpriteSortMode::SpriteSortMode_Immediate, spriteBlendState, nullptr, spriteDepthState, spriteRasterState);
		 
		myDX->GetDeviceContext()->RSSetState(spriteRasterState);

		unsigned int imageCount = myDX->ImagesToRender.size();

		unsigned int textCount = myDX->TextToRender.size();
		for (unsigned int i = 0; i < textCount; i++)
		{
			int font = myDX->TextToRender[i].m_font;
			const char* text = myDX->TextToRender[i].m_text;
			DirectX::SimpleMath::Vector2 pos = myDX->TextToRender[i].m_pos;
			if (font == F_ARIAL)
			{
				myDX->ArialFont->DrawString(myDX->spriteBatch.get(), text, pos);
			}
			else if (font == F_COMICSANS)
			{
				myDX->ComicSansFont->DrawString(myDX->spriteBatch.get(), text, pos);
			}
		}

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

DX * Graphics::GetDX()
{
	return myDX;
}

void Graphics::Update(InputManager *myInput, float delta)
{
	myPlayer->Update(delta);

	myPlayer->GetPhysicsComponent()->SetForward(float3{ myCamera->GetCharDirection().m128_f32[0], myCamera->GetCharDirection().m128_f32[1], myCamera->GetCharDirection().m128_f32[2] });

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
		if (bullets[i]->GetPhysicsComponent()->GetLifeTime() <= 0.0f)
		{
			bullets[i]->Shutdown();
			bullets.erase(bullets.begin() + i);
		}
	}
}

void Graphics::ShootBullet(HWND hwnd)
{
	GameObject* newBullet = new GameObject("Assets/Sphere.mesh", myDX->GetDevice());
	newBullet->Initialize(myDX->GetDevice());
	float3 forward = float3{ myCamera->GetCharDirection().m128_f32[0], myCamera->GetCharDirection().m128_f32[1], myCamera->GetCharDirection().m128_f32[2] };
	newBullet->GetPhysicsComponent()->SetForward(forward);
	newBullet->GetPhysicsComponent()->SetPosition(float3{myPlayer->GetPhysicsComponent()->GetPosition().x, myPlayer->GetPhysicsComponent()->GetPosition().y + 2.0f, myPlayer->GetPhysicsComponent()->GetPosition().z});
	//float3 velocity = forward * -100.0f;
	newBullet->GetPhysicsComponent()->SetLifeTime(2.0f);
	//velocity = velocity.componentProduct(forward);
	newBullet->GetPhysicsComponent()->SetVelocity(forward * -100.0f);
	newBullet->GetPhysicsComponent()->SetAccel(float3{ 0, -1.0, 0});
	newBullet->GetPhysicsComponent()->SetMass(2.0f);
	newBullet->GetPhysicsComponent()->SetDamping(0.99f);
	bullets.push_back(newBullet);
	myShots.push_back(new Sound((char*)"Gunshot.wav"));
	myShots[myShots.size() - 1]->Initialize(hwnd);
	myShots[myShots.size() - 1]->PlayWaveFile();
}