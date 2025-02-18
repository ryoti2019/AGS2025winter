#include "MoveComponent.h"
#include "../Object/ActorBase.h"
#include "../Object/Player.h"

MoveComponent::MoveComponent(std::weak_ptr<ActorBase> actor)
{
	actor_ = actor;
}

void MoveComponent::Move()
{

	// ���K��
	const VECTOR normDir = VNorm(actor_.lock()->GetMoveDir());

	// �ړ���
	const VECTOR movePow = VScale(normDir, actor_.lock()->GetSpeed());

	// �ړ���̍��W�����
	actor_.lock()->SetPos(VAdd(actor_.lock()->GetTransform()->pos, movePow));

}

void MoveComponent::Lerp()
{

	// ���K��
	const VECTOR normDir = VNorm(actor_.lock()->GetMoveDir());

	// �ړ�����W
	const VECTOR movedPos = VAdd(actor_.lock()->GetTransform()->pos,VScale(normDir, actor_.lock()->GetSpeed()));

	// �����O�ɂ������ړ�
	actor_.lock()->SetPos(Utility::Lerp(actor_.lock()->GetTransform()->pos, movedPos, MOVE_SMOOTHNESS));

}

void MoveComponent::HitMove()
{

	// ����
	const VECTOR dir = actor_.lock()->GetMoveDir();

	// �X�s�[�h
	const float speed = actor_.lock()->GetSpeed();

	// �ړ���
	const VECTOR movePow = VScale(dir, speed);

	// �ړ���̍��W�����
	actor_.lock()->SetPos(VAdd(actor_.lock()->GetTransform()->pos, movePow));

}

