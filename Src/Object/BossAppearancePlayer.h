#pragma once
#include "ActorBase.h"
#include "PlayerState.h"

class BossAppearancePlayer :public ActorBase
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
		"ATTACK_SPECIAL_PUNCH",
		"POWER_CHARGE",
		"EVASION",
		"HIT_HEAD",
		"HIT_BODY",
		"DEATH",
		"TITLE_IDLE",
		"VICTORY"
	};

	BossAppearancePlayer(const VECTOR& pos, const json& data);

	~BossAppearancePlayer()override = default;

	void Init(const VECTOR& pos)override;
	void Update(const float deltaTime)override;

private:

	// 状態
	PlayerState state_;

	// 機能の初期化
	void InitFunction()override;

	// パラメータの初期化
	void InitParameter()override;

	// アニメーションの初期化
	void InitAnimation()override;

	// ImGuiのデバッグ描画の更新
	void UpdateDebugImGui()override;
	
	// 状態遷移
	void ChangeState(const PlayerState state);

};

