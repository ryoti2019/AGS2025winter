#pragma once
#include <DxLib.h>
#include <functional>
#include "ActorBase.h"
#include "Common/InputController.h"
#include "PlayerState.h"
#include "EnemyState.h"

class Player : public ActorBase
{

public:

	// アニメーションコントローラーに渡す引数
	std::string ANIM_DATA_KEY[static_cast<int>(PlayerState::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN",
		"JAB",
		"STRAIGHT",
		"HOOK",
		"LEFT_KICK",
		"RIGHT_KICK",
		"UPPER",
		"CHARGE_PUNCH",
		"HIT_HEAD",
		"HIT_BODY"
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

	// 溜めパンチを出すためにボタン押す長さ
	const float CHARGE_TIME;

	Player(const VECTOR& pos, const json& data);

	~Player() = default;

	void Init(const VECTOR& pos) override;
	void Update(const float deltaTime) override;

	// 攻撃中の状態かを取得
	bool GetAttackState()override;

	// 攻撃種類を取得
	const std::vector<int> GetToatlAttackTypes()const
	{

		std::vector<int> intStates;
		intStates.reserve(attackState_.size());

		// 変換処理
		for (const auto& state : attackState_)
		{
			intStates.push_back(static_cast<int>(state));
		}

		return intStates;

	}

	// 攻撃を受けている状態を取得
	bool GetHitState()override;

	// コンボ中の状態かを取得
	bool GetComboState();

	// 攻撃のヒット処理
	void AttackHit(const int damage, const int state)override;

	// 今の状態を取得
	int GetState()override { return static_cast<int>(state_); }

	// ダメージ量を取得
	int GetDamage()override { return damage_; }

private:

	// 入力用コントローラー
	std::unique_ptr<InputController> inputController_;

	// 攻撃中の状態
	const std::vector<PlayerState> attackState_ =
	{
		{PlayerState::JAB},
		{PlayerState::STRAIGHT},
		{PlayerState::HOOK},
		{PlayerState::LEFT_KICK},
		{PlayerState::RIGHT_KICK},
		{PlayerState::UPPER},
		{PlayerState::CHARGE_PUNCH}
	};

	// コンボ中の判定
	const std::vector<PlayerState> comboState_=
	{
		{PlayerState::JAB},
		{ PlayerState::STRAIGHT },
		{ PlayerState::HOOK },
		{ PlayerState::LEFT_KICK },
		{ PlayerState::RIGHT_KICK }
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
		{EnemyState::PUNCH}
	};

	// 体にヒットする敵の攻撃
	const std::vector<EnemyState> hitBodyState_ =
	{
		{EnemyState::KICK}
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
	virtual void UpdateDebugImGui();

	// 状態遷移
	void ChangeState(PlayerState state);

	// 移動処理
	void Move()override;

	// 攻撃処理
	void Attack(const float deltaTime)override;

	// 攻撃するときの移動や回転の処理
	void MoveAndRotate();

	// どのヒットアニメーションかチェックする
	virtual void AttackHitCheck(const int state);


};

