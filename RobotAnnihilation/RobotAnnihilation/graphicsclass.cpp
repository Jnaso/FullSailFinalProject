#include "graphicsclass.h"

Graphics::Graphics(InputManager* input)
{
	//Zero memory 
	myDX = nullptr;
	Ground = nullptr;
	Skybox = nullptr;
	Walls = nullptr;
	myLighting = nullptr;
	myShaderManager = nullptr;
	myCamera = nullptr;
	myDebug = nullptr;
	debugCam = false;
	timeBetween = timeGetTime();
	playerWorld = XMMatrixIdentity();
	myUI = nullptr;
}

bool Graphics::Initialize(int windowWidth, int windowHeight, HWND window, InputManager *myInput)
{
	bool result;

	playerWorld = XMMatrixIdentity();

	//Initialize the direct x object 
	if (!myDX)
	{
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
	}


	//Create Turtle Image
	//myDX->CreateImage("DrawingStuff/turtle.dds", DirectX::SimpleMath::Vector2(0,0));

	//Initialize the game object 
	Ground = new GameObject();
	Ground->Initialize("Assets/Ground.mesh", myDX->GetDevice());	
	if (!Ground)
	{
		return false;
	}

	Skybox = new GameObject();
	Skybox->Initialize("Assets/SkyBox.mesh", myDX->GetDevice());
	if (!Skybox)
	{
		return false;
	}

	Walls = new GameObject();
	Walls->Initialize("Assets/OutsideWalls.mesh", myDX->GetDevice());
	if (!Walls)
	{
		return false;
	}

	//myTargets.push_back(new Target());
	//myTargets[myTargets.size() - 1]->Initialize(myDX->GetDevice(), "Assets/Sphere.mesh", float3{ 0.0f, 2.0f, -20.0f });

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
	if (!myCamera)
	{
		myCamera = new Camera();
		if (!myCamera)
		{
			return false;
		}
	}

	myCamera->SetPosition(0.0f, 3.0f, -7.0f);

	if (!myDebug)
	{
		myDebug = new DebugCamera();
		if (!myDebug)
		{
			return false;
		}
	}

	myDebug->SetPosition(0.0f, 2.0f, -5.0f);

	//Initialize the lighting 
	myLighting = new Lighting();
	if (!myLighting)
	{
		return false;
	}

	myLighting->SetDirectionalColor(1.0f, 1.0f, 1.0f, 1.0f);
	myLighting->SetDirection(0.0f, 0.0f, 1.0f);
	myLighting->SetSpotlightColor(1.0f, 1.0f, 1.0f, 1.0f);
	myLighting->SetSpotlightDirection(0, -1, 0, 1);
	myLighting->SetSpotlightPosition(0, 25, 0, 1);
	myLighting->SetSpotlightExtra(0.988f, 0.986f, 100, 1);
	//myLighting->SetSpecularColor(1, 0, 1, 1);
	//myLighting->SetSpecularExtra(50, 1.0f, 0, 0);

	myPosition[0] = { 0.0f, 3.0f, 10.0f, 5.0f };
	myPosition[1] = { 0.0f, 3.0f, -10.0f, 5.0f };
	myColors[0] = {0.0f, 0.0f, 0.0f, 0.0f};
	myColors[1] = {0.0f, 0.0f, 0.0f, 0.0f};

	myFrustum = new Frustum();
	if (!myFrustum)
	{
		return false;
	}

	//Create UI Manager
	if (!myUI)
	{
		myUI = new UIManager(myInput, myDX->GetDevice(), myDX->GetDeviceContext());
		if (!myUI)
		{
			return false;
		}
	}

	if (!m_spriteBatch)
	{
		m_spriteBatch.reset(new DirectX::SpriteBatch(myDX->GetDeviceContext()));
		if (!m_spriteBatch)
		{
			return false;
		}
	}

	if (!m_arialFont)
	{
		m_arialFont.reset(new DirectX::SpriteFont(myDX->GetDevice(), L"DrawingStuff/Arial.spritefont"));
		if (!m_arialFont)
		{
			return false;
		}
	}

	if (!m_comicSansFont)
	{
		m_comicSansFont.reset(new DirectX::SpriteFont(myDX->GetDevice(), L"DrawingStuff/ComicSans.spritefont"));
		if (!m_comicSansFont)
		{
			return false;
		}
	}

	//return true;

	HRESULT hr;

	CD3D11_RASTERIZER_DESC rdesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT());
	hr = myDX->GetDevice()->CreateRasterizerState(&rdesc, &spriteRasterState);
	if (FAILED(hr))
	{
		return false;
	}
	CD3D11_BLEND_DESC bdesc = CD3D11_BLEND_DESC(CD3D11_DEFAULT());
	bdesc.AlphaToCoverageEnable = true;
	bdesc.IndependentBlendEnable = true;
	hr = myDX->GetDevice()->CreateBlendState(&bdesc, &spriteBlendState);
	if (FAILED(hr))
	{
		return false;
	}
	CD3D11_DEPTH_STENCIL_DESC sdesc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());
	hr = myDX->GetDevice()->CreateDepthStencilState(&sdesc, &spriteDepthState);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void Graphics::CreateText(RECT srcRect, bool interactable, bool enabled, float2 pos, int font, const char * text)
{
	myUI->CreateText(srcRect, interactable, enabled, pos, font, text);
}

void Graphics::CreateImage(RECT srcRect, bool interactable, bool enabled, float2 pos, const char * filePath)
{
	myUI->CreateImage(srcRect, interactable, enabled, pos, filePath, myDX->GetDevice());
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
	if (Ground)
	{
		Ground->Shutdown();
		delete Ground;
		Ground = nullptr;
	}

	if (Skybox)
	{
		Skybox->Shutdown();
		delete Skybox;
		Skybox = nullptr;
	}

	if (Walls)
	{
		Walls->Shutdown();
		delete Walls;
		Walls = nullptr;
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

	if (myFrustum)
	{
		delete myFrustum;
		myFrustum = nullptr;
	}

	//Clean up for 2d graphics
	if (m_spriteBatch) { m_spriteBatch.release(); }
	if (m_arialFont) { m_arialFont.release(); }
	if (m_comicSansFont) { m_comicSansFont.release(); }
}

//Called each frame 
bool Graphics::Render(InputManager *myInput, Player* myPlayer, std::vector<Bullet*> bullets, vector<Enemy*> myTargets, vector<GameObject*> Obstacles, vector<Pickup*> Pickups)
{
	XMMATRIX world, view, projection, frustumView;
	bool result, render;
	unsigned int renderCount = 0;

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
	myCamera->PassInViewMatrix(frustumView);

	myDX->PassProjectionMatrix(projection);

	camPosition = { view.r[3].m128_f32[0],  view.r[3].m128_f32[1], view.r[3].m128_f32[2], view.r[3].m128_f32[3] };

	XMVECTOR myCam = XMMatrixInverse(nullptr, XMMatrixTranspose(view)).r[3];
	XMFLOAT4 skyboxPosition;
	XMStoreFloat4(&skyboxPosition, myCam);

	//if (!myUI->GetUIElements()[4]->GetEnabled())
	if (!myUI->m_mainMenu && !myUI->m_pauseMenu)
	{
		myDX->SetSkyboxRaster();

		world = XMMatrixScaling(900.0f, 900.0f, 900.f);

		Skybox->Render(myDX->GetDeviceContext());

		myShaderManager->RenderSkyboxShader(myDX->GetDeviceContext(), Skybox->GetModelComponent()->GetObjectIndices().size(), world, view, projection, myShaderManager->GetSkyBox());

		myDX->SetRegularRaster();

		world = playerWorld;
		//world = XMMatrixTranslation(Player->GetPhysicsComponent()->GetPosition().x, Player->GetPhysicsComponent()->GetPosition().y, Player->GetPhysicsComponent()->GetPosition().z);
		
		if (debugCam)
		{
			myDebug->Update();
		}
		else
		{
			XMMATRIX worldcopy = world;
			world = XMMatrixMultiply(XMMatrixTranslation(1.2f, 0.75f, 0), worldcopy);
			myCamera->Update({ world.r[3].m128_f32[0],  world.r[3].m128_f32[1],  world.r[3].m128_f32[2] });
		}

		XMMATRIX worldcopy = world;
		world = XMMatrixMultiply(XMMatrixScaling(.035, .035, .035), XMMatrixMultiply(XMMatrixTranslation(-1.2f, -.75, 0), worldcopy));
		myPlayer->Render(myDX->GetDeviceContext());
		myPlayer->GetPhysicsComponent()->SetPosition({ world.r[3].m128_f32[0],  world.r[3].m128_f32[1],  world.r[3].m128_f32[2] });
		//result = myShaderManager->RenderAnimatedShader(myDX->GetDeviceContext(), Player->GetObjectIndices().size(), world, view, projection, Player->GetDiffuseTexture(), Player->GetNormalTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), Player->GetCurrentAnimation()->GetJoints(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra());
		result = myShaderManager->RenderAnimatedShader(myDX->GetDeviceContext(), myPlayer->GetModelComponent()->GetObjectIndices().size(), world, view, projection, myPlayer->GetModelComponent()->GetDiffuseTexture(), myPlayer->GetModelComponent()->GetNormalTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), myPlayer->GetJoints(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra(), camPosition, myLighting->GetSpecularColor(), myLighting->GetSpecularExtra());
		myDX->PassWorldMatrix(world);
		for (unsigned int i = 0; i < bullets.size(); i++)
		{
			bullets[i]->Render(myDX->GetDeviceContext());
			world = XMMatrixMultiply(XMMatrixScaling(.25f, .25f, .25f), XMMatrixTranslation(bullets[i]->GetPhysicsComponent()->GetPosition().x, bullets[i]->GetPhysicsComponent()->GetPosition().y, bullets[i]->GetPhysicsComponent()->GetPosition().z));
			//result = myShaderManager->RenderStaticShader(myDX->GetDeviceContext(), bullets[i]->GetObjectIndices().size(), world, view, projection, bullets[i]->GetDiffuseTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra());
			result = myShaderManager->RenderStaticShader(myDX->GetDeviceContext(), bullets[i]->GetModelComponent()->GetObjectIndices().size(), world, view, projection, bullets[i]->GetModelComponent()->GetDiffuseTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra(), camPosition, myLighting->GetSpecularColor(), myLighting->GetSpecularExtra());
		}
		world = XMMatrixIdentity();
		Ground->Render(myDX->GetDeviceContext());

		//result = myShaderManager->RenderStaticShader(myDX->GetDeviceContext(), Ground->GetObjectIndices().size(), world, view, projection, Ground->GetDiffuseTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra());
		result = myShaderManager->RenderStaticShader(myDX->GetDeviceContext(), Ground->GetModelComponent()->GetObjectIndices().size(), world, view, projection, Ground->GetModelComponent()->GetDiffuseTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra(), camPosition, myLighting->GetSpecularColor(), myLighting->GetSpecularExtra());

		world = XMMatrixIdentity();
		Walls->Render(myDX->GetDeviceContext());

		result = myShaderManager->RenderStaticShader(myDX->GetDeviceContext(), Walls->GetModelComponent()->GetObjectIndices().size(), world, view, projection, Walls->GetModelComponent()->GetDiffuseTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra(), camPosition, myLighting->GetSpecularColor(), myLighting->GetSpecularExtra());

		myFrustum->Construct(FAR_PLANE, projection, frustumView);
		
		for (unsigned int i = 0; i < myTargets.size(); i++)
		{

			render = myFrustum->CheckSphere(*myTargets[i]->GetCollider(0));

			if (render)
			{
				XMVECTOR player = playerWorld.r[3];
				player.m128_f32[1] += 2.0f;
				XMMATRIX lookcopy = XMMatrixTranslation(myTargets[i]->GetPhysicsComponent()->GetPosition().x, myTargets[i]->GetPhysicsComponent()->GetPosition().y, myTargets[i]->GetPhysicsComponent()->GetPosition().z);
				XMVECTOR zclamp = XMVectorSubtract(player, lookcopy.r[3]);
				zclamp.m128_f32[1] = 0;
				XMVECTOR z = XMVector4Normalize(zclamp);// { UserControlled.r[3].m128_f32[0] - TurnToResult.r[3].m128_f32[0], UserControlled.r[3].m128_f32[1] - TurnToResult.r[3].m128_f32[1], UserControlled.r[3].m128_f32[2] - TurnToResult.r[3].m128_f32[2], UserControlled.r[3].m128_f32[3] - TurnToResult.r[3].m128_f32[3] };
				XMVECTOR up = { 0, 1, 0, 0 };
				XMVECTOR x = XMVector4Normalize(XMVector3Cross(up, z));// { (up.m128_f32[1] * z.m128_f32[2]) - (up.m128_f32[2] * z.m128_f32[1]), (up.m128_f32[2] * z.m128_f32[0]) - (up.m128_f32[0] * z.m128_f32[2]), (up.m128_f32[0] * z.m128_f32[1]) - (up.m128_f32[1] * z.m128_f32[0]), 1 };
				XMVECTOR y = XMVector4Normalize(XMVector3Cross(z, x));// { (z.m128_f32[1] * x.m128_f32[2]) - (z.m128_f32[2] * x.m128_f32[1]), (z.m128_f32[2] * x.m128_f32[0]) - (z.m128_f32[0] * x.m128_f32[2]), (z.m128_f32[0] * x.m128_f32[1]) - (z.m128_f32[1] * x.m128_f32[0]), 1 };
				lookcopy.r[0] = x;// XMMatrixSet(x.m128_f32[0], x.m128_f32[1], x.m128_f32[2], x.m128_f32[3], y.m128_f32[0], y.m128_f32[1], y.m128_f32[2], y.m128_f32[3], z.m128_f32[0], z.m128_f32[1], z.m128_f32[2], z.m128_f32[2], myTargets[i]->GetPhysicsComponent()->GetPosition().x, myTargets[i]->GetPhysicsComponent()->GetPosition().y, myTargets[i]->GetPhysicsComponent()->GetPosition().z, world.r[3].m128_f32[3]);
				lookcopy.r[1] = y;
				lookcopy.r[2] = z;
				lookcopy.r[3] = float3toXMVector(myTargets[i]->GetPhysicsComponent()->GetPosition());
				lookcopy.r[3].m128_f32[1] -= 1.0f;// XMMatrixSet(x.m128_f32[0], x.m128_f32[1], x.m128_f32[2], x.m128_f32[3], y.m128_f32[0], y.m128_f32[1], y.m128_f32[2], y.m128_f32[3], z.m128_f32[0], z.m128_f32[1], z.m128_f32[2], z.m128_f32[2], myTargets[i]->GetPhysicsComponent()->GetPosition().x, myTargets[i]->GetPhysicsComponent()->GetPosition().y, myTargets[i]->GetPhysicsComponent()->GetPosition().z, world.r[3].m128_f32[3]);
				RangedEnemy* currEnemy = dynamic_cast<RangedEnemy*>(myTargets[i]);
				if (currEnemy)
				{
					world = XMMatrixMultiply(XMMatrixScaling(4, 4, 4), lookcopy);
				}
				else
				{
					world = XMMatrixMultiply(XMMatrixScaling(2, 2, 2), lookcopy);
				}
				//world = lookcopy;

				//world = XMMatrixTranslation(myTargets[i]->GetPhysicsComponent()->GetPosition().x, myTargets[i]->GetPhysicsComponent()->GetPosition().y - 1.0f, myTargets[i]->GetPhysicsComponent()->GetPosition().z);

				myTargets[i]->Render(myDX->GetDeviceContext());

				//result = myShaderManager->RenderAnimatedShader(myDX->GetDeviceContext(), myPlayer->GetModelComponent()->GetObjectIndices().size(), world, view, projection, myPlayer->GetModelComponent()->GetDiffuseTexture(), myPlayer->GetModelComponent()->GetNormalTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), myPlayer->GetJoints(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra(), camPosition, myLighting->GetSpecularColor(), myLighting->GetSpecularExtra());
				result = myShaderManager->RenderAnimatedShader(myDX->GetDeviceContext(), myTargets[i]->GetModelComponent()->GetObjectIndices().size(), world, view, projection, myTargets[i]->GetModelComponent()->GetDiffuseTexture(), myTargets[i]->GetModelComponent()->GetNormalTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), myTargets[i]->GetCurrentAnimation()->GetJoints(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra(), camPosition, myLighting->GetSpecularColor(), myLighting->GetSpecularExtra(), myTargets[i]->GetHurt());
				//renderCount++;
			}

			//result = myShaderManager->RenderAnimatedShader(myDX->GetDeviceContext(), myTargets[i]->GetModelComponent()->GetObjectIndices().size(), world, view, projection, myTargets[i]->GetModelComponent()->GetDiffuseTexture(), myTargets[i]->GetModelComponent()->GetNormalTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), myTargets[i]->GetCurrentAnimation()->GetJoints(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra(), camPosition, myLighting->GetSpecularColor(), myLighting->GetSpecularExtra());
		}


		for (unsigned int i = 0; i < Obstacles.size(); i++)
		{
			render = myFrustum->CheckSphere(*Obstacles[i]->GetCollider(0));
			if (render)
			{
				world = XMMatrixMultiply(XMMatrixScaling(1.0f, 3.0f, 1.0f), XMMatrixTranslation(Obstacles[i]->GetPhysicsComponent()->GetPosition().x, Obstacles[i]->GetPhysicsComponent()->GetPosition().y + 2, Obstacles[i]->GetPhysicsComponent()->GetPosition().z));

				Obstacles[i]->Render(myDX->GetDeviceContext());
				result = myShaderManager->RenderStaticShader(myDX->GetDeviceContext(), Obstacles[i]->GetModelComponent()->GetObjectIndices().size(), world, view, projection, Obstacles[i]->GetModelComponent()->GetDiffuseTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra(), camPosition, myLighting->GetSpecularColor(), myLighting->GetSpecularExtra());
			}
		}

		for (unsigned int i = 0; i < Pickups.size(); i++)
		{
			if (Pickups[i]->GetType() == Pickup::PickupType::DAMAGE)
			{
				world = XMMatrixMultiply(XMMatrixMultiply(XMMatrixScaling(1.25, 1.25, 1.25), XMMatrixMultiply(XMMatrixTranslation(0, sin(Pickups[i]->GetTotalTime() * 10.0f * .25 + 1.5), 0), XMMatrixRotationY(Pickups[i]->GetTotalTime()))), XMMatrixTranslation(Pickups[i]->GetPhysicsComponent()->GetPosition().x, Pickups[i]->GetPhysicsComponent()->GetPosition().y, Pickups[i]->GetPhysicsComponent()->GetPosition().z));
			}
			else
			{
				world = XMMatrixMultiply(XMMatrixMultiply(XMMatrixTranslation(0, sin(Pickups[i]->GetTotalTime() * 10.0f * .25 + 1.5), 0), XMMatrixRotationY(Pickups[i]->GetTotalTime())), XMMatrixTranslation(Pickups[i]->GetPhysicsComponent()->GetPosition().x, Pickups[i]->GetPhysicsComponent()->GetPosition().y, Pickups[i]->GetPhysicsComponent()->GetPosition().z));
			}
			Pickups[i]->Render(myDX->GetDeviceContext());

			result = myShaderManager->RenderStaticShader(myDX->GetDeviceContext(), Pickups[i]->GetModelComponent()->GetObjectIndices().size(), world, view, projection, Pickups[i]->GetModelComponent()->GetDiffuseTexture(), myLighting->GetDirectionalDirection(), myLighting->GetDirectionalColor(), myPosition, myColors, myLighting->GetSpotlightColor(), myLighting->GetSpotlightDirection(), myLighting->GetSpotlightPosition(), myLighting->GetSpotlightExtra(), camPosition, myLighting->GetSpecularColor(), myLighting->GetSpecularExtra());
		}
	}

	if (!debugCam)
	{

		m_spriteBatch->Begin(SpriteSortMode::SpriteSortMode_Deferred, spriteBlendState, nullptr, spriteDepthState, spriteRasterState);
		 
		myDX->GetDeviceContext()->RSSetState(spriteRasterState);
		
		myUI->Render(m_spriteBatch, m_arialFont, m_comicSansFont);
		
		m_spriteBatch->End();
	}
	
	//Present the swap chain 
	myDX->PresentScreen();

	//modelList.clear();

	return true;
}

void Graphics::Update()
{
	
}

void Graphics::ExitLevel()
{

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

	if (Ground)
	{
		Ground->Shutdown();
		delete Ground;
		Ground = nullptr;
	}

	if (Skybox)
	{
		Skybox->Shutdown();
		delete Skybox;
		Skybox = nullptr;
	}


	if (myFrustum)
	{
		delete myFrustum;
		myFrustum = nullptr;
	}

}

void Graphics::RenderOnlyUI()
{
	XMMATRIX world, view, projection, frustumView;
	bool result, render;
	unsigned int renderCount = 0;

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
	myCamera->PassInViewMatrix(frustumView);

	myDX->PassProjectionMatrix(projection);

	//camPosition = { view.r[3].m128_f32[0],  view.r[3].m128_f32[1], view.r[3].m128_f32[2], view.r[3].m128_f32[3] };

	m_spriteBatch->Begin(SpriteSortMode::SpriteSortMode_Deferred, spriteBlendState, nullptr, spriteDepthState, spriteRasterState);

	myDX->GetDeviceContext()->RSSetState(spriteRasterState);

	myUI->Render(m_spriteBatch, m_arialFont, m_comicSansFont);

	m_spriteBatch->End();

	myDX->PresentScreen();
}



void Graphics::Update(InputManager *myInput, float delta, Player *myPlayer)
{
	if (!myUI->m_mainMenu && !myUI->m_pauseMenu)
	{
		if (debugCam)
		{
			myDebug->GetInput(myInput, delta);
		}
		else
		{
			myCamera->GetInput(myInput, delta, playerWorld, myPlayer);
		}

		if (myInput->GetKeyState((int)'O'))
		{
			if (timeGetTime() >= timeBetween + 300)
			{
				debugCam = !debugCam;
				timeBetween = timeGetTime();
			}
		}
	}

	//system("CLS");
}