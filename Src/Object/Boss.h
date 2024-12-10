#pragma once
#include "../Component/BossAIComponent.h"
#include "EnemyBase.h"
#include "BossState.h"

class Boss : public EnemyBase
{

public:

	// 飛び道具の攻撃開始フレーム
	const float ATTACK_PROJECTILE_START_FRAME;

	// 飛び道具のダメージ量
	const int ATTACK_PROJECTILE_DAMAGE;

	// 飛び道具の攻撃が続く時間
	const float ATTACK_PROJECTILE_COLLISION_TIME;

	// アニメーションコントローラーに渡す引数
	std::string ANIM_DATA_KEY[static_cast<int>(BossState::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN",
		"ATTACK_PUNCH",
		"ATTACK_KICK",
		"ATTACK_PROJECTILE"
	};

	enum class EffectData
	{
		NONE,
		PROJECTILE,
		MAX
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

	// 攻撃のヒット処理
	void AttackHit(const int damage, const int state)override;

	// 飛び道具のヒット処理
	void ProjectileHit(const int damage)override;

	// 今の状態を取得
	const int GetState()const override { return static_cast<int>(state_); }

private:

	// AIコンポーネント
	std::unique_ptr<BossAIComponent> aiComponent_;

	// 攻撃中の状態
	const std::vector<BossState>& attackState_ =
	{
		{BossState::ATTACK_PUNCH},
		{BossState::ATTACK_KICK},
		{BossState::ATTACK_PROJECTILE}
	};

	// 攻撃を受けている状態
	const std::vector<BossState>& hitState_ =
	{
		{BossState::HIT_HEAD},
		{BossState::HIT_BODY},
		{BossState::HIT_FLY},
		{BossState::HIT_FLINCH_UP},
		{BossState::HIT_KNOCK_BACK},
		{BossState::KIP_UP}
	};

	// 状態
	BossState state_;

	// 飛び道具の飛んでいく方向
	VECTOR projectileDir_;

	// 飛び道具の衝突判定が続く時間のカウンタ
	float projectileCollisionCnt_;

	// 状態遷移
	std::unordered_map<BossState, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();
	void ChangePunch();
	void ChangeKick();
	void ChangeProjectile();
	void ChangeHitHead();
	void ChangeHitBody();
	void ChangeHitFly();
	void ChangeHitFlinchUp();
	void ChangeHitKnockback();
	void ChangeHitKipUp();
	void ChangeDeath();


	// 状態の更新
	std::function<void(const float deltaTime)> stateUpdate_;
	void UpdateIdle(const float deltaTime);
	void UpdateRun(const float deltaTime);
	void UpdatePunch(const float deltaTime);
	void UpdateKick(const float deltaTime);
	void UpdateProjectile(const float deltaTime);
	void UpdateHitHead(const float deltaTime);
	void UpdateHitBody(const float deltaTime);
	void UpdateHitFly(const float deltaTime);
	void UpdateHitFlinchUp(const float deltaTime);
	void UpdateHitKnockback(const float deltaTime);
	void UpdateHitKipUp(const float deltaTime);
	void UpdateDeath(const float deltaTime);

	// 機能の初期化
	void InitFunction()override;

	// パラメータの初期化
	void InitParameter()override;

	// アニメーションの初期化
	void InitAnimation()override;

	// エフェクトの初期化
	void InitEffect(void)override;

	// 関数ポインタの初期化
	void InitFunctionPointer()override;

	// ImGuiのデバッグ描画の更新
	void UpdateDebugImGui()override;

	// アニメーションのフレームを固定
	void AnimationFrame()override;

	// 飛び道具の更新
	void Projectile(const float deltaTime);

};

