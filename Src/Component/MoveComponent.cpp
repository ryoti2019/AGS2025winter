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

	// ³‹K‰»
	const VECTOR normDir = VNorm(moveDir);

	// ˆÚ“®—Ê
	const VECTOR movePow = VScale(normDir, speed);

	// ˆÚ“®Œã‚ÌÀ•W‚ğì‚é
	const VECTOR movePos = VAdd(actor.GetTransform()->pos, movePow);

	// À•W‚ğİ’è
	actor.SetPos(movePos);

}