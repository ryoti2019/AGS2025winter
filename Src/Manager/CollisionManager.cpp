#define NOMINMAX
#include <map>
#include <algorithm>
#include <cmath>
#include <vector>
#include <DxLib.h>
#include "CollisionManager.h"
#include "../Object/Stage.h"
#include "../Object/StageCollision.h"
#include "../Object/Area1Collision.h"
#include "../Object/Area2Collision.h"
#include "../Object/Area3Collision.h"
#include "../Object/Area4Collision.h"
#include "../Object/Area5Collision.h"

CollisionManager::CollisionManager()
	:
	ATTACK_START_NUM(3),
	STAGE_COLLISION_COUNT(10),
	ENEMY_PUSH_FORCE(10.0f),
	OBJECT_COLLISION_PUSH_FORCE(50.0f),
	CAMERA_COLLISION_PUSH_FORCE(1000.0f),
	DOWN_DIR(0.9f),
	LEN_CAMERACOLLISION_LINE(500.0f),
	MIN_LEN_CAMERA_COLLISION_LINE(1.0f),
	collisionLineStageCamera_(0.0f)
{

}

void CollisionManager::Init()
{
}

void CollisionManager::Update(const float deltaTime)
{

	// �v���C���[�ƓG�̍U���̓����蔻����`�F�b�N
	CheckAttackCollision(deltaTime);

	// �v���C���[�ƓG�̔�ѓ���̓����蔻��
	CheckProjectileCollision(deltaTime);

	// �X�e�[�W�Ƃ̓����蔻����`�F�b�N
	CheckStageCollision();

	// �v���C���[�ƓG���m���d�Ȃ�Ȃ��悤�ɂ��铖���蔻����`�F�b�N
	CheckResolveCollision();

	// �����G���A���o�Ă���ꏊ�Ƃ̓����蔻��
	CheckRestrictedAreasCollision();

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

void CollisionManager::StageErasure(std::shared_ptr<StageBase>& actor)
{

	// �X�e�[�W�f�[�^��T��
	auto stageData = collisionActorData_.find(ActorType::STAGE);

	// �X�e�[�W�f�[�^�̒��g�����邩�`�F�b�N
	if (stageData == collisionActorData_.end()) return;

	// �X�e�[�W�̃f�[�^�����o��
	auto& stages = collisionActorData_.at(ActorType::STAGE);

	// �킽���Ă��������Ɠ����|�C���^������Δz�񂩂����
	std::erase_if(stages, [&actor](std::shared_ptr<ActorBase> actVal)
		{
			return (actor == std::dynamic_pointer_cast<StageBase>(actVal));
		});

}

void CollisionManager::SetCamera(const std::weak_ptr<Camera>& camera)
{
	camera_ = camera;
}

void CollisionManager::CheckAttackCollision(const float deltaTime)
{

	// ���G���Ԃ̌v�Z
	for (auto& data : isCloseRangeAttackHitData_)
	{
		for (auto& state : data.second)
		{
			// ���G���Ԃ����Z���Ă���
			state.second -= deltaTime;
		}
	}

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

				// �|�C���^�������Ă��邩�m�F
				if (!attacker)return;
				if (!target)return;

				// �U����Ԃɓ����Ă��Ȃ������瓖���蔻���ʂ�Ȃ�
				if (!attacker->GetCloseRangeAttackState())continue;

				// ���g�����邩�m�F
				auto hitData = isCloseRangeAttackHitData_.find(target);

				// ��������Ă��Ȃ��ꍇ�́A�V����vector�z��̔�����肻�̒��ɗv�f�����Ă���
				if (hitData == isCloseRangeAttackHitData_.end())
				{

					// �f�[�^�����
					std::map<int, float> data;
					for (int type = 0; type < attacker->GetTotalAttackTypes().size(); type++)
					{
						data.emplace(type, 0.0f);
					}

					// �����������̂��i�[
					isCloseRangeAttackHitData_.emplace(target, data);

					// ��ō�����f�[�^���i�[
					hitData = isCloseRangeAttackHitData_.find(target);

				}

				// ���U�����Ă��鑤�̃A�j���[�V�������v�f�ɂ��邩�m�F����
				auto a = hitData->second.find(attacker->GetState() - ATTACK_START_NUM);

				// ���g��������Ώ������Ȃ�
				if (a == hitData->second.end())continue;

				// ���̃A�j���[�V�������̖��G���Ԃ������Ă��Ȃ���Ώ������Ȃ�
				if (a->second > 0.0f)continue;

				// �E��̔���
				if (HitCheck_Capsule_Capsule(attacker->GetCollisionData().rightHandCapsuleUpPos, attacker->GetCollisionData().rightHandCapsuleDownPos,
					attacker->GetCollisionData().handAndFootCollisionRadius,
					target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius)
					&& attacker->GetCollisionData().isRightHandAttack)
				{
					// �����������̏���
					OnAttackCollision(attacker, target);
				}
				// ����̔���
				if (HitCheck_Capsule_Capsule(attacker->GetCollisionData().leftHandCapsuleUpPos, attacker->GetCollisionData().leftHandCapsuleDownPos,
					attacker->GetCollisionData().handAndFootCollisionRadius,
					target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius)
					&& attacker->GetCollisionData().isLeftHandAttack)
				{
					// �����������̏���
					OnAttackCollision(attacker, target);
				}
				// �E���̔���
				if (HitCheck_Capsule_Capsule(attacker->GetCollisionData().rightFootCapsuleUpPos, attacker->GetCollisionData().rightFootCapsuleDownPos,
					attacker->GetCollisionData().handAndFootCollisionRadius,
					target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius)
					&& attacker->GetCollisionData().isRightFootAttack)
				{
					// �����������̏���
					OnAttackCollision(attacker, target);
				}
				// �����̔���
				if (HitCheck_Capsule_Capsule(attacker->GetCollisionData().leftFootCapsuleUpPos, attacker->GetCollisionData().leftFootCapsuleDownPos,
					attacker->GetCollisionData().handAndFootCollisionRadius,
					target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius)
					&& attacker->GetCollisionData().isLeftFootAttack)
				{
					// �����������̏���
					OnAttackCollision(attacker, target);
				}

			}
		}
	}

}

