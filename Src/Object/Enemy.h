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
		HIT,
		HIT_FLY,
		MAX
	};

	// アニメーションコントローラーに渡す引数
	std::string ANIM_DATA_KEY[static_cast<int>(STATE::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN",
		"HIT",
		"HIT_FLY"
	};

	Enemy(const VECTOR& pos, const json& data);

	~Enemy() = default;

	void Init(const VECTOR& pos)override;
	void Update(const float deltaTime)override;

	// 攻撃中か取得
	bool GetAttackState()override;

	// 攻撃のヒット処理
	void AttackHit()override;

	// 攻撃のヒットで飛んでいく処理
	void AttackHitFly()override;

private:

	// 状態
	STATE state_;

	// 状態遷移
	std::unordered_map<STATE, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();
	void ChangeHit();
	void ChangeHitFly();

	// 状態の更新
	std::function<void()> stateUpdate_;
	void UpdateIdle();
	void UpdateRun();
	void UpdateHit();
	void UpdateHitFly();

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

