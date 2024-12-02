#pragma once
#include <DxLib.h>
#include <functional>
#include "../Component/InputComponent.h"
#include "ActorBase.h"
#include "PlayerState.h"
#include "EnemyState.h"

class InputComponent;

class Player : public ActorBase
{

public:

	// アニメーションコントローラーに渡す引数
	std::string ANIM_DATA_KEY[static_cast<int>(PlayerState::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN",
		"ATTACK_JAB",
		"ATTACK_STRAIGHT",
		"ATTACK_HOOK",
		"ATTACK_LEFT_KICK",
		"ATTACK_RIGHT_KICK",
		"ATTACK_UPPER",
		"ATTACK_CHARGE_PUNCH",
		"HIT_HEAD",
		"HIT_BODY"
	};

	// 入力でのアクション
	enum class InputAction
	{
		STICK,
		ATTACK,
		CHARGE_ATTACK,
		SHIFT_ATTACK
	};

	// ジャブの攻撃開始フレーム
	const float JAB_ATTACK_START_FRAME;

	// ジャブの攻撃終了フレーム
	const float JAB_ATTACK_END_FRAME;

	// ジャブのダメージ量
	const int JAB_DAMAGE;

	// ストレートの攻撃開始フレーム
	const float STRAIGHT_ATTACK_START_FRAME;

	// ストレートの攻撃終了フレーム
	const float STRAIGHT_ATTACK_END_FRAME;

	// ストレートのダメージ量
	const int STRAIGHT_DAMAGE;

	// フックの攻撃開始フレーム
	const float HOOK_ATTACK_START_FRAME;

	// フックの攻撃終了フレーム
	const float HOOK_ATTACK_END_FRAME;

	// フックのダメージ量
	const int HOOK_DAMAGE;

	// 左キックの攻撃開始フレーム
	const float LEFT_KICK_ATTACK_START_FRAME;

	// 左キックの攻撃終了フレーム
	const float LEFT_KICK_ATTACK_END_FRAME;

	// 左キックのダメージ量
	const int LEFT_KICK_DAMAGE;

	// 右キックの攻撃開始フレーム
	const float RIGHT_KICK_ATTACK_START_FRAME;

	// 右キックの攻撃終了フレーム
	const float RIGHT_KICK_ATTACK_END_FRAME;

	// 右キックのダメージ量
	const int RIGHT_KICK_DAMAGE;

	// アッパーの攻撃開始フレーム
	const float UPPER_ATTACK_START_FRAME;

	// アッパーの攻撃終了フレーム
	const float UPPER_ATTACK_END_FRAME;

	// アッパーのダメージ量
	const int UPPER_DAMAGE;

	// 溜めパンチの攻撃開始フレーム
	const float CHARGE_PUNCH_ATTACK_START_FRAME;

	// 溜めパンチの攻撃終了フレーム
	const float CHARGE_PUNCH_ATTACK_END_FRAME;

	// 溜めパンチのダメージ量
	const int CHARGE_PUNCH_DAMAGE;

	// 溜めパンチを出すためにボタン押す長さ
	const float CHARGE_TIME;

	Player(const VECTOR& pos, const json& data);

	~Player() = default;

	void Init(const VECTOR& pos) override;
	void Update(const float deltaTime) override;

	// 攻撃中の状態かを取得
	const bool GetAttackState()const override;

	// 攻撃種類を取得
	const std::vector<int> GetTotalAttackTypes()const;

	// 攻撃を受けている状態を取得
	const bool GetHitState()const override;

	// コンボ中の状態かを取得
	bool GetComboState();

	// 攻撃のヒット処理
	void AttackHit(const int damage, const int state)override;

	// 今の状態を取得
	const int GetState()const override { return static_cast<int>(state_); }

	// ダメージ量を取得
	const int GetDamage()const override { return damage_; }

	// 溜め攻撃のカウンタを取得
	float GetChargeCnt()const { return chargeCnt_; };

	// 溜め攻撃のカウンタを設定
	void SetChargeCnt(const float cnt) { chargeCnt_ = cnt; }

	// 溜め攻撃のカウンタを加算
	void AddChargeCnt(const float cnt) { chargeCnt_ += cnt; }

	// 攻撃を入力しているか取得
	const std::map<PlayerState, bool>& GetIsCombo()const { return isCombo_; }

	// 攻撃を入力しているか設定
	void SetIsCombo(int i, bool isCombo)
	{
		isCombo_[static_cast<PlayerState>(i)] = isCombo;
	}

	// 状態遷移
	void ChangeState(const PlayerState state);

private:

	// 入力用のコンポーネント
	std::unique_ptr<InputComponent> inputComponent_;

	// 攻撃中の状態
	const std::vector<PlayerState> attackState_ =
	{
		{PlayerState::ATTACK_JAB},
		{PlayerState::ATTACK_STRAIGHT},
		{PlayerState::ATTACK_HOOK},
		{PlayerState::ATTACK_LEFT_KICK},
		{PlayerState::ATTACK_RIGHT_KICK},
		{PlayerState::ATTACK_UPPER},
		{PlayerState::ATTACK_CHARGE_PUNCH}
	};

	// コンボ中の判定
	const std::vector<PlayerState> comboState_=
	{
		{PlayerState::ATTACK_JAB},
		{ PlayerState::ATTACK_STRAIGHT },
		{ PlayerState::ATTACK_HOOK },
		{ PlayerState::ATTACK_LEFT_KICK },
		{ PlayerState::ATTACK_RIGHT_KICK }
	};

	// 攻撃を受けている状態
	const std::vector<PlayerState> hitState_ =
	{
		{PlayerState::HIT_HEAD},
		{PlayerState::HIT_BODY}
	};

	// 頭にヒットする敵の攻撃
	const std::vector<EnemyState> hitHeadState_ =
	{
		{EnemyState::ATTACK_PUNCH}
	};

	// 体にヒットする敵の攻撃
	const std::vector<EnemyState> hitBodyState_ =
	{
		{EnemyState::ATTACK_KICK}
	};

	// 状態
	PlayerState state_;

	// 攻撃を入力しているか
	std::map<PlayerState, bool> isCombo_;

	// 入力カウンタ
	float acceptCnt_;

	// 溜めパンチのカウンタ
	float chargeCnt_;

	// 状態遷移
	std::unordered_map<PlayerState, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();
	void ChangeJab();
	void ChangeStraight();
	void ChangeHook();
	void ChangeLeftKick();
	void ChangeRightKick();
	void ChangeUpper();
	void ChangeChargePunch();
	void ChangeHitHead();
	void ChangeHitBody();

	// 状態の更新
	std::function<void()> stateUpdate_;
	void UpdateIdle();
	void UpdateRun();
	void UpdateJab();
	void UpdateStraight();
	void UpdateHook();
	void UpdateLeftKick();
	void UpdateRightKick();
	void UpdateUpper();
	void UpdateChargePunch();
	void UpdateHitHead();
	void UpdateHitBody();

	// 機能の初期化
	void InitFunction()override;

	// パラメータの初期化
	void InitParameter()override;

	// アニメーションの初期化
	void InitAnimation()override;

	// 関数ポインタの初期化
	void InitFunctionPointer()override;

	// ImGuiのデバッグ描画の更新
	void UpdateDebugImGui()override;

	// どのヒットアニメーションかチェックする
	virtual void AttackHitCheck(const int state);


};

