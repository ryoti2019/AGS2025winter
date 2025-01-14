#pragma once
#include "EnemyBase.h"
#include "BossState.h"

class BossAppearanceBoss : public EnemyBase
{

public:

	// アニメーションコントローラーに渡す引数
	std::string ANIM_DATA_KEY[static_cast<int>(BossState::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN",
		"JUMP_IDLE",
		"LAND",
		"ATTACK_PUNCH",
		"ATTACK_KICK",
		"ATTACK_PROJECTILE",
		"HIT_HEAD",
		"HIT_BODY",
		"HIT_FLY",
		"HIT_FLINCH_UP",
		"HIT_KNOCK_BACK",
		"KIP_UP",
		"DEATH"
	};

	BossAppearanceBoss(const VECTOR& pos, const json& data);

	~BossAppearanceBoss() = default;

	void Init(const VECTOR& pos)override;
	void Update(const float deltaTime)override;

private:

	// 状態
	BossState state_;

	// 地面についた時のフラグ
	bool isLand_;

	// 状態遷移
	std::unordered_map<BossState, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeJumpIdle();
	void ChangeLand();

	// 状態の更新
	std::function<void(const float deltaTime)> stateUpdate_;
	void UpdateIdle(const float deltaTime);
	void UpdateJumpIdle(const float deltaTime);
	void UpdateLand(const float deltaTime);

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

	// 状態遷移
	void ChangeState(const BossState state);

};

