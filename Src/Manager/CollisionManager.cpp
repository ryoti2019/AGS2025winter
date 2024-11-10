#include "CollisionManager.h"

void CollisionManager::Init()
{
}

void CollisionManager::Update()
{
	CollisionCheck();
}

void CollisionManager::Register(const std::shared_ptr<ActorBase>& actor)
{

	// collisionActorData_�̒��ɂ��łɓ����^����������Ă��邩�`�F�b�N����
	auto collisionElem = collisionActorData_.find(actor->GetActorType());

	// ��������Ă��Ȃ��ꍇ�́A�V����vector�z��̔�����肻�̒��ɗv�f�����Ă���
	if (collisionElem == collisionActorData_.end())
	{
		std::vector<std::shared_ptr<ActorBase>> data;
		data.emplace_back(actor);
		collisionActorData_.emplace(actor->GetActorType(), data);
	}
	// ��������Ă���ꍇ�͂��łɂ��锠�̒��ɗv�f�����Ă���
	else
	{
		collisionElem->second.emplace_back(actor);
	}

}

void CollisionManager::CollisionCheck()
{

	// �Փ˂��Ă��邩���肷��
	for (const collisionChannnelInfo& info : collisionChannelList_)
	{

		// �U�����Ă��鑤
		const auto& data1 = collisionActorData_.find(info.type1);

		// �U�����󂯂鑤
		const auto& data2 = collisionActorData_.find(info.type2);

		// ���g�������Ă��邩�m�F
		if (data1 == collisionActorData_.end())continue;
		if (data2 == collisionActorData_.end())continue;

		for (const std::shared_ptr<ActorBase>& actor1 : data1->second)
		{
			for (const std::shared_ptr<ActorBase>& actor2 : data2->second)
			{

				// �|�C���^�������Ă���m�F
				if (!actor1)return;
				if (!actor2)return;

				// �U����Ԃɓ����Ă��Ȃ������瓖���蔻���ʂ�Ȃ�
				if (!actor1->GetAttackState())continue;
				if (actor1->GetIsAttackHit())continue;

				// �U�����󂯂Ă����Ԃ������瓖���蔻���ʂ�Ȃ�
				//if (actor2->GetHitState())continue;

				// �E��̔���
				if (HitCheck_Capsule_Capsule(actor1->GetCollisionData().rightHandCapsuleUpPos, actor1->GetCollisionData().rightHandCapsuleDownPos,
					actor1->GetCollisionData().handAndFootCollisionRadius,
					actor2->GetCollisionData().bodyCapsuleUpPos, actor2->GetCollisionData().bodyCapsuleDownPos, actor2->GetCollisionData().bodyCollisionRadius)
					&& actor1->GetCollisionData().isRightHandAttack)
				{

					// �U���������Ɠ�����Ȃ��悤�ɂ���
					actor1->SetIsAttackHit(true);

					// �U����������������
					actor2->AttackHit();

					// ����̍��W��ݒ�
					actor2->SetTargetPos(actor1->GetPos());

				}
				// ����̔���
				if (HitCheck_Capsule_Capsule(actor1->GetCollisionData().leftHandCapsuleUpPos, actor1->GetCollisionData().leftHandCapsuleDownPos,
					actor1->GetCollisionData().handAndFootCollisionRadius,
					actor2->GetCollisionData().bodyCapsuleUpPos, actor2->GetCollisionData().bodyCapsuleDownPos, actor2->GetCollisionData().bodyCollisionRadius)
					&& actor1->GetCollisionData().isLeftHandAttack)
				{

					// �U���������Ɠ�����Ȃ��悤�ɂ���
					actor1->SetIsAttackHit(true);

					// �U����������������
					actor2->AttackHit();

					// ����̍��W��ݒ�
					actor2->SetTargetPos(actor1->GetPos());

				}
				// �E���̔���
				if (HitCheck_Capsule_Capsule(actor1->GetCollisionData().rightFootCapsuleUpPos, actor1->GetCollisionData().rightFootCapsuleDownPos,
					actor1->GetCollisionData().handAndFootCollisionRadius,
					actor2->GetCollisionData().bodyCapsuleUpPos, actor2->GetCollisionData().bodyCapsuleDownPos, actor2->GetCollisionData().bodyCollisionRadius)
					&& actor1->GetCollisionData().isRightFootAttack)
				{

					// �U���������Ɠ�����Ȃ��悤�ɂ���
					actor1->SetIsAttackHit(true);

					// �U����������������
					actor2->AttackHitFly();

					// ����̍��W��ݒ�
					actor2->SetTargetPos(actor1->GetPos());

				}
				// �����̔���
				if (HitCheck_Capsule_Capsule(actor1->GetCollisionData().leftFootCapsuleUpPos, actor1->GetCollisionData().leftFootCapsuleDownPos,
					actor1->GetCollisionData().handAndFootCollisionRadius,
					actor2->GetCollisionData().bodyCapsuleUpPos, actor2->GetCollisionData().bodyCapsuleDownPos, actor2->GetCollisionData().bodyCollisionRadius)
					&& actor1->GetCollisionData().isLeftFootAttack)
				{

					// �U���������Ɠ�����Ȃ��悤�ɂ���
					actor1->SetIsAttackHit(true);

					// �U����������������
					actor2->AttackHit();

					// ����̍��W��ݒ�
					actor2->SetTargetPos(actor1->GetPos());

				}
			}
		}
	}

}