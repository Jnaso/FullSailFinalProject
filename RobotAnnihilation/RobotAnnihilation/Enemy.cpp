#include "Enemy.h"

Enemy::Enemy()
{
	GameObject::GameObject();
	readyToDestroy = false;
	ImHurt = false;
	isFrozen = false;
	freezeTime = 0.0f;
}

bool Enemy::Destroy()
{
	return readyToDestroy;
}

void Enemy::SetDestroy()
{
	readyToDestroy = true;
}

void Enemy::Update(float delta, Player * myPlayer, std::vector<Bullet*>& bullets, ID3D11Device * myDevice, HWND window)
{
	if (timeBetweenDamage > 0)
	{
		timeBetweenDamage -= delta;
	}
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
			timeBetweenDamage = 0.25f;
		}
	}
	else if (dmg == DamageType::Gun)
	{
		//this->AddSound(new Sound((char*)"Assets/HitSound.wav", -1000));
		//this->GetSounds()[this->GetSounds().size() - 1]->Initialize(window);
		//this->GetSounds()[this->GetSounds().size() - 1]->PlayWaveFile();
		health -= newHealth;
	}
}

void Enemy::SetHurt()
{
	if (!ImHurt)
	{
		HurtTime = timeGetTime();
		ImHurt = true;
	}
}

bool Enemy::GetHurt()
{
	return ImHurt;
}
