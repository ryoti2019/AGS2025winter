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
		"UPPER"
	};

	// ジャブを打っている間に動き始めるフレーム数
	const float JAB_MOVE_START_FRAME;

	// ジャブを打っている間に動き終わるフレーム数
	const float JAB_MOVE_END_FRAME;

	// ストレートを打っている間に動き始めるフレーム数
	const float STRAIGHT_MOVE_START_FRAME;

	// ストレートを打っている間に動き終わるフレーム数
	const float STRAIGHT_MOVE_END_FRAME;

	// フックを打っている間に動き始めるフレーム数
	const float HOOK_MOVE_START_FRAME;

	// フックを打っている間に動き終わるフレーム数
	const float HOOK_MOVE_END_FRAME;

	// 左キックを打っている間に動き始めるフレーム数
	const float LEFT_KICK_MOVE_START_FRAME;

	// 左キックを打っている間に動き終わるフレーム数
	const float LEFT_KICK_MOVE_END_FRAME;

	// 右キックを打っている間に動き始めるフレーム数
	const float RIGHT_KICK_MOVE_START_FRAME;

	// 右キックを打っている間に動き終わるフレーム数
	const float RIGHT_KICK_MOVE_END_FRAME;

	Player(const VECTOR& pos, const json& data);

	~Player() = default;

	void Init(const VECTOR& pos) override;
	void Update(const float deltaTime) override;

	// 攻撃中の状態かを取得
	bool GetAttackState()override;

	// コンボ中の状態かを取得
	bool GetComboState();

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
	void ComboAttack(const float deltaTime)override;

};

