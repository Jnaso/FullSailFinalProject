#include "InventoryManager.h"



InventoryManager::InventoryManager()
{
}


InventoryManager::~InventoryManager()
{
}

InventoryManager::KEYSTATE InventoryManager::GetKeyState(int keyCode)
{
	if (m_keys[keyCode])
	{
		return KEYSTATE::DOWN;
	}
	else
	{
		return KEYSTATE::UP;
	}
}

void InventoryManager::SetKeyState(int keyCode , bool isPress)
{
	m_keys[keyCode] = isPress;
}
