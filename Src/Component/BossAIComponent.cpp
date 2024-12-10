#include <random>
#include "BossAIComponent.h"
#include "../Object/Boss.h"

BossAIComponent::BossAIComponent(const std::shared_ptr<Boss> boss)
{
	boss_ = boss;
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
	std::uniform_int_distribution<> dist_int(0, 1);
	int number = dist_int(gen);

	if (number == 0)
	{
		// �ړ�����
		Move(deltaTime);
	}
	else if (number == 1)
	{
		//�U������
		Attack(deltaTime);
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

void BossAIComponent::Attack(const float deltaTime)
{

	// ����

	// �񌈒�I�ȗ���������
	std::random_device rd;

	// �����Z���k�E�c�C�X�^�@�ɂ�闐��������
	std::mt19937 gen(rd());

	// �w��͈̔͂Ń����_���Ȑ����擾
	std::uniform_int_distribution<> dist_int(2, 2);
	int number = dist_int(gen);

	// �v���C���[�̍��W
	std::optional<VECTOR> playerPos = boss_->GetPlayerPos();

	// ����̍��W
	boss_->SetTargetPos(playerPos.value());

	// �G����v���C���[�ւ̃x�N�g��
	VECTOR vec = VSub(boss_->GetTargetPos(), boss_->GetTransform()->pos);

	// �x�N�g���̒���
	float length = Utility::Magnitude(vec);

	if (number == 2)
	{

		boss_->ChangeState(BossState::ATTACK_PROJECTILE);

		// �s�������߂�
		boss_->SetIsActionDecided(true);

	}

	// �v���C���[�Ƃ̋���������������U���ł��Ȃ�
	if (length >= boss_->ACTIVATION_DISTANCE) return;

	if (number == 0)
	{

		boss_->ChangeState(BossState::ATTACK_PUNCH);

		// �s�������߂�
		boss_->SetIsActionDecided(true);

	}
	else if (number == 1)
	{

		boss_->ChangeState(BossState::ATTACK_KICK);

		// �s�������߂�
		boss_->SetIsActionDecided(true);

	}



}
