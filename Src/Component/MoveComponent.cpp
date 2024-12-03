#include "MoveComponent.h"
#include "../Object/ActorBase.h"
#include "../Object/Player.h"

MoveComponent::MoveComponent(std::shared_ptr<ActorBase> actor)
{
	actor_ = actor;
}

void MoveComponent::Move()
{

	// ���K��
	const VECTOR normDir = VNorm(actor_->GetMoveDir());

	// �ړ���
	const VECTOR movePow = VScale(normDir, actor_->GetSpeed());

	// �ړ���̍��W�����
	actor_->SetPos(VAdd(actor_->GetTransform()->pos, movePow));

}

void MoveComponent::Lerp()
{

	// ���K��
	const VECTOR normDir = VNorm(actor_->GetMoveDir());

	// �ړ�����W
	const VECTOR movedPos = VAdd(actor_->GetTransform()->pos,VScale(normDir, actor_->GetSpeed()));

	// �����O�ɂ������ړ�
	actor_->SetPos(Utility::Lerp(actor_->GetTransform()->pos, movedPos, 0.1f));

}
