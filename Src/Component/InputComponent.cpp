#include <map>
#include "InputComponent.h"
#include "../Object/ActorBase.h"
#include "../Object/Player.h"

InputComponent::InputComponent(std::shared_ptr<Player> player)
{
	inputController_ = std::make_unique<InputController>();
	player_ = player;
	actor_ = player;
}

void InputComponent::Update(const float deltaTime)
{

	// �q�b�g���͍s���ł��Ȃ�
	if (player_->GetHitState() || player_->GetHp() <= 0)return;

	// �U��
	Attack(deltaTime);

	// �ړ�
	Move();

	// ���
	Evasion();

}

void InputComponent::Dir()
{

	// ���͕���
	VECTOR dir = inputController_->Dir();

	if (!Utility::EqualsVZero(dir))
	{
		// �������X�V
		player_->SetDir(dir);
	}

}

void InputComponent::Move()
{

	// ���͕���
	VECTOR dir = inputController_->Dir();

	// �U�����͈ړ��ł��Ȃ�
	if (!player_->GetAttackState() && player_->GetState() != static_cast<int>(PlayerState::POWER_CHARGE)
		&& player_->GetState() != static_cast<int>(PlayerState::EVASION))
	{
		// ���͂��Ă�����ړ�����
		if (!Utility::EqualsVZero(dir))
		{
			// �������X�V
			player_->SetDir(dir);

			player_->ChangeState(PlayerState::RUN);
		}
		// ���͂��Ă��Ȃ���Αҋ@��Ԃɂ���
		else if (Utility::EqualsVZero(dir))
		{
			player_->ChangeState(PlayerState::IDLE);
		}
	}

}

void InputComponent::Attack(const float deltaTime)
{

	// �K�E�Z�ɑJ��
	if (inputController_->SpecialAttack() && player_->GetState() != static_cast<int>(PlayerState::ATTACK_SPECIAL_PUNCH)
		&& player_->GetSpecialAttackGauge() >= player_->SPECIAL_ATTACK_MAX_GAUGE)
	{
		player_->ChangeState(PlayerState::POWER_CHARGE);
	}

	// �K�E�Z�̎��Ɨ͂����߂�Ƃ��͉�������ł��Ȃ��悤�ɂ���
	if (static_cast<PlayerState>(player_->GetState()) == PlayerState::POWER_CHARGE || static_cast<PlayerState>(player_->GetState()) == PlayerState::ATTACK_SPECIAL_PUNCH)return;

	// ���߃p���`�p�̃{�^���𒷉������Ă��邩
	if (inputController_->ChargeAttack() && player_->GetChargeCnt() <= player_->ATTACK_CHARGE_PUNCH_TIME)
	{
		// �����Ă�������Z����
		player_->AddChargeCnt(deltaTime);
	}

	// ���߃p���`
	if (player_->GetChargeCnt() >= player_->ATTACK_CHARGE_PUNCH_TIME)
	{
		player_->SetChargeCnt(0.0f);
		player_->ChangeState(PlayerState::ATTACK_CHARGE_PUNCH);
	}

	// �U���̐�s���� ���ߍU���̌�ʂ�Ȃ��悤�ɂ���
	if (inputController_->Attack()/* && player_->GetKey() != player_->ANIM_DATA_KEY[static_cast<int>(PlayerState::ATTACK_CHARGE_PUNCH)]*/)
	{

		// �R���{�̐�s���͂̏���
		for (int i = static_cast<int>(PlayerState::ATTACK_JAB); i <= static_cast<int>(PlayerState::ATTACK_RIGHT_KICK); i++)
		{

			// �����画�f������̂�T��
			auto isCombo = player_->GetIsCombo().find(static_cast<PlayerState>(i));

			// false�ł����true�ɂ��Đ�s���͂�ێ�����
			if (!isCombo->second)
			{
				player_->SetIsCombo(i, true);
				break;
			}
			
		}

		// �W���u�ɑJ��
		if (player_->GetIsCombo().at(PlayerState::ATTACK_JAB) && !player_->GetIsCombo().at(PlayerState::ATTACK_STRAIGHT))
		{
			player_->ChangeState(PlayerState::ATTACK_JAB);
		}

	}

	// �A�b�p�[�ɑJ��
	if (inputController_->Upper() && player_->GetState() != static_cast<int>(PlayerState::ATTACK_UPPER))
	{
		player_->ChangeState(PlayerState::ATTACK_UPPER);
	}

	//�R���{��������
	if (!player_->GetComboState())return;

	// �R���{���̍U�������肷��
	if (player_->GetState() >= static_cast<int>(PlayerState::ATTACK_RIGHT_KICK))return;

	// ���̓��͂��Ȃ���΃R���{���L�����Z������
	for (int i = static_cast<int>(PlayerState::ATTACK_JAB); i < static_cast<int>(PlayerState::ATTACK_RIGHT_KICK); i++)
	{

		// ���̏�Ԃ̓��͂����邽�߂̌v�Z
		const int nextState = player_->GetState() + 1;

		// false��������R���{���L�����Z��
		if (!player_->GetIsCombo().at(static_cast<PlayerState>(nextState)))
		{
			// �ҋ@��ԂɑJ��
			if (player_->GetAnimationController()->IsEndPlayAnimation())
			{
				player_->ChangeState(PlayerState::IDLE);
				return;
			}
		}

	}

}

void InputComponent::Evasion()
{

	// �K�E�Z�̎��Ɨ͂����߂�Ƃ��͉�������ł��Ȃ��悤�ɂ���
	if (static_cast<PlayerState>(player_->GetState()) == PlayerState::POWER_CHARGE || static_cast<PlayerState>(player_->GetState()) == PlayerState::ATTACK_SPECIAL_PUNCH)return;

	// ������悤�Ƃ��Ă��邩����
	if (inputController_->Dir().x != 0.0f && inputController_->Evasion())
	{
		// �����ԂɑJ��
		player_->ChangeState(PlayerState::EVASION);
	}

}
