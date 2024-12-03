#include "MoveComponent.h"
#include "../Object/ActorBase.h"
#include "../Object/Player.h"

MoveComponent::MoveComponent(std::shared_ptr<ActorBase> actor)
{
	actor_ = actor;
}

void MoveComponent::Move()
{

	// 正規化
	const VECTOR normDir = VNorm(actor_->GetMoveDir());

	// 移動量
	const VECTOR movePow = VScale(normDir, actor_->GetSpeed());

	// 移動後の座標を作る
	actor_->SetPos(VAdd(actor_->GetTransform()->pos, movePow));

}

void MoveComponent::Lerp()
{

	// 正規化
	const VECTOR normDir = VNorm(actor_->GetMoveDir());

	// 移動後座標
	const VECTOR movedPos = VAdd(actor_->GetTransform()->pos,VScale(normDir, actor_->GetSpeed()));

	// 少し前にゆっくり移動
	actor_->SetPos(Utility::Lerp(actor_->GetTransform()->pos, movedPos, 0.1f));

}

void MoveComponent::HitMove()
{

	// 方向
	const VECTOR dir = actor_->GetMoveDir();

	// スピード
	const float speed = actor_->GetSpeed();

	// 移動量
	const VECTOR movePow = VScale(dir, speed);

	// 移動後の座標を作る
	actor_->SetPos(VAdd(actor_->GetTransform()->pos, movePow));

}

