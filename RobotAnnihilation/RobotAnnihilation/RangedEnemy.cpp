#include "RangedEnemy.h"

RangedEnemy::RangedEnemy()
{
	Enemy::Enemy();
	timeBetweenAttacks = 5.0f;
}

bool RangedEnemy::Initialize(ID3D11Device * myDevice, const char * fileName, float3 position)
{
	GetModelComponent()->Initialize(fileName, myDevice);
	if (!GetModelComponent())
	{
		return false;
	}

	GetPhysicsComponent()->SetPosition(position);
	GetPhysicsComponent()->SetMass(5.0f);
	GetPhysicsComponent()->SetDamping(0.99f);

	AddCollider(GetPhysicsComponent()->GetPosition(), 0.8f);
	AddCollider({ GetPhysicsComponent()->GetPosition().x, GetPhysicsComponent()->GetPosition().y + 1.0f, GetPhysicsComponent()->GetPosition().z }, 0.5f);
	//srand((unsigned int)time(NULL));

	velocity = .6f;

	attacking = false;

	startTime = 0.0f;

	return true;
}

void RangedEnemy::Update(float delta, Player * myPlayer, std::vector<Bullet*> &bullets, ID3D11Device *myDevice, HWND window)
{
	if (PlayOnce)
	{
		if (!Dead)
		{
			if (currentAnimation->GetFrameTime() + delta > currentAnimation->GetAnimationClip().duration)
			{
				currentAnimation->SetFrameTime(delta);
				currentAnimation = PrevAnimation;
				PlayOnce = false;
			}
		}
	}
	GameObject::Update(delta);
	GetCollider(0)->center = { GetPhysicsComponent()->GetPosition().x, GetPhysicsComponent()->GetPosition().y + 1.0f, GetPhysicsComponent()->GetPosition().z };
	GetCollider(1)->center = { GetPhysicsComponent()->GetPosition().x, GetPhysicsComponent()->GetPosition().y + 2.5f, GetPhysicsComponent()->GetPosition().z };

	GetPhysicsComponent()->SetPosition({ GetPhysicsComponent()->GetPosition().x, 1.0f, GetPhysicsComponent()->GetPosition().z });

	startTime += delta;

	if (startTime > 5.0f)
	{
		Attack(myPlayer, bullets, myDevice, window);
	}

	if (DitanceFloat3(GetPhysicsComponent()->GetPosition(), myPlayer->GetPhysicsComponent()->GetPosition()) <= 15.0f)
	{
		GetPhysicsComponent()->SetPosition(float3{ (((float)rand() - (float)rand()) / RAND_MAX) * 60.0f, 2.0f, ((((float)rand() - (float)rand()) / RAND_MAX) * 60.0f) + 5.0f });
	}

	if (timeGetTime() >= HurtTime + 200)
	{
		ImHurt = false;
		HurtTime = 0.8f;
	}

}

void RangedEnemy::Attack(Player * myPlayer, std::vector<Bullet*> &bullets, ID3D11Device *myDevice, HWND window)
{
	if (timeGetTime() >= timeBetweenAttacks + 3000)
	{
		PlayOnce = true;
		PrevAnimation = currentAnimation;
		currentAnimation = objectAnimations[1];
		currentAnimation->Update(0);
		float3 forward = myPlayer->GetPhysicsComponent()->GetPosition() - GetPhysicsComponent()->GetPosition();
		//forward.y = forward.y + 2.5f;
		if (rand() % 3 == 0)
		{
			float3 bulletV = forward * 3.0f;
			float time = DitanceFloat3(GetPhysicsComponent()->GetPosition(), myPlayer->GetPhysicsComponent()->GetPosition()) / Magnitude(bulletV);
			float Offset = time * Magnitude(myPlayer->GetPhysicsComponent()->GetVelocity());
			forward = { forward.x + Offset, forward.y, forward.z + Offset };
		}
		bullets.push_back(new Bullet(1.0f));
		float3 playerPos = { GetPhysicsComponent()->GetPosition().x, GetPhysicsComponent()->GetPosition().y + 3.0f, GetPhysicsComponent()->GetPosition().z };
		bullets[bullets.size() - 1]->Initialize(myDevice, "Assets/Bullet.mesh", forward, playerPos, "Enemy", 1.5f);
		AddSound(new Sound((char*)"Assets/EnemyShoot.wav", 0));
		GetSounds()[GetSounds().size() - 1]->Initialize(window);
		GetSounds()[GetSounds().size() - 1]->PlayWaveFile();
		timeBetweenAttacks = timeGetTime();
	}
	
}

void RangedEnemy::Death()
{
	Dead = true;
	PlayOnce = true;
	PrevAnimation = currentAnimation;
	currentAnimation = objectAnimations[2];
}
