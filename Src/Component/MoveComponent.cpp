#include "MoveComponent.h"
#include "../Object/ActorBase.h"
#include "../Object/Player.h"

MoveComponent::MoveComponent(std::weak_ptr<ActorBase> actor)
{
	actor_ = actor;
}

void MoveComponent::Move()
{

	// 正規化
	const VECTOR normDir = VNorm(actor_.lock()->GetMoveDir());

	// 移動量
	const VECTOR movePow = VScale(normDir, actor_.lock()->GetSpeed());

	// 移動後の座標を作る
	actor_.lock()->SetPos(VAdd(actor_.lock()->GetTransform()->pos, movePow));

}

void MoveComponent::Lerp()
{

	// 正規化
	const VECTOR normDir = VNorm(actor_.lock()->GetMoveDir());

	// 移動後座標
	const VECTOR movedPos = VAdd(actor_.lock()->GetTransform()->pos,VScale(normDir, actor_.lock()->GetSpeed()));

	// 少し前にゆっくり移動
	actor_.lock()->SetPos(Utility::Lerp(actor_.lock()->GetTransform()->pos, movedPos, MOVE_SMOOTHNESS));

}

void MoveComponent::HitMove()
{

	// 方向
	const VECTOR dir = actor_.lock()->GetMoveDir();

	// スピード
	const float speed = actor_.lock()->GetSpeed();

	// 移動量
	const VECTOR movePow = VScale(dir, speed);

	// 移動後の座標を作る
	actor_.lock()->SetPos(VAdd(actor_.lock()->GetTransform()->pos, movePow));

}

