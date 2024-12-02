#include "MoveComponent.h"
#include "../Object/ActorBase.h"

MoveComponent::MoveComponent()
{

}

MoveComponent::~MoveComponent()
{
}

void MoveComponent::Update(ActorBase& actor ,const VECTOR& moveDir, float speed)
{

	// 正規化
	const VECTOR normDir = VNorm(moveDir);

	// 移動量
	const VECTOR movePow = VScale(normDir, speed);

	// 移動後の座標を作る
	const VECTOR movePos = VAdd(actor.GetTransform()->pos, movePow);

	// 座標を設定
	actor.SetPos(movePos);

}