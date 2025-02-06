#pragma once
#include "../Component/BossAIComponent.h"
#include "EnemyBase.h"
#include "BossState.h"
#include "PlayerState.h"

class Boss : public EnemyBase
{

public:

	// 飛び道具の攻撃開始フレーム
	const float ATTACK_PROJECTILE_START_FRAME;

	// 飛び道具のダメージ量
	const int ATTACK_PROJECTILE_DAMAGE;

	// 飛び道具の攻撃が続く時間
	const float ATTACK_PROJECTILE_COLLISION_TIME;

	// 遠距離攻撃してくる距離
	const float LONG_RANGE_ATTACK_DISTANCE;

	// スーパーアーマーHPの最大値
	const int SUPER_ARMOR_HP;

	// スーパーアーマーHPが回復するまでのクールタイム
	const int SUPER_ARMOR_HP_COOL_TIME;

	// 敵を生成するときのクールタイム
	const float CREATE_ENEMY_COOL_TIME;

	// 敵を生成するフレーム
	const float CREATE_ENEMY_FRAME;

	// アニメーションコントローラーに渡す引数
	std::string ANIM_DATA_KEY[static_cast<int>(BossState::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN",
		"ATTACK_PUNCH",
		"ATTACK_KICK",
		"ATTACK_PROJECTILE",
		"HIT_HEAD",
		"HIT_BODY",
		"HIT_FLY",
		"HIT_FLINCH_UP",
		"HIT_KNOCK_BACK",
		"KIP_UP",
		"JUMP_IDLE",
		"LAND",
		"CALL",
		"DEATH"
	};

	// エフェクトデータ
	enum class EffectData
	{
		NONE,
		PROJECTILE,
		HIT,
		MAX
	};

	Boss(const VECTOR& pos, const json& data);

	~Boss()override = default;

	void Init(const VECTOR& pos)override;
	void Update(const float deltaTime)override;
	void Draw(const float deltaTime)override;

	// 状態遷移
	void ChangeState(const BossState state);

	// 攻撃中か取得
	const bool GetAttackState()const override;

	// 近接攻撃か取得
	const bool GetCloseRangeAttackState()const override;

	// 攻撃種類を取得
	const std::vector<int> GetTotalAttackTypes()const;

	// 攻撃を受けている状態を取得
	const bool GetHitState()const override;

	// スーパーアーマー状態かを取得
	const bool GetIsSuperArmor()const override;

	// 攻撃のヒット処理
	void AttackHit(const int damage, const int state)override;

	// 飛び道具のヒット処理
	void ProjectileHit(const int damage)override;

	// 今の状態を取得
	const int GetState()const override { return static_cast<int>(state_); }

	// ダメージ量を取得
	const int GetDamage()const { return damage_; }

	// 敵を生成したかどうかのフラグを取得
	const bool GetIsCreateEnemy()const { return isCreateEnemy_; }

	// 敵を生成したかどうかのフラグを設定
	void SetIsCreateEnemy(const bool isCreateEnemy) { isCreateEnemy_ = isCreateEnemy; }

	// 敵を生成するときのクールタイムのカウンタ
	const float GetCreateEnemyCoolTimeCnt()const { return createEnemyCoolTimeCnt_; }

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

	// 近接攻撃中の状態
	const std::vector<BossState> closeRangeAttackState_ =
	{
		{BossState::ATTACK_PUNCH},
		{BossState::ATTACK_KICK}
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

	// 頭にヒットするプレイヤーの攻撃
	const std::vector<PlayerState>& hitHeadState_ =
	{
		{PlayerState::ATTACK_JAB},
		{PlayerState::ATTACK_STRAIGHT},
		{PlayerState::ATTACK_HOOK},
	};

	// 体にヒットするプレイヤーの攻撃
	const std::vector<PlayerState>& hitBodyState_ =
	{
		{PlayerState::ATTACK_LEFT_KICK}
	};

	// 吹っ飛ばされるプレイヤーの攻撃
	const std::vector<PlayerState>& hitFlyState_ =
	{
		{PlayerState::ATTACK_RIGHT_KICK},
		{PlayerState::ATTACK_SPECIAL_PUNCH}
	};

	// 上に飛ばされるプレイヤーの攻撃
	const std::vector<PlayerState>& hitFlinchUpState_ =
	{
		{PlayerState::ATTACK_UPPER}
	};

	// 空中に浮き続けるプレイヤーの攻撃
	const std::vector<PlayerState>& hitAirState_ =
	{
		{PlayerState::ATTACK_JAB},
		{PlayerState::ATTACK_STRAIGHT},
		{PlayerState::ATTACK_HOOK},
		{PlayerState::ATTACK_LEFT_KICK}
	};

	// 真っすぐ飛ばされるプレイヤーの攻撃
	const std::vector<PlayerState>& hitKnockBackState_ =
	{
		{PlayerState::ATTACK_CHARGE_PUNCH}
	};

	// その場で死ぬときのプレイヤーの攻撃
	const std::vector<PlayerState>& deathState_ =
	{
		{PlayerState::ATTACK_JAB},
		{PlayerState::ATTACK_STRAIGHT},
		{PlayerState::ATTACK_HOOK},
		{PlayerState::ATTACK_LEFT_KICK},
		{PlayerState::ATTACK_UPPER}
	};

	// 吹っ飛んで死ぬときのプレイヤーの攻撃
	const std::vector<PlayerState>& hitFlyDeathState_ =
	{
		{PlayerState::ATTACK_RIGHT_KICK},
	};

	// 状態
	BossState state_;

	// 飛び道具の飛んでいく方向
	VECTOR projectileDir_;

	// 飛び道具の衝突判定が続く時間のカウンタ
	float projectileCollisionCnt_;

	// スーパーアーマーのHP
	int superArmorHp_;

	// スーパーアーマーが回復中の時の数値
	float superArmorRecoveryHp_;

	// スーパーアーマーが回復するまでのクールタイムのカウンタ
	float superArmorCoolTimeCnt_;

	// 敵を生成するときのクールタイムのカウンタ
	float createEnemyCoolTimeCnt_;

	// 飛び道具の音
	int projectileSE_;
	
	// 敵を生成したかどうかのフラグ
	bool isCreateEnemy_;

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
	void ChangeKipUp();
	void ChangeCall();
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
	void UpdateKipUp(const float deltaTime);
	void UpdateCall(const float deltaTime);
	void UpdateDeath(const float deltaTime);

	// 機能の初期化
	void InitFunction()override;

	// パラメータの初期化
	void InitParameter()override;

	// アニメーションの初期化
	void InitAnimation()override;

	// エフェクトの初期化
	void InitEffect(void)override;

	// BGMとSEの初期化
	void InitBGMAndSE()override;

	// 関数ポインタの初期化
	void InitFunctionPointer()override;

	// ImGuiのデバッグ描画の更新
	void UpdateDebugImGui()override;

	// アニメーションのフレームを固定
	void AnimationFrame()override;

	// どのヒットアニメーションかチェックする
	void AttackHitCheck(const int state);

	// 飛び道具の更新
	void Projectile(const float deltaTime);

	// スーパーアーマーのHPを減らす
	void SubSuperArmorHp(const int superArmorHp);

	// スーパーアーマーのHPを回復するまでのクールタイム
	void ResetSuperArmorCoolTime(const float deltaTime);

	// HPが0になったら死亡アニメーションに遷移
	void DeathAnim(int state)override;

};