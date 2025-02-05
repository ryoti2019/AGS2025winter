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

	// ヒット中は行動できない
	if (player_->GetHitState() || player_->GetHp() <= 0)return;

	// 攻撃
	Attack(deltaTime);

	// 移動
	Move();

	// 回避
	Evasion();

}

void InputComponent::Dir()
{

	// 入力方向
	VECTOR dir = inputController_->Dir();

	if (!Utility::EqualsVZero(dir))
	{
		// 方向を更新
		player_->SetDir(dir);
	}

}

void InputComponent::Move()
{

	// 入力方向
	VECTOR dir = inputController_->Dir();

	// 攻撃中は移動できない
	if (!player_->GetAttackState() && player_->GetState() != static_cast<int>(PlayerState::POWER_CHARGE)
		&& player_->GetState() != static_cast<int>(PlayerState::EVASION))
	{
		// 入力していたら移動する
		if (!Utility::EqualsVZero(dir))
		{
			// 方向を更新
			player_->SetDir(dir);

			player_->ChangeState(PlayerState::RUN);
		}
		// 入力していなければ待機状態にする
		else if (Utility::EqualsVZero(dir))
		{
			player_->ChangeState(PlayerState::IDLE);
		}
	}

}

void InputComponent::Attack(const float deltaTime)
{

	// 必殺技に遷移
	if (inputController_->SpecialAttack() && player_->GetState() != static_cast<int>(PlayerState::ATTACK_SPECIAL_PUNCH)
		&& player_->GetSpecialAttackGauge() >= player_->SPECIAL_ATTACK_MAX_GAUGE)
	{
		player_->ChangeState(PlayerState::POWER_CHARGE);
	}

	// 必殺技の時と力をためるときは何も操作できないようにする
	if (static_cast<PlayerState>(player_->GetState()) == PlayerState::POWER_CHARGE || static_cast<PlayerState>(player_->GetState()) == PlayerState::ATTACK_SPECIAL_PUNCH)return;

	// 溜めパンチ用のボタンを長押ししているか
	if (inputController_->ChargeAttack() && player_->GetChargeCnt() <= player_->ATTACK_CHARGE_PUNCH_TIME)
	{
		// 押していたら加算する
		player_->AddChargeCnt(deltaTime);
	}

	// 溜めパンチ
	if (player_->GetChargeCnt() >= player_->ATTACK_CHARGE_PUNCH_TIME)
	{
		player_->SetChargeCnt(0.0f);
		player_->ChangeState(PlayerState::ATTACK_CHARGE_PUNCH);
	}

	// 攻撃の先行入力 ため攻撃の後通らないようにする
	if (inputController_->Attack()/* && player_->GetKey() != player_->ANIM_DATA_KEY[static_cast<int>(PlayerState::ATTACK_CHARGE_PUNCH)]*/)
	{

		// コンボの先行入力の処理
		for (int i = static_cast<int>(PlayerState::ATTACK_JAB); i <= static_cast<int>(PlayerState::ATTACK_RIGHT_KICK); i++)
		{

			// 今から判断するものを探す
			auto isCombo = player_->GetIsCombo().find(static_cast<PlayerState>(i));

			// falseであればtrueにして先行入力を保持する
			if (!isCombo->second)
			{
				player_->SetIsCombo(i, true);
				break;
			}
			
		}

		// ジャブに遷移
		if (player_->GetIsCombo().at(PlayerState::ATTACK_JAB) && !player_->GetIsCombo().at(PlayerState::ATTACK_STRAIGHT))
		{
			player_->ChangeState(PlayerState::ATTACK_JAB);
		}

	}

	// アッパーに遷移
	if (inputController_->Upper() && player_->GetState() != static_cast<int>(PlayerState::ATTACK_UPPER))
	{
		player_->ChangeState(PlayerState::ATTACK_UPPER);
	}

	//コンボ中か判定
	if (!player_->GetComboState())return;

	// コンボ中の攻撃か判定する
	if (player_->GetState() >= static_cast<int>(PlayerState::ATTACK_RIGHT_KICK))return;

	// 次の入力がなければコンボをキャンセルする
	for (int i = static_cast<int>(PlayerState::ATTACK_JAB); i < static_cast<int>(PlayerState::ATTACK_RIGHT_KICK); i++)
	{

		// 次の状態の入力を見るための計算
		const int nextState = player_->GetState() + 1;

		// falseだったらコンボをキャンセル
		if (!player_->GetIsCombo().at(static_cast<PlayerState>(nextState)))
		{
			// 待機状態に遷移
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

	// 必殺技の時と力をためるときは何も操作できないようにする
	if (static_cast<PlayerState>(player_->GetState()) == PlayerState::POWER_CHARGE || static_cast<PlayerState>(player_->GetState()) == PlayerState::ATTACK_SPECIAL_PUNCH)return;

	// 回避しようとしているか判定
	if (inputController_->Dir().x != 0.0f && inputController_->Evasion())
	{
		// 回避状態に遷移
		player_->ChangeState(PlayerState::EVASION);
	}

}
