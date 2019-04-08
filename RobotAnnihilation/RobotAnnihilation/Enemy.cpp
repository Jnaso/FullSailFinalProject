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

void Enemy::SubHealth(float newHealth, DamageType dmg, HWND window)
{
	if (dmg == DamageType::Melee)
	{
		if (timeBetweenDamage <= 0)
		{
			this->AddSound(new Sound((char*)"Assets/HitSound.wav", -1000));
			this->GetSounds()[this->GetSounds().size() - 1]->Initialize(window);
			this->GetSounds()[this->GetSounds().size() - 1]->PlayWaveFile();
			health -= newHealth;
			timeBetweenDamage = 0.5f;
		}
	}
	else if (dmg == DamageType::Gun)
	{
		this->AddSound(new Sound((char*)"Assets/HitSound.wav", -1000));
		this->GetSounds()[this->GetSounds().size() - 1]->Initialize(window);
		this->GetSounds()[this->GetSounds().size() - 1]->PlayWaveFile();
		health -= newHealth;
	}
}