void CollisionManager::OnAttackCollision(const std::shared_ptr<ActorBase>& attacker, const std::shared_ptr<ActorBase>& target)
{

	// �U����������������
	target->AttackHit(attacker->GetDamage(), attacker->GetState());

	// ����̍��W��ݒ�
	target->SetTargetPos(attacker->GetTransform()->pos);

	// �U�������������t���O��true�ɂ���
	attacker->SetIsHitAttack(true);

	// ���������^�[�Q�b�g�̏����擾
	auto& data = isCloseRangeAttackHitData_[target];

	// �^�[�Q�b�g�ɍ��U�����ꂽ�U����Ԃ̖��G���Ԃ�ݒ肷��
	data[attacker->GetState() - ATTACK_START_NUM] = 1.0f;

}

void CollisionManager::CheckProjectileCollision(const float deltaTime)
{

	// ���G���Ԃ̌v�Z
	for (auto& data : isLongRangeAttackHitData_)
	{
		// ���G���Ԃ����Z���Ă���
		data.second -= deltaTime;
	}

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

				// �|�C���^�������Ă��邩�m�F
				if (!attacker)return;
				if (!target)return;

				// ���g�����邩�m�F
				auto hitData = isLongRangeAttackHitData_.find(target);

				// ��������Ă��Ȃ��ꍇ�́A�V����vector�z��̔�����肻�̒��ɗv�f�����Ă���
				if (hitData == isLongRangeAttackHitData_.end())
				{

					// ���G���Ԃ����
					float invincibleTime = 0.0f;

					// �����������̂��i�[
					isLongRangeAttackHitData_.emplace(target, invincibleTime);

					// ��ō�����f�[�^���i�[
					hitData = isLongRangeAttackHitData_.find(target);

				}

				// ���̃A�j���[�V�������̖��G���Ԃ������Ă��Ȃ���Ώ������Ȃ�
				if (hitData->second > 0.0f)continue;

				// ��ѓ���̔���
				if (HitCheck_Sphere_Capsule(attacker->GetCollisionData().projectilePos, attacker->GetCollisionData().projectileCollisionRadius,
					target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius)
					&& attacker->GetCollisionData().isProjectileAttack)
				{
					// �����������̏���
					OnProjectileCollision(attacker, target);
				}
			}
		}
	}

}

