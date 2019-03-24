#include "Player.h"



Player::Player()
{
}

Player::Player(const char * filePath, ID3D11Device * device)
{
	ReadMeshFile(filePath, device);
	ObjectPhysics = new PhysicsComponent();
}


Player::~Player()
{
}

Animation* Player::GetRunAnimation()
{
	return RunAnimation;
}

void Player::SetRunAnimation(Animation* anim)
{
	RunAnimation = anim;
}

Animation* Player::GetIdleAnimation()
{
	return IdleAnimation;
}

void Player::SetIdleAnimation(Animation* anim)
{
	IdleAnimation = anim;
}

Animation* Player::GetCurrentAnimation()
{
	return CurrentAnimation;
}

void Player::SetCurrentAnimation(Animation* anim)
{
	CurrentAnimation = anim;
}

void Player::Update(float delta)
{
	CurrentAnimation->Update(delta);
	ObjectPhysics->Update(delta);
}