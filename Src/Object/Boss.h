#pragma once
#include "../Component/BossAIComponent.h"
#include "EnemyBase.h"
#include "BossState.h"

class Boss : public EnemyBase
{

public:

	// アニメーションコントローラーに渡す引数
	std::string ANIM_DATA_KEY[static_cast<int>(BossState::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN",
		"PUNCH",
		"KICK",
		"SONIC_BOOM"
	};

	Boss(const VECTOR& pos, const json& data);

	~Boss() = default;

	void Init(const VECTOR& pos)override;
	void Update(const float deltaTime)override;

	// 状態遷移
	void ChangeState(const BossState state);

	// 攻撃中か取得
	const bool GetAttackState()const override;

	// 攻撃種類を取得
	const std::vector<int> GetTotalAttackTypes()const;

	// 今の状態を取得
	const int GetState()const override { return static_cast<int>(state_); }

private:

	// AIコンポーネント
	std::unique_ptr<BossAIComponent> aiComponent_;

	// 攻撃中の状態
	const std::vector<BossState>& attackState_ =
	{
		{BossState::PUNCH},
		{BossState::KICK}
	};

	// 状態
	BossState state_;

	// 状態遷移
	std::unordered_map<BossState, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();
	void ChangePunch();
	void ChangeKick();
	void ChangeSonicBoom();

	// 状態の更新
	std::function<void(const float deltaTime)> stateUpdate_;
	void UpdateIdle(const float deltaTime);
	void UpdateRun(const float deltaTime);
	void UpdatePunch(const float deltaTime);
	void UpdateKick(const float deltaTime);
	void UpdateSonicBoom(const float deltaTime);

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

	// アニメーションのフレームを固定
	void AnimationFrame()override;

};

