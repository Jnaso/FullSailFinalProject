#include "GameManager.h"



GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}

void GameManager::Update(float delta)
{
	for (unsigned int i = 0; i < GameObjects.size(); i++)
	{
		// Uncomment when GameObjects have been implemented
		//GameObjects[i].Update(delta);
	}
}