#include "ActorBase.h"

ActorBase::ActorBase(const VECTOR& pos)
	:
	resMng_(ResourceManager::GetInstance()),
	transform_(Transform()),
	speed_(0.0f),
	hp_(0.0f),
	actorType_(ActorType::NONE),
	isAlive_(false),
	isActive_(false)
{
}

void ActorBase::Init(const VECTOR& pos)
{

#pragma region オブジェクトの情報

	SetPos(pos);
	speed_ = 0.0f;
	hp_ = 0;
	actorType_ = ActorType::NONE;
	isAlive_ = true;
	isActive_ = false;

#pragma endregion

}

void ActorBase::Create(const VECTOR& pos)
{
}

void ActorBase::Update(const float deltaTime)
{
}

void ActorBase::Draw()
{
	MV1DrawModel(transform_.modelId);
}
