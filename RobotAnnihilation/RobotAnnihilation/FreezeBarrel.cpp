#include "FreezeBarrel.h"

FreezeBarrel::FreezeBarrel()
{
	readyToDestroy = false;
}

bool FreezeBarrel::Initialize(ID3D11Device * myDevice, const char * fileName, float3 position)
{
	GetModelComponent()->Initialize(fileName, myDevice);
	if (!GetModelComponent())
	{
		return false;
	}

	GetPhysicsComponent()->SetPosition(position); GetPhysicsComponent()->SetPosition(position);

	AddCollider({ position.x, position.y + 1.0f, position.z }, 2.3f);

	return true;
}

void FreezeBarrel::Destroy(Player * myPlayer, std::vector<Enemy*>& myEnemies, HWND window)
{
	if (DitanceFloat3(GetPhysicsComponent()->GetPosition(), myPlayer->GetPhysicsComponent()->GetPosition()) <= 10.0f)
	{
		myPlayer->SetFrozen();
	}

	for (unsigned int i = 0; i < myEnemies.size(); i++)
	{
		if (DitanceFloat3(GetPhysicsComponent()->GetPosition(), myEnemies[i]->GetPhysicsComponent()->GetPosition()) <= 10.0f)
		{
			myEnemies[i]->SetFrozen();
		}
	}

	readyToDestroy = true;
}


