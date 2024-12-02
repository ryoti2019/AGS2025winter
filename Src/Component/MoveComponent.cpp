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

	// ���K��
	const VECTOR normDir = VNorm(moveDir);

	// �ړ���
	const VECTOR movePow = VScale(normDir, speed);

	// �ړ���̍��W�����
	const VECTOR movePos = VAdd(actor.GetTransform()->pos, movePow);

	// ���W��ݒ�
	actor.SetPos(movePos);

}