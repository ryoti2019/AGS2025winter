#pragma once
#include <DxLib.h>
#include <functional>
#include "ActorBase.h"

class Enemy : public ActorBase
{

public:

	enum class STATE
	{
		NONE,
		IDLE,
		RUN,
		MAX
	};

	// アニメーションコントローラーに渡す引数
	std::string ANIM_DATA_KEY[static_cast<int>(STATE::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN"
	};

	Enemy(const VECTOR& pos);

	~Enemy() = default;

	void Init(const VECTOR& pos)override;
	void Update(const float deltaTime)override;

private:

	// 状態
	STATE state_;

	// 状態遷移
	std::unordered_map<STATE, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();

	// 状態の更新
	std::function<void()> stateUpdate_;
	void UpdateIdle();
	void UpdateRun();

	// 状態の描画
	std::function<void()> stateDraw_;
	void DrawIdle();
	void DrawRun();

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
	void ComboAttack(const float deltaTime)override;

	// 攻撃状態
	bool AttackState()override;

};

