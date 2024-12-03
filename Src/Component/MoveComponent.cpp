#include "MoveComponent.h"
#include "../Object/ActorBase.h"
#include "../Object/Player.h"

MoveComponent::MoveComponent(std::shared_ptr<ActorBase> actor)
{
	actor_ = actor;
}

void MoveComponent::Move()
{

	// ³‹K‰»
	const VECTOR normDir = VNorm(actor_->GetMoveDir());

	// ˆÚ“®—Ê
	const VECTOR movePow = VScale(normDir, actor_->GetSpeed());

	// ˆÚ“®Œã‚ÌÀ•W‚ðì‚é
	actor_->SetPos(VAdd(actor_->GetTransform()->pos, movePow));

}

void MoveComponent::Lerp()
{

	// ³‹K‰»
	const VECTOR normDir = VNorm(actor_->GetMoveDir());

	// ˆÚ“®ŒãÀ•W
	const VECTOR movedPos = VAdd(actor_->GetTransform()->pos,VScale(normDir, actor_->GetSpeed()));

	// ­‚µ‘O‚É‚ä‚Á‚­‚èˆÚ“®
	actor_->SetPos(Utility::Lerp(actor_->GetTransform()->pos, movedPos, 0.1f));

}
