#include "ActorBase.h"

ActorBase::ActorBase(const VECTOR& pos)
	:
	pos_(pos),
	movedPos_(pos),
	speed_(0.0f),
	movePow_(),
	hp_(),
	isHit_(),
	actorType_(),
	isAlive_(),
	isActive_()
{
}

void ActorBase::Init(const VECTOR& pos)
{

#pragma region オブジェクトの情報

	SetPos(pos);
	movedPos_ = { 0.0f,0.0f };
	speed_ = 10.0f;
	movePow_ = 0.0f;
	hp_ = 0;
	isHit_ = false;
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
}
