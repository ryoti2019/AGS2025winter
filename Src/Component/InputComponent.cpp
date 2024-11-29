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

	//// ヒット中は行動できない
	//if (actor_->GetHitState())return;

	//// 入力方向
	//VECTOR dir = inputController_->Dir();

	//// 攻撃中は移動できない
	//if (!actor_->GetAttackState())
	//{
	//	// 入力していたら移動する
	//	if (!Utility::EqualsVZero(dir))
	//	{
	//		// 方向を更新
	//		actor_->SetDir(dir);

	//		actor_->ChangeState(PlayerState::RUN);
	//	}
	//	// 入力していなければ待機状態にする
	//	else if (Utility::EqualsVZero(dir))
	//	{
	//		actor_->ChangeState(PlayerState::IDLE);
	//	}
	//}

	//// 溜めパンチ用のボタンを長押ししているか
	//if (inputController_->ChargeAttack() && player->GetChargeCnt() <= player->CHARGE_TIME)
	//{
	//	// 押していたら加算する
	//	chargeCnt_ += deltaTime;
	//}

	//// 溜めパンチ
	//if (chargeCnt_ >= player->CHARGE_TIME)
	//{
	//	chargeCnt_ = 0.0f;
	//	player->ChangeState(PlayerState::ATTACK_CHARGE_PUNCH);
	//}

	//// コンボの先行入力を入れるもの
	//std::map<PlayerState, bool> combo;

	//// 攻撃の先行入力 ため攻撃の後通らないようにする
	//if (inputController_->Attack() && player->GetKey() != player->ANIM_DATA_KEY[static_cast<int>(PlayerState::ATTACK_CHARGE_PUNCH)])
	//{
	//	// コンボの先行入力の処理
	//	for (int i = static_cast<int>(PlayerState::ATTACK_JAB); i <= static_cast<int>(PlayerState::ATTACK_RIGHT_KICK); i++)
	//	{

	//		// 今から判断するものを探す
	//		auto isCombo = player->GetIsCombo().find(static_cast<PlayerState>(i));

	//		// falseであればtrueにして先行入力を保持する
	//		if (!isCombo->second)
	//		{
	//			combo.emplace(static_cast<PlayerState>(i), true);
	//			player->SetIsCombo(combo);
	//			break;
	//		}
	//	}

	//	// ジャブに遷移
	//	if (player->GetIsCombo().at(PlayerState::ATTACK_JAB) && !player->GetIsCombo().at(PlayerState::ATTACK_STRAIGHT))
	//	{
	//		player->ChangeState(PlayerState::ATTACK_JAB);
	//	}

	//}

	//// アッパーに遷移
	//if (inputController_->Upper() && player->GetState() != static_cast<int>(PlayerState::ATTACK_UPPER))
	//{
	//	player->ChangeState(PlayerState::ATTACK_UPPER);
	//}

	////コンボ中か判定
	//if (!player->GetComboState())return;

	//// 次の入力がなければコンボをキャンセルする
	//for (int i = static_cast<int>(PlayerState::ATTACK_JAB); i < static_cast<int>(PlayerState::ATTACK_RIGHT_KICK); i++)
	//{

	//	// 次の状態の入力を見るための計算
	//	const int nextState = player->GetState() + 1;

	//	// falseだったらコンボをキャンセル
	//	if (!player->GetIsCombo().at(static_cast<PlayerState>(nextState)))
	//	{
	//		// 待機状態に遷移
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
