#include <random>
#include "BossAIComponent.h"
#include "../Object/Boss.h"

BossAIComponent::BossAIComponent(const std::shared_ptr<Boss> boss)
{
	boss_ = boss;
	actor_ = boss;
}

void BossAIComponent::Update(const float deltaTime)
{

	// �ǂ̍s�������邩���߂�
	if (!boss_->GetIsActionDecided() && boss_->GetCoolTime() <= 0.0f)
	{
		SelectAction(deltaTime);
	}

}

void BossAIComponent::SelectAction(const float deltaTime)
{

	// �q�b�g���͍s���ł��Ȃ�
	if (boss_->GetHitState())return;

	// ����

	// �񌈒�I�ȗ���������
	std::random_device rd;

	// �����Z���k�E�c�C�X�^�@�ɂ�闐��������
	std::mt19937 gen(rd());

	// �w��͈̔͂Ń����_���Ȑ����擾
	std::uniform_int_distribution<> dist_int(0, static_cast<int>(ACTION_TYPE::MAX) - 1);
	int number = dist_int(gen);

	if (number == static_cast<int>(ACTION_TYPE::MOVE))
	{
		// �ړ�����
		Move(deltaTime);
	}
	else if (number == static_cast<int>(ACTION_TYPE::CLOSE_RANGE))
	{
		// �ߋ����U������
		CloseRangeAttack(deltaTime);
	}
	else if (number == static_cast<int>(ACTION_TYPE::LONG_RANGE))
	{
		// �������U������
		LongRangeAttack(deltaTime);
	}
	else if (number == static_cast<int>(ACTION_TYPE::CALL) && boss_->GetCreateEnemyCoolTimeCnt() == 0.0f)
	{

		// �G�����Ăԍs��
		boss_->ChangeState(BossState::CALL);

		// �s�������߂�
		boss_->SetIsActionDecided(true);

	}

}

void BossAIComponent::Move(const float deltaTime)
{

	// �v���C���[�̍��W
	std::optional<VECTOR> playerPos = boss_->GetPlayerPos();
	boss_->SetTargetPos(playerPos.value());

	// �G����v���C���[�ւ̃x�N�g��
	VECTOR vec = VSub(boss_->GetTargetPos(), boss_->GetTransform()->pos);

	// �x�N�g���̒���
	float length = Utility::Magnitude(vec);

	// �v���C���[�̋߂��Ɉړ�
	if (length >= boss_->ACTIVATION_DISTANCE)
	{

		boss_->ChangeState(BossState::RUN);

		// �s�������߂�
		boss_->SetIsActionDecided(true);

	}

}

void BossAIComponent::CloseRangeAttack(const float deltaTime)
{

	// ����

	// �񌈒�I�ȗ���������
	std::random_device rd;

	// �����Z���k�E�c�C�X�^�@�ɂ�闐��������
	std::mt19937 gen(rd());

	// �w��͈̔͂Ń����_���Ȑ����擾
	std::uniform_int_distribution<> dist_int(0, static_cast<int>(CLOSE_RANGE_TYPE::MAX) - 1);
	int number = dist_int(gen);

	// �v���C���[�̍��W
	std::optional<VECTOR> playerPos = boss_->GetPlayerPos();

	// ����̍��W
	boss_->SetTargetPos(playerPos.value());

	// �G����v���C���[�ւ̃x�N�g��
	VECTOR vec = VSub(boss_->GetTargetPos(), boss_->GetTransform()->pos);

	// �x�N�g���̒���
	float length = Utility::Magnitude(vec);

	// �v���C���[�Ƃ̋���������������U���ł��Ȃ�
	if (length >= boss_->ACTIVATION_DISTANCE) return;

	// �������p�x�ɕϊ�����
	float angle = atan2f(vec.x, vec.z);

	// �v���C���[�����ɉ�]
	boss_->LazyRotation(angle);

	// ���W�A������f�O���[
	float goalDeg = Utility::Rad2DegF(angle);

	// ���������p�x��360�x���Ɏ��߂�
	goalDeg = Utility::DegIn360(goalDeg);

	// �{�X�̃��W�A���p
	auto rad = boss_->GetTransform()->quaRot.ToEuler();

	// �{�X�̃f�O���[�p
	auto deg = Utility::Rad2DegF(rad.y);

	// �{�X�̊p�x��360�x�Ɏ��߂�
	deg = Utility::DegIn360(deg);

	// �ړI�̊p�x�Ǝ����̊p�x�̍��𑪂�
	float sub = goalDeg - deg;

	// ��Βl�����߂�
	sub = abs(sub);

	// ��]������܂ŏ������Ȃ�
	if (sub >= ANGLE_TOLERANCE)return;

	if (number == static_cast<int>(CLOSE_RANGE_TYPE::PUNCH))
	{

		// �p���`�U���ɑJ��
		boss_->ChangeState(BossState::ATTACK_PUNCH);

		// �s�������߂�
		boss_->SetIsActionDecided(true);

	}
	else if (number == static_cast<int>(CLOSE_RANGE_TYPE::KICK))
	{

		// �L�b�N�U���ɑJ��
		boss_->ChangeState(BossState::ATTACK_KICK);

		// �s�������߂�
		boss_->SetIsActionDecided(true);

	}

}

void BossAIComponent::LongRangeAttack(const float deltaTime)
{

	// �v���C���[�̍��W
	std::optional<VECTOR> playerPos = boss_->GetPlayerPos();

	// ����̍��W
	boss_->SetTargetPos(playerPos.value());

	// �G����v���C���[�ւ̃x�N�g��
	VECTOR vec = VSub(boss_->GetTargetPos(), boss_->GetTransform()->pos);

	// �x�N�g���̒���
	float length = Utility::Magnitude(vec);

	// �v���C���[�Ƃ̋���������������U���ł��Ȃ�
	if (length < boss_->LONG_RANGE_ATTACK_DISTANCE) return;

	// �������p�x�ɕϊ�����
	float angle = atan2f(vec.x, vec.z);

	// �v���C���[�����ɉ�]
	boss_->LazyRotation(angle);

	// ���W�A������f�O���[
	float goalDeg = Utility::Rad2DegF(angle);

	// ���������p�x��360�x���Ɏ��߂�
	goalDeg = Utility::DegIn360(goalDeg);

	// �{�X�̃��W�A���p
	auto rad = boss_->GetTransform()->quaRot.ToEuler();

	// �{�X�̃f�O���[�p
	auto deg = Utility::Rad2DegF(rad.y);

	// �{�X�̊p�x��360�x�Ɏ��߂�
	deg = Utility::DegIn360(deg);

	// �ړI�̊p�x�Ǝ����̊p�x�̍��𑪂�
	float sub = goalDeg - deg;

	// ��Βl�����߂�
	sub = abs(sub);

	// ��]������܂ŏ������Ȃ�
	if (sub >= ANGLE_TOLERANCE)return;

	// ��ѓ���U���ɑJ��
	boss_->ChangeState(BossState::ATTACK_PROJECTILE);

	// �s�������߂�
	boss_->SetIsActionDecided(true);

}
