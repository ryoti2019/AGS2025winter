#include <map>
#include "InputComponent.h"
#include "../Object/ActorBase.h"
#include "../Object/Common/InputController.h"
#include "../Object/PlayerState.h"
#include "../Object/Player.h"

InputComponent::InputComponent()
{
	inputController_ = std::make_unique<InputController>();
}

void InputComponent::Update()
{

	//// �q�b�g���͍s���ł��Ȃ�
	//if (actor_->GetHitState())return;

	//// ���͕���
	//VECTOR dir = inputController_->Dir();

	//// �U�����͈ړ��ł��Ȃ�
	//if (!actor_->GetAttackState())
	//{
	//	// ���͂��Ă�����ړ�����
	//	if (!Utility::EqualsVZero(dir))
	//	{
	//		// �������X�V
	//		actor_->SetDir(dir);

	//		actor_->ChangeState(PlayerState::RUN);
	//	}
	//	// ���͂��Ă��Ȃ���Αҋ@��Ԃɂ���
	//	else if (Utility::EqualsVZero(dir))
	//	{
	//		actor_->ChangeState(PlayerState::IDLE);
	//	}
	//}

	//// ���߃p���`�p�̃{�^���𒷉������Ă��邩
	//if (inputController_->ChargeAttack() && player->GetChargeCnt() <= player->CHARGE_TIME)
	//{
	//	// �����Ă�������Z����
	//	chargeCnt_ += deltaTime;
	//}

	//// ���߃p���`
	//if (chargeCnt_ >= player->CHARGE_TIME)
	//{
	//	chargeCnt_ = 0.0f;
	//	player->ChangeState(PlayerState::ATTACK_CHARGE_PUNCH);
	//}

	//// �R���{�̐�s���͂��������
	//std::map<PlayerState, bool> combo;

	//// �U���̐�s���� ���ߍU���̌�ʂ�Ȃ��悤�ɂ���
	//if (inputController_->Attack() && player->GetKey() != player->ANIM_DATA_KEY[static_cast<int>(PlayerState::ATTACK_CHARGE_PUNCH)])
	//{
	//	// �R���{�̐�s���͂̏���
	//	for (int i = static_cast<int>(PlayerState::ATTACK_JAB); i <= static_cast<int>(PlayerState::ATTACK_RIGHT_KICK); i++)
	//	{

	//		// �����画�f������̂�T��
	//		auto isCombo = player->GetIsCombo().find(static_cast<PlayerState>(i));

	//		// false�ł����true�ɂ��Đ�s���͂�ێ�����
	//		if (!isCombo->second)
	//		{
	//			combo.emplace(static_cast<PlayerState>(i), true);
	//			player->SetIsCombo(combo);
	//			break;
	//		}
	//	}

	//	// �W���u�ɑJ��
	//	if (player->GetIsCombo().at(PlayerState::ATTACK_JAB) && !player->GetIsCombo().at(PlayerState::ATTACK_STRAIGHT))
	//	{
	//		player->ChangeState(PlayerState::ATTACK_JAB);
	//	}

	//}

	//// �A�b�p�[�ɑJ��
	//if (inputController_->Upper() && player->GetState() != static_cast<int>(PlayerState::ATTACK_UPPER))
	//{
	//	player->ChangeState(PlayerState::ATTACK_UPPER);
	//}

	////�R���{��������
	//if (!player->GetComboState())return;

	//// ���̓��͂��Ȃ���΃R���{���L�����Z������
	//for (int i = static_cast<int>(PlayerState::ATTACK_JAB); i < static_cast<int>(PlayerState::ATTACK_RIGHT_KICK); i++)
	//{

	//	// ���̏�Ԃ̓��͂����邽�߂̌v�Z
	//	const int nextState = player->GetState() + 1;

	//	// false��������R���{���L�����Z��
	//	if (!player->GetIsCombo().at(static_cast<PlayerState>(nextState)))
	//	{
	//		// �ҋ@��ԂɑJ��
	//		if (player->GetAnimationController()->IsEndPlayAnimation())
	//		{
	//			player->ChangeState(PlayerState::IDLE);
	//			return;
	//		}
	//	}

	//}

}

void InputComponent::Move()
{
}

void InputComponent::Attack(const float deltaTime)
{
}