void CollisionManager::OnProjectileCollision(const std::shared_ptr<ActorBase>& attacker, const std::shared_ptr<ActorBase>& target)
{

	// ��ѓ��������������
	target->ProjectileHit(attacker->GetDamage());

	// ����̍��W��ݒ�
	target->SetTargetPos(attacker->GetTransform()->pos);

	// �U�������������t���O��true�ɂ���
	attacker->SetIsHitAttack(true);

	// �G�̔�ѓ���������蔻�������
	if (attacker->GetActorType() == ActorType::BOSS)
	{
		attacker->SetIsProjectileCollision(false);
	}

	// ���������^�[�Q�b�g�̏����擾
	auto& data = isLongRangeAttackHitData_[target];

	// �^�[�Q�b�g�ɍ��U�����ꂽ�U����Ԃ̖��G���Ԃ�ݒ肷��
	data = 1.0f;

}

void CollisionManager::CheckStageCollision()
{

	// �A�N�^�[�ƃX�e�[�W�Ƃ̓����蔻��
	CheckActorsAndStageCollision();

	// �J�����ƃX�e�[�W�Ƃ̓����蔻��
	CheckCameraAndStageCollision();

}

void CollisionManager::CheckActorsAndStageCollision()
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

				// �|�C���^�������Ă��邩�m�F
				if (!target)return;
				if (!stage)return;

				// �X�e�[�W�ɏ���ύX
				auto stageData = std::dynamic_pointer_cast<StageBase>(stage);

				// �X�e�[�W�ɓ����蔻�肪���邩�`�F�b�N
				if (!stageData->GetIsCollision()) continue;

				// �J�v�Z���Ƃ̏Փ˔���
				MV1_COLL_RESULT_POLY_DIM hits = MV1CollCheck_Capsule(
					stage->GetTransform()->modelId, -1,
					target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius);

				// �Փ˂��������̃|���S���ƏՓˉ������܂ŁA
				// �v���C���[�̈ʒu���ړ�������
				for (int i = 0; i < hits.HitNum; i++)
				{

					auto hit = hits.Dim[i];

					// �n�ʂƈقȂ�A�Փˉ���ʒu���s���Ȃ��߁A���x���ړ�������
					// ���̎��A�ړ�����������́A�ړ��O���W�Ɍ����������ł�������A
					// �Փ˂����|���S���̖@�������������肷��
					for (int tryCnt = 0; tryCnt < STAGE_COLLISION_COUNT; tryCnt++)
					{

						// �ēx�A���f���S�̂ƏՓˌ��o����ɂ́A���������߂���̂ŁA
						// �ŏ��̏Փ˔���Ō��o�����Փ˃|���S��1���ƏՓ˔�������
						int pHit = HitCheck_Capsule_Triangle(
							target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius,
							hit.Position[0], hit.Position[1], hit.Position[2]);

						if (pHit)
						{

							// y�����ɂ͏������Ȃ�
							if (hit.Normal.y == 1.0f)continue;

							// �������Ă��邩���肵����Actor�̍��W
							VECTOR pos = target->GetTransform()->pos;

							// �@���̕����ɂ�����Ƃ����ړ�������
							pos = VAdd(pos, VScale(hit.Normal, OBJECT_COLLISION_PUSH_FORCE));

							// �J�v�Z�����ꏏ�Ɉړ�������
							target->SetPos(pos);

						}
					}
				}

				// ���o�����n�ʃ|���S�����̌�n��
				MV1CollResultPolyDimTerminate(hits);

				// StgaeCollision�ɃL���X�g
				auto stageCollision = std::dynamic_pointer_cast<StageCollision>(stage);

				// StageCollision�o�Ȃ���Ώ������Ȃ�
				if (stageCollision)
				{

					// �n�ʂƂ̏Փ�
					MV1_COLL_RESULT_POLY hit = MV1CollCheck_Line(
						stage->GetTransform()->modelId, -1,
						VAdd(target->GetCollisionData().bodyCapsuleUpPos, VECTOR(0.0f, target->GetCollisionData().bodyCollisionRadius, 0.0f)),
						VAdd(target->GetCollisionData().bodyCapsuleDownPos, VECTOR(0.0f, -target->GetCollisionData().bodyCollisionRadius, 0.0f)));

					// �n�ʂɓ������Ă��鎞�Ɖ������ɓ����Ă��鎞�̂ݔ��肷��
					if (hit.HitFlag > 0 && VDot({ 0.0f,-1.0f,0.0f }, target->GetVelocity()) > DOWN_DIR)
					{

						// �Փ˒n�_����A������Ɉړ�
						target->SetPos(hit.HitPosition);

						// �Փ˂��Ă���ɕύX
						target->SetIsCollisionFloor(true);

						// �W�����v���Z�b�g
						target->SetVelocity(Utility::VECTOR_ZERO);

					}
					else
					{
						// �Փ˂��Ă��Ȃ��ɕύX
						target->SetIsCollisionFloor(false);
					}
				}
			}

			// �����n�ʂ��ђʂ��ĉ��ɍs���Ă��܂����Ƃ�
			//if (target->GetTransform()->pos.y < -19500.0f)
			//{
			//	target->SetPos({ target->GetTransform()->pos.x, -19500.0f,target->GetTransform()->pos.z });
			//}

		}
	}

}

