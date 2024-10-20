#include "ActorBase.h"

ActorBase::ActorBase(const VECTOR& pos)
	:
	resMng_(ResourceManager::GetInstance()),
	transform_(Transform()),
	ROTATION_POW(0.1f),
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

	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	SetPos(pos);
	transform_.scl = { 1.0f,1.0f,1.0f };
	transform_.quaRot = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(0.0f),Utility::Deg2RadF(0.0f) });
	transform_.quaRotLocal = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(180.0f),Utility::Deg2RadF(0.0f) });
	transform_.Update();

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

void ActorBase::LazyRotation(float goalRot)
{
	auto goal = Quaternion::Euler(0.0f, goalRot, 0.0f);
	transform_.quaRot = Quaternion::Slerp(transform_.quaRot, goal, ROTATION_POW);
}

void ActorBase::Draw()
{
	MV1DrawModel(transform_.modelId);
}