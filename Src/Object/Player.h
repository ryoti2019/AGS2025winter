#pragma once
#include <DxLib.h>
#include <functional>
#include "ActorBase.h"
#include "Common/InputController.h"

class Player : public ActorBase
{

public:

	// コンボの受付時間
	const float COMBO_MAX_TIME;


	enum class STATE
	{
		NONE,
		IDLE,
		RUN,
		JAB,
		STRAIGHT,
		MAX
	};

	// アニメーションコントローラーに渡す引数
	std::string ANIM_DATA_KEY[static_cast<int>(STATE::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN",
		"JAB",
		"STRAIGHT"
	};

	Player(const VECTOR& pos);

	~Player() = default;

	void Init(const VECTOR& pos) override;
	void Update(const float deltaTime) override;

private:

	// 入力用コントローラー
	std::unique_ptr<InputController> inputController_;

	// 状態
	STATE state_;

	// コンボカウンタ
	float comboCnt_;

	// 攻撃1段階目
	bool attack_;

	// 攻撃2段階目
	bool attack2_;

	// 状態遷移
	std::unordered_map<STATE, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();
	void ChangeJab();
	void ChangeStraight();

	// 状態の更新
	std::function<void()> stateUpdate_;
	void UpdateIdle();
	void UpdateRun();
	void UpdateJab();
	void UpdateStraight();

	// 状態の描画
	std::function<void()> stateDraw_;
	void DrawIdle();
	void DrawRun();
	void DrawJab();
	void DrawStraight();

	// 機能の初期化
	void InitFunction()override;

	// パラメータの初期化
	void InitParameter()override;

	// アニメーションの初期化
	void InitAnimation()override;

	// 関数ポインタの初期化
	void InitFunctionPointer()override;

	// 状態遷移
	void ChangeState(STATE state);

	// 移動処理
	void Move()override;

	// 攻撃処理
	void Attack(const float deltaTime)override;

};