void CollisionManager::CheckCameraAndStageCollision()
{

	// �X�e�[�W
	const auto& stages = collisionActorData_.find(ActorType::STAGE);

	// ���g�������Ă��邩�m�F
	if (stages == collisionActorData_.end())return;

	// �J�������W
	auto cPos = camera_.lock()->GetPos();

	// �����_
	auto tPos = camera_.lock()->GetTargetPos();

	// �J�������璍���_�Ɍ����Ẵx�N�g��
	auto cDir = VNorm(VSub(tPos, cPos));

	for (const std::shared_ptr<ActorBase>& stage : stages->second)
	{

		// �X�e�[�W�ɏ���ύX
		auto stageData = std::dynamic_pointer_cast<StageBase>(stage);

		// �X�e�[�W�ɓ����蔻�肪���邩�`�F�b�N
		if (!stageData->GetIsCollision()) continue;

		// �X�e�[�W�ƏՓ˂��Ă��邩����
		MV1_COLL_RESULT_POLY hit = MV1CollCheck_Line(stage->GetTransform()->modelId, -1, camera_.lock()->GetTargetPos(), camera_.lock()->GetPos());

		if (hit.HitFlag)
		{

			// ���������n�_�ɍ��W��ݒ�
			camera_.lock()->SetPos(VAdd(hit.HitPosition, VScale(cDir, 10.0f)));

			// �J�������W
			auto tmpCPos = camera_.lock()->GetPos();

			// �����_
			auto tmpTPos = camera_.lock()->GetTargetPos();

			// ���[���h�̏�����ƒn�ʂ̖@�����߂���������������J�����̏㏸�␳���������Ȃ�
			auto a = VDot({ 0.0f,1.0f,0.0f }, hit.Normal);
			if (a > 0.9f)return;

			// �J�����ƒ����_�̋������v�Z
			auto disPow2 = Utility::SqrMagnitude(tmpCPos, tmpTPos);

			// ����
			float len = 5000.0f;
			if (disPow2 < len * len)
			{
				auto l = Utility::Lerp(tmpCPos, VAdd(tmpCPos, { 0.0f,len ,0.0f }), 0.1f);
				camera_.lock()->SetPos(l);
			}

		}
	}
}

void CollisionManager::CheckResolveCollision()
{

	// �Փ˂��Ă��邩���肷��
	for (const collisionChannnelInfo& info : resolveCollisionChannelList_)
	{

		// �A�N�^�[1
		const auto& actors1 = collisionActorData_.find(info.type1);

		// �A�N�^�[2
		const auto& actors2 = collisionActorData_.find(info.type2);

		// ���g�������Ă��邩�m�F
		if (actors1 == collisionActorData_.end())continue;
		if (actors2 == collisionActorData_.end())continue;

		for (const std::shared_ptr<ActorBase>& actor1 : actors1->second)
		{
			for (const std::shared_ptr<ActorBase>& actor2 : actors2->second)
			{

				// �|�C���^�������Ă��邩�m�F
				if (!actor1)return;
				if (!actor2)return;

				// �v���C���[�ƓG���d�Ȃ��Ă��Ƃ��̏���
				if (actor1->GetActorType() == ActorType::PLAYER)
				{
					ResolvePlayerEnemyCollision(actor1, actor2);
				}
				// �v���C���[�ƓG���d�Ȃ��Ă��Ƃ��̏���
				else if (actor1->GetActorType() == ActorType::ENEMY)
				{
					ResolveEnemysCollision(actor1, actor2);
				}

			}
		}
	}

}

