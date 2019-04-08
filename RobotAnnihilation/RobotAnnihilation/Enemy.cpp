#include "Enemy.h"

Enemy::Enemy()
{
	GameObject::GameObject();
	readyToDestroy = false;
}

bool Enemy::Destroy()
{
	return readyToDestroy;
}

void Enemy::SetDestroy()
{
	readyToDestroy = true;
}
