#pragma once
#include <DxLib.h>
#include <functional>
#include "ActorBase.h"
#include "Common/InputController.h"

class Player : public ActorBase
{

public:

	enum class STATE
	{
		NONE,
		IDLE,
		RUN,
		JAB,
		STRAIGHT,
		HOOK,
		LEFT_KICK,
		RIGHT_KICK,
		UPPER,
		HIT,
		MAX
	};

	enum class ATTACK_STATE
	{
		JAB,
		STRAIGHT,
		HOOK,
		LEFT_KICK,
		RIGHT_KICK,
		UPPER
	};

	// アニメーションコントローラーに渡す引数
	std::string ANIM_DATA_KEY[static_cast<int>(STATE::MAX)] =
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
		"HIT"
	};

	// ジャブの攻撃開始フレーム
	const float JAB_ATTACK_START_FRAME;

	// ジャブの攻撃終了フレーム
	const float JAB_ATTACK_END_FRAME;

	// ストレートの攻撃開始フレーム
	const float STRAIGHT_ATTACK_START_FRAME;

	// ストレートの攻撃終了フレーム
	const float STRAIGHT_ATTACK_END_FRAME;

	// フックの攻撃開始フレーム
	const float HOOK_ATTACK_START_FRAME;

	// フックの攻撃終了フレーム
	const float HOOK_ATTACK_END_FRAME;

	// 左キックの攻撃開始フレーム
	const float LEFT_KICK_ATTACK_START_FRAME;

	// 左キックの攻撃終了フレーム
	const float LEFT_KICK_ATTACK_END_FRAME;

	// 右キックの攻撃開始フレーム
	const float RIGHT_KICK_ATTACK_START_FRAME;

	// 右キックの攻撃終了フレーム
	const float RIGHT_KICK_ATTACK_END_FRAME;

	// アッパーの攻撃開始フレーム
	const float UPPER_ATTACK_START_FRAME;

	// アッパーの攻撃終了フレーム
	const float UPPER_ATTACK_END_FRAME;

	Player(const VECTOR& pos, const json& data);

	~Player() = default;

	void Init(const VECTOR& pos) override;
	void Update(const float deltaTime) override;

	// 攻撃中の状態かを取得
	bool GetAttackState()override;

	// 攻撃を受けている状態を取得
	bool GetHitState()override;

	// コンボ中の状態かを取得
	bool GetComboState();

	// 攻撃のヒット処理
	void AttackHit()override;

	// 攻撃のヒットで飛んでいく処理
	void AttackHitFly()override;

private:

	// 入力用コントローラー
	std::unique_ptr<InputController> inputController_;

	// 攻撃中の状態
	const std::vector<STATE> attackState_ =
	{
		{STATE::JAB},
		{STATE::STRAIGHT},
		{STATE::HOOK},
		{STATE::LEFT_KICK},
		{STATE::RIGHT_KICK},
		{STATE::UPPER}
	};

	// コンボ中の判定
	const std::vector<STATE> comboState_=
	{
		{STATE::JAB},
		{ STATE::STRAIGHT },
		{ STATE::HOOK },
		{ STATE::LEFT_KICK },
		{ STATE::RIGHT_KICK }
	};

	// 攻撃を受けている状態
	const std::vector<STATE> hitState_ =
	{
		{STATE::HIT}
	};

	// 状態
	STATE state_;

	// 攻撃を入力しているか
	std::map<STATE, bool> isCombo_;

	// 入力カウンタ
	float acceptCnt_;

	// 状態遷移
	std::unordered_map<STATE, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();
	void ChangeJab();
	void ChangeStraight();
	void ChangeHook();
	void ChangeLeftKick();
	void ChangeRightKick();
	void ChangeUpper();
	void ChangeHit();

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
	void UpdateHit();

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
	void ChangeState(STATE state);

	// 移動処理
	void Move()override;

	// 攻撃処理
	void Attack()override;

};

