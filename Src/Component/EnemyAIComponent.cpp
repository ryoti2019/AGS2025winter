#include <random>
#include "EnemyAIComponent.h"
#include "../Object/Enemy.h"

EnemyAIComponent::EnemyAIComponent(const std::shared_ptr<Enemy> enemy)
{
	enemy_ = enemy;
	actor_ = enemy;
}

void EnemyAIComponent::Update(const float deltaTime)
{

	// �ǂ̍s�������邩���߂�
	if (!enemy_->GetIsActionDecided() && enemy_->GetCoolTime() <= 0.0f)
	{
		SelectAction(deltaTime);
	}

}

void EnemyAIComponent::SelectAction(const float deltaTime)
{

	// �q�b�g���͍s���ł��Ȃ�
	if (enemy_->GetHitState())return;

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

void EnemyAIComponent::Move(const float deltaTime)
{

	// �v���C���[�̍��W
	std::optional<VECTOR> playerPos = enemy_->GetPlayerPos();
	enemy_->SetTargetPos(playerPos.value());

	// �G����v���C���[�ւ̃x�N�g��
	VECTOR vec = VSub(enemy_->GetTargetPos(), enemy_->GetTransform()->pos);

	// �x�N�g���̒���
	float length = Utility::Magnitude(vec);

	// �v���C���[�̋߂��Ɉړ�
	if (length >= enemy_->ACTIVATION_DISTANCE)
	{

		enemy_->ChangeState(EnemyState::RUN);

		// �s�������߂�
		enemy_->SetIsActionDecided(true);

	}

}

void EnemyAIComponent::Attack(const float deltaTime)
{

	// ����

	// �񌈒�I�ȗ���������
	std::random_device rd;

	// �����Z���k�E�c�C�X�^�@�ɂ�闐��������
	std::mt19937 gen(rd());

	// �w��͈̔͂Ń����_���Ȑ����擾
	std::uniform_int_distribution<> dist_int(0, 1);
	int number = dist_int(gen);

	// �v���C���[�̍��W
	std::optional<VECTOR> playerPos = enemy_->GetPlayerPos();

	// ����̍��W
	enemy_->SetTargetPos(playerPos.value());

	// �G����v���C���[�ւ̃x�N�g��
	VECTOR vec = VSub(enemy_->GetTargetPos(), enemy_->GetTransform()->pos);

	// �x�N�g���̒���
	float length = Utility::Magnitude(vec);

	// �v���C���[�Ƃ̋���������������U���ł��Ȃ�
	if (length >= enemy_->ACTIVATION_DISTANCE) return;

	if (number == 0)
	{

		enemy_->ChangeState(EnemyState::ATTACK_PUNCH);

		// �s�������߂�
		enemy_->SetIsActionDecided(true);

	}
	else if (number == 1)
	{

		enemy_->ChangeState(EnemyState::ATTACK_KICK);

		// �s�������߂�
		enemy_->SetIsActionDecided(true);

	}

}