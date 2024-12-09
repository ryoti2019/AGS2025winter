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

	// ƒ‚ƒfƒ‹•`‰æ
	MV1DrawModel(actor_->GetTransform()->modelId);

	// ƒfƒoƒbƒO•`‰æ
	DrawDebug();

}

void DrawComponent::DrawDebug()
{

	// ‘Ì‚Ì’†SÀ•W‚Ì•`‰æ
	//DrawSphere3D(actor_->GetCollisionData().bodyPos, actor_->GetCollisionData().bodyCollisionRadius, 10, 0xff0000, 0xff0000, false);

	// ‰EŽè‚Ì“–‚½‚è”»’è‚Ì•`‰æ
	//DrawCapsule3D(actor_->GetCollisionData().rightHandCapsuleUpPos, actor_->GetCollisionData().rightHandCapsuleDownPos, actor_->GetCollisionData().handAndFootCollisionRadius, 10, 0xff0000, 0xff0000, false);

	// ¶Žè‚Ì“–‚½‚è”»’è‚Ì•`‰æ
	//DrawCapsule3D(actor_->GetCollisionData().leftHandCapsuleUpPos, actor_->GetCollisionData().leftHandCapsuleDownPos, actor_->GetCollisionData().handAndFootCollisionRadius, 10, 0xff0000, 0xff0000, false);

	// ‰E‘«‚Ì“–‚½‚è”»’è‚Ì•`‰æ
	//DrawCapsule3D(actor_->GetCollisionData().rightFootCapsuleUpPos, actor_->GetCollisionData().rightFootCapsuleDownPos, actor_->GetCollisionData().handAndFootCollisionRadius, 10, 0xff0000, 0xff0000, false);

	// ¶‘«‚Ì“–‚½‚è”»’è‚Ì•`‰æ
	//DrawCapsule3D(actor_->GetCollisionData().leftFootCapsuleUpPos, actor_->GetCollisionData().leftFootCapsuleDownPos, actor_->GetCollisionData().handAndFootCollisionRadius, 10, 0xff0000, 0xff0000, false);

	// ‘Ì‚Ì“–‚½‚è”»’è‚Ì•`‰æ
	//DrawCapsule3D(actor_->GetCollisionData().bodyCapsuleUpPos, actor_->GetCollisionData().bodyCapsuleDownPos, actor_->GetCollisionData().bodyCollisionRadius, 10, 0xff0000, 0xff0000, false);

	// •KŽE‹Z‚Ì“–‚½‚è”»’è‚Ì•`‰æ
	DrawSphere3D(actor_->GetCollisionData().projectilePos, actor_->GetCollisionData().projectileCollisionRadius, 10, 0xff0000, 0xff0000, false);

	// ’n–Ê‚Æ‚Ì“–‚½‚è”»’è‚ÌŽž‚Ìü‚Ì•`‰æ
	//DrawLine3D(
	//	VAdd(actor_->GetCollisionData().bodyCapsuleUpPos,VGet(0.0f, actor_->GetCollisionData().bodyCollisionRadius,0.0f)),
	//	VAdd(actor_->GetCollisionData().bodyCapsuleDownPos, VGet(0.0f, -actor_->GetCollisionData().bodyCollisionRadius, 0.0f)), 0x00ff00);

	// ƒXƒe[ƒW‚Ì“–‚½‚è”»’èƒ‚ƒfƒ‹‚Ì•`‰æ
	//MV1DrawModel(actor_->GetCollisionModelId());

}