void CollisionManager::ResolvePlayerEnemyCollision(const std::shared_ptr<ActorBase>& actor1, const std::shared_ptr<ActorBase>& actor2)
{

	// �J�v�Z�����m�̓����蔻��
	if (HitCheck_Capsule_Capsule(actor1->GetCollisionData().bodyCapsuleUpPos, actor1->GetCollisionData().bodyCapsuleDownPos, actor1->GetCollisionData().bodyCollisionRadius,
								 actor2->GetCollisionData().bodyCapsuleUpPos, actor2->GetCollisionData().bodyCapsuleDownPos, actor2->GetCollisionData().bodyCollisionRadius))
	{

		// �d�Ȃ��Ă�����݂��ɉ����Ԃ�
		VECTOR overlap = VSub(actor2->GetCollisionData().bodyPos, actor1->GetCollisionData().bodyPos);

		// ���K������
		VECTOR normOverlap = VNorm(overlap);

		// �G�����ړ�������
		actor2->AddPos(VScale(normOverlap, ENEMY_PUSH_FORCE));

	}

}

void CollisionManager::ResolveEnemysCollision(const std::shared_ptr<ActorBase>& actor1, const std::shared_ptr<ActorBase>& actor2)
{

	// �J�v�Z�����m�̓����蔻��
	if (HitCheck_Capsule_Capsule(actor1->GetCollisionData().bodyCapsuleUpPos, actor1->GetCollisionData().bodyCapsuleDownPos, actor1->GetCollisionData().bodyCollisionRadius,
								 actor2->GetCollisionData().bodyCapsuleUpPos, actor2->GetCollisionData().bodyCapsuleDownPos, actor2->GetCollisionData().bodyCollisionRadius))
	{

		// �d�Ȃ��Ă�����݂��ɉ����Ԃ�
		VECTOR overlap = VSub(actor2->GetCollisionData().bodyPos, actor1->GetCollisionData().bodyPos);

		// ���K������
		VECTOR normOverlap = VNorm(overlap);

		// �������Ȃ���
		normOverlap.y = 0.0f;

		// ���݂��Ɉړ�������
		actor1->AddPos(VScale(normOverlap, -ENEMY_PUSH_FORCE / 2));
		actor2->AddPos(VScale(normOverlap, ENEMY_PUSH_FORCE / 2));

	}

}

void CollisionManager::CheckRestrictedAreasCollision()
{

	// �X�e�[�W�Ɠ������Ă��邩���肵��������
	const auto& players = collisionActorData_.find(ActorType::PLAYER);

	// �X�e�[�W
	const auto& stages = collisionActorData_.find(ActorType::STAGE);

	// ���g�������Ă��邩�m�F
	if (players == collisionActorData_.end())return;
	if (stages == collisionActorData_.end())return;

	for (const std::shared_ptr<ActorBase>& player : players->second)
	{
		for (const std::shared_ptr<ActorBase>& stage : stages->second)
		{

			// �|�C���^�������Ă��邩�m�F
			if (!player)return;
			if (!stage)return;

			// �X�e�[�W�ɏ���ύX
			auto stageData = std::dynamic_pointer_cast<StageBase>(stage);

			// �X�e�[�W�ɓ����蔻�肪���邩�`�F�b�N
			if ((!stageData->GetIsRestrictedArea() || stageData->GetIsCollision())) continue;

			// �Փ˂��Ă��邩����
			if (HitCheck_Sphere_Sphere(
				player->GetTransform()->pos,
				player->GetCollisionData().bodyCollisionRadius,
				stageData->GetRestrictedAreaPos(),stageData->GetRestrictedAreaRadius()))
			{
				// �����G���A�̓����蔻���t����
				stageData->SetIsCollision(true);
			}

		}
	}

}