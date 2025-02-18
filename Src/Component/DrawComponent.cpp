#include <DxLib.h>
#include "DrawComponent.h"
#include "../Object/Common/Transform.h"
#include "../Object/ActorBase.h"

DrawComponent::DrawComponent(std::shared_ptr<ActorBase> actor)
{
	actor_ = actor;
}

void DrawComponent::Update()
{

	// モデル描画
	MV1DrawModel(actor_.lock()->GetTransform()->modelId);

	// デバッグ描画
	DrawDebug();

}

void DrawComponent::DrawDebug()
{

	// 体の中心座標の描画
	//DrawSphere3D(actor_->GetCollisionData().bodyPos, actor_->GetCollisionData().bodyCollisionRadius, 10, 0xff0000, 0xff0000, false);

	// 右手の当たり判定の描画
	//DrawCapsule3D(actor_->GetCollisionData().rightHandCapsuleUpPos, actor_->GetCollisionData().rightHandCapsuleDownPos, actor_->GetCollisionData().handAndFootCollisionRadius, 10, 0xff0000, 0xff0000, false);

	// 左手の当たり判定の描画
	//DrawCapsule3D(actor_->GetCollisionData().leftHandCapsuleUpPos, actor_->GetCollisionData().leftHandCapsuleDownPos, actor_->GetCollisionData().handAndFootCollisionRadius, 10, 0xff0000, 0xff0000, false);

	// 右足の当たり判定の描画
	//DrawCapsule3D(actor_->GetCollisionData().rightFootCapsuleUpPos, actor_->GetCollisionData().rightFootCapsuleDownPos, actor_->GetCollisionData().handAndFootCollisionRadius, 10, 0xff0000, 0xff0000, false);

	// 左足の当たり判定の描画
	//DrawCapsule3D(actor_->GetCollisionData().leftFootCapsuleUpPos, actor_->GetCollisionData().leftFootCapsuleDownPos, actor_->GetCollisionData().handAndFootCollisionRadius, 10, 0xff0000, 0xff0000, false);

	// 体の当たり判定の描画
	//DrawCapsule3D(actor_.lock()->GetCollisionData().bodyCapsuleUpPos, actor_.lock()->GetCollisionData().bodyCapsuleDownPos, actor_.lock()->GetCollisionData().bodyCollisionRadius, 10, 0xff0000, 0xff0000, false);

	// 必殺技の当たり判定の描画
	//DrawSphere3D(actor_.lock()->GetCollisionData().projectilePos, actor_.lock()->GetCollisionData().projectileCollisionRadius, 10, 0xff0000, 0xff0000, false);

	// 地面との当たり判定の時の線の描画
	//DrawLine3D(
	//	VAdd(actor_->GetCollisionData().bodyCapsuleUpPos,VGet(0.0f, actor_->GetCollisionData().bodyCollisionRadius,0.0f)),
	//	VAdd(actor_->GetCollisionData().bodyCapsuleDownPos, VGet(0.0f, -actor_->GetCollisionData().bodyCollisionRadius, 0.0f)), 0x00ff00);

	// ステージの当たり判定モデルの描画
	//MV1DrawModel(actor_->GetCollisionModelId());

}
