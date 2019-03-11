#include "stdafx.h"
#include "graphicsclass.h"

Graphics::Graphics()
{
	myDX = 0;
}

Graphics::Graphics(const Graphics &other)
{


}

Graphics::~Graphics()
{

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
}

//Called each frame 
bool Graphics::Render()
{
	//Clear the screen 
	myDX->ClearScreen(0.0f, 1.0f, 0.0f, 1.0f);

	//Present the swap chain 
	myDX->PresentScreen();

	return true;
}