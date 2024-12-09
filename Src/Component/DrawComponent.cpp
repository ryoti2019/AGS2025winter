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

	// ���f���`��
	MV1DrawModel(actor_->GetTransform()->modelId);

	// �f�o�b�O�`��
	DrawDebug();

}

void DrawComponent::DrawDebug()
{

	// �̂̒��S���W�̕`��
	//DrawSphere3D(actor_->GetCollisionData().bodyPos, actor_->GetCollisionData().bodyCollisionRadius, 10, 0xff0000, 0xff0000, false);

	// �E��̓����蔻��̕`��
	//DrawCapsule3D(actor_->GetCollisionData().rightHandCapsuleUpPos, actor_->GetCollisionData().rightHandCapsuleDownPos, actor_->GetCollisionData().handAndFootCollisionRadius, 10, 0xff0000, 0xff0000, false);

	// ����̓����蔻��̕`��
	//DrawCapsule3D(actor_->GetCollisionData().leftHandCapsuleUpPos, actor_->GetCollisionData().leftHandCapsuleDownPos, actor_->GetCollisionData().handAndFootCollisionRadius, 10, 0xff0000, 0xff0000, false);

	// �E���̓����蔻��̕`��
	//DrawCapsule3D(actor_->GetCollisionData().rightFootCapsuleUpPos, actor_->GetCollisionData().rightFootCapsuleDownPos, actor_->GetCollisionData().handAndFootCollisionRadius, 10, 0xff0000, 0xff0000, false);

	// �����̓����蔻��̕`��
	//DrawCapsule3D(actor_->GetCollisionData().leftFootCapsuleUpPos, actor_->GetCollisionData().leftFootCapsuleDownPos, actor_->GetCollisionData().handAndFootCollisionRadius, 10, 0xff0000, 0xff0000, false);

	// �̂̓����蔻��̕`��
	//DrawCapsule3D(actor_->GetCollisionData().bodyCapsuleUpPos, actor_->GetCollisionData().bodyCapsuleDownPos, actor_->GetCollisionData().bodyCollisionRadius, 10, 0xff0000, 0xff0000, false);

	// �K�E�Z�̓����蔻��̕`��
	DrawSphere3D(actor_->GetCollisionData().projectilePos, actor_->GetCollisionData().projectileCollisionRadius, 10, 0xff0000, 0xff0000, false);

	// �n�ʂƂ̓����蔻��̎��̐��̕`��
	//DrawLine3D(
	//	VAdd(actor_->GetCollisionData().bodyCapsuleUpPos,VGet(0.0f, actor_->GetCollisionData().bodyCollisionRadius,0.0f)),
	//	VAdd(actor_->GetCollisionData().bodyCapsuleDownPos, VGet(0.0f, -actor_->GetCollisionData().bodyCollisionRadius, 0.0f)), 0x00ff00);

	// �X�e�[�W�̓����蔻�胂�f���̕`��
	//MV1DrawModel(actor_->GetCollisionModelId());

}
