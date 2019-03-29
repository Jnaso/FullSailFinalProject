#include "Gun.h"



bool Gun::Initialize(const char * filePath, ID3D11Device * device)
{
	return gunModel->Initialize(filePath, device);
}

void Gun::Render(ID3D11DeviceContext * context)
{
	gunModel->Render(context);
}

void Gun::Shutdown()
{
	if(gunModel) gunModel->Shutdown();
	delete gunModel;
}

void Gun::Update(float delta)
{

}

Gun::Gun()
{
}


Gun::~Gun()
{
}
