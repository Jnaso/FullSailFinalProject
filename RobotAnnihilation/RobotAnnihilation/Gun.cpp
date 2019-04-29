#include "Gun.h"

bool Gun::Initialize(const char * filePath, ID3D11Device * device)
{
	CurrentAmmo = MaxClipAmmo;
	return gunModel->Initialize(filePath, device);
}

void Gun::Render(ID3D11DeviceContext * context)
{
	gunModel->Render(context);
}

void Gun::Shutdown()
{
	if(gunModel) gunModel->Shutdown();
}

void Gun::Update(float delta)
{
	if (Reloading)
	{
		TimeLeftReloading -= delta;
		if (TimeLeftReloading<= 0)
		{
			Reloading = false;
			CurrentAmmo = MaxClipAmmo;
			/*if (ReserveAmmo - MaxClipAmmo > 0)
			{
				CurrentAmmo = MaxClipAmmo;
				ReserveAmmo -= MaxClipAmmo;
			}
			else if (ReserveAmmo < MaxClipAmmo)
			{
				CurrentAmmo = ReserveAmmo;
				ReserveAmmo -= ReserveAmmo;
			}*/
		}
	}
}

void Gun::Reload()
{
	Reloading = true;
	TimeLeftReloading = ReloadTime;
}

Gun::Gun()
{
	FireRate = 0.0f;
	DamageAmount = 0.0f;
	ShootPos = float3{0, 0, 0};
	gunModel = nullptr;
}


Gun::~Gun()
{
	delete gunModel;
}
