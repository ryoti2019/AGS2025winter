#include "../Utility/Utility.h"
#include "Player.h"

Player::Player(const VECTOR& pos) : ActorBase(pos)
{
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	SetPos(pos);
	transform_.scl = { 1.0f,1.0f,1.0f };
	transform_.quaRot = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(0.0f),Utility::Deg2RadF(0.0f) });
	transform_.quaRotLocal = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(0.0f),Utility::Deg2RadF(0.0f) });
	transform_.Update();
}

void Player::Init(const VECTOR& pos)
{
}

void Player::Update(const float deltaTime)
{
}