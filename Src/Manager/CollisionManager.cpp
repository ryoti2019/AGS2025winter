#include <map>
#include "CollisionManager.h"

void CollisionManager::Init()
{
}

void CollisionManager::Update(const float deltaTime)
{

	// �v���C���[�ƓG�̍U���̓����蔻����`�F�b�N
	CheckAttackCollision(deltaTime);

	// �v���C���[�ƓG�̃X�e�[�W�̓����蔻����`�F�b�N
	CheckStageCollision();

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

void CollisionManager::CheckAttackCollision(const float deltaTime)
{

	// �Փ˂��Ă��邩���肷��
	for (const collisionChannnelInfo& info : attackCollisionChannelList_)
	{

		// �U�����Ă��鑤
		const auto& attackers = collisionActorData_.find(info.type1);

		// �U�����󂯂鑤
		const auto& targets = collisionActorData_.find(info.type2);

		// ���g�������Ă��邩�m�F
		if (attackers == collisionActorData_.end())continue;
		if (targets == collisionActorData_.end())continue;

		for (const std::shared_ptr<ActorBase>& attacker : attackers->second)
		{
			for (const std::shared_ptr<ActorBase>& target : targets->second)
			{

				// �|�C���^�������Ă���m�F
				if (!attacker)return;
				if (!target)return;

				// �U����Ԃɓ����Ă��Ȃ������瓖���蔻���ʂ�Ȃ�
				if (!attacker->GetAttackState())continue;

				// ���g�����邩�m�F
				auto hitData = invincibleData_.find(target);

				// ��������Ă��Ȃ��ꍇ�́A�V����vector�z��̔�����肻�̒��ɗv�f�����Ă���
				if (hitData == invincibleData_.end())
				{

					// �����������̂̃f�[�^�����
					std::map<int, float> data;
					for (int i = ; i <= attacker->GetToatlAttackTypes(); i++)
					{
						data.emplace(i, 0.0f);
					}

					// �����������̂��i�[
					invincibleData_.emplace(target, data);

				}

				// ���g��������Ώ������Ȃ�
				if (hitData == invincibleData_.end())return;

				// ���U�����Ă��鑤�̃A�j���[�V�������v�f�ɂ��邩�m�F����
				auto a = hitData->second.find(attacker->GetState());

				// ���g��������Ώ������Ȃ�
				if (a == hitData->second.end())return;

				// ���G���Ԃ����Z���Ă���
				a->second -= deltaTime;

				// ���̃A�j���[�V�������̖��G���Ԃ������Ă��Ȃ���Ώ������Ȃ�
				if (a->second > 0.0f)return;

				// �U�������łɓ������Ă����瓖���蔻���ʂ�Ȃ�
				//if (attacker->GetIsAttackHit())continue;

				// �U�����󂯂Ă����Ԃ������瓖���蔻���ʂ�Ȃ�
				//if (target->GetHitState())continue;

				// �E��̔���
				if (HitCheck_Capsule_Capsule(attacker->GetCollisionData().rightHandCapsuleUpPos, attacker->GetCollisionData().rightHandCapsuleDownPos,
					attacker->GetCollisionData().handAndFootCollisionRadius,
					target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius)
					&& attacker->GetCollisionData().isRightHandAttack)
				{

					// �U���������Ɠ�����Ȃ��悤�ɂ���
					attacker->SetIsAttackHit(true);

					// �U����������������
					target->AttackHit(attacker->GetDamage(), attacker->GetState());

					// ����̍��W��ݒ�
					target->SetTargetPos(attacker->GetPos());

					// �����������̂̃f�[�^�����
					std::map<int,float> data;
					data.emplace(attacker->GetState(), 1.0f);
					
					// �����������̂��i�[
					invincibleData_.emplace(target, data);

				}
				// ����̔���
				if (HitCheck_Capsule_Capsule(attacker->GetCollisionData().leftHandCapsuleUpPos, attacker->GetCollisionData().leftHandCapsuleDownPos,
					attacker->GetCollisionData().handAndFootCollisionRadius,
					target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius)
					&& attacker->GetCollisionData().isLeftHandAttack)
				{

					// �U���������Ɠ�����Ȃ��悤�ɂ���
					attacker->SetIsAttackHit(true);

					// �U����������������
					target->AttackHit(attacker->GetDamage(), attacker->GetState());

					// ����̍��W��ݒ�
					target->SetTargetPos(attacker->GetPos());

					// �����������̂̃f�[�^�����
					std::map<int, float> data;
					data.emplace(attacker->GetState(), 1.0f);

					// �����������̂��i�[
					invincibleData_.emplace(target, data);

				}
				// �E���̔���
				if (HitCheck_Capsule_Capsule(attacker->GetCollisionData().rightFootCapsuleUpPos, attacker->GetCollisionData().rightFootCapsuleDownPos,
					attacker->GetCollisionData().handAndFootCollisionRadius,
					target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius)
					&& attacker->GetCollisionData().isRightFootAttack)
				{

					// �U���������Ɠ�����Ȃ��悤�ɂ���
					attacker->SetIsAttackHit(true);

					// �U����������������
					target->AttackHit(attacker->GetDamage(), attacker->GetState());

					// ����̍��W��ݒ�
					target->SetTargetPos(attacker->GetPos());

					// �����������̂̃f�[�^�����
					std::map<int, float> data;
					data.emplace(attacker->GetState(), 1.0f);

					// �����������̂��i�[
					invincibleData_.emplace(target, data);

				}
				// �����̔���
				if (HitCheck_Capsule_Capsule(attacker->GetCollisionData().leftFootCapsuleUpPos, attacker->GetCollisionData().leftFootCapsuleDownPos,
					attacker->GetCollisionData().handAndFootCollisionRadius,
					target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius)
					&& attacker->GetCollisionData().isLeftFootAttack)
				{

					// �U���������Ɠ�����Ȃ��悤�ɂ���
					attacker->SetIsAttackHit(true);

					// �U����������������
					target->AttackHit(attacker->GetDamage(), attacker->GetState());

					// ����̍��W��ݒ�
					target->SetTargetPos(attacker->GetPos());

					// �����������̂̃f�[�^�����
					std::map<int, float> data;
					data.emplace(attacker->GetState(), 1.0f);

					// �����������̂��i�[
					invincibleData_.emplace(target, data);

				}
			}
		}
	}

}

void CollisionManager::CheckStageCollision()
{

	// �Փ˂��Ă��邩���肷��
	for (const collisionChannnelInfo& info : stageCollisionChannelList_)
	{

		// �X�e�[�W�Ɠ������Ă��邩���肵��������
		const auto& targets = collisionActorData_.find(info.type1);

		// �X�e�[�W
		const auto& stages = collisionActorData_.find(info.type2);

		// ���g�������Ă��邩�m�F
		if (targets == collisionActorData_.end())continue;
		if (stages == collisionActorData_.end())continue;

		for (const std::shared_ptr<ActorBase>& target : targets->second)
		{
			for (const std::shared_ptr<ActorBase>& stage : stages->second)
			{

				// �|�C���^�������Ă���m�F
				if (!target)return;
				if (!stage)return;

				// �J�v�Z���Ƃ̏Փ˔���
				auto hits = MV1CollCheck_Capsule(
					stage->GetTransform().modelId, -1,
					target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius);

				// �Փ˂��������̃|���S���ƏՓˉ������܂ŁA
				// �v���C���[�̈ʒu���ړ�������
				for (int i = 0; i < hits.HitNum; i++)
				{

					auto hit = hits.Dim[i];

					// �n�ʂƈقȂ�A�Փˉ���ʒu���s���Ȃ��߁A���x���ړ�������
					// ���̎��A�ړ�����������́A�ړ��O���W�Ɍ����������ł�������A
					// �Փ˂����|���S���̖@�������������肷��
					for (int tryCnt = 0; tryCnt < 10; tryCnt++)
					{

						// �ēx�A���f���S�̂ƏՓˌ��o����ɂ́A���������߂���̂ŁA
						// �ŏ��̏Փ˔���Ō��o�����Փ˃|���S��1���ƏՓ˔�������
						int pHit = HitCheck_Capsule_Triangle(
							target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius,
							hit.Position[0], hit.Position[1], hit.Position[2]);

						if (pHit)
						{

							// �������Ă��邩���肵����Actor�̍��W
							VECTOR pos = target->GetTransform().pos;

							// �@���̕����ɂ�����Ƃ����ړ�������
							pos = VAdd(pos, VScale(hit.Normal, 30.0f));

							// �J�v�Z�����ꏏ�Ɉړ�������
							target->SetPos(pos);
							continue;

						}
						break;
					}
				}

				// ���o�����n�ʃ|���S�����̌�n��
				MV1CollResultPolyDimTerminate(hits);
				
				// �n�ʂƂ̏Փ�
				auto hit = MV1CollCheck_Line(
					stage->GetTransform().modelId, -1,
					VAdd(target->GetCollisionData().bodyCapsuleUpPos, VECTOR(0.0f,target->GetCollisionData().bodyCollisionRadius,0.0f)),
					VAdd(target->GetCollisionData().bodyCapsuleDownPos, VECTOR(0.0f, -target->GetCollisionData().bodyCollisionRadius, 0.0f)));
				if (hit.HitFlag > 0 && VDot({0.0f,-1.0f,0.0f}, target->GetVelocity()) > 0.9f)
				{

					// �Փ˒n�_����A������Ɉړ�
					target->SetPos(hit.HitPosition);

					// �W�����v���Z�b�g
					target->SetVelocity(Utility::VECTOR_ZERO);

				}
			}
		}
	}

}
