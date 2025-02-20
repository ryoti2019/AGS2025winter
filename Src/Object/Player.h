#pragma once
#include <functional>
#include <DxLib.h>
#include "../Component/InputComponent.h"
#include "../Manager/CollisionTypes.h"
#include "ActorBase.h"
#include "PlayerState.h"
#include "EnemyState.h"
#include "BossState.h"

class InputComponent;

class Player : public ActorBase
{

public:

	// ボス戦の初期角度
	static constexpr float BOSS_BATTLE_INIT_ANGLE = 180.0f;

	// サウンドの最大音量
	static constexpr int SOUND_MAX = 255;

	// 足音の音量の割合
	static constexpr float SOUND_FOOT_STEPS_VOLUME = 0.6f;


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

	// エフェクトデータ
	enum class EffectData
	{
		NONE,
		HIT,
		SPECIAL_ATTACK,
		SPECIAL_ATTACK2,
		POWER_CHARGE,
		MAX
	};

	// ジャブの攻撃開始フレーム
	const float ATTACK_JAB_START_FRAME;

	// ジャブの攻撃終了フレーム
	const float ATTACK_JAB_END_FRAME;

	// ジャブのダメージ量
	const int ATTACK_JAB_DAMAGE;

	// ストレートの攻撃開始フレーム
	const float ATTACK_STRAIGHT_START_FRAME;

	// ストレートの攻撃終了フレーム
	const float ATTACK_STRAIGHT_END_FRAME;

	// ストレートのダメージ量
	const int ATTACK_STRAIGHT_DAMAGE;

	// フックの攻撃開始フレーム
	const float ATTACK_HOOK_START_FRAME;

	// フックの攻撃終了フレーム
	const float ATTACK_HOOK_END_FRAME;

	// フックのダメージ量
	const int ATTACK_HOOK_DAMAGE;

	// 左キックの攻撃開始フレーム
	const float ATTACK_LEFT_KICK_START_FRAME;

	// 左キックの攻撃終了フレーム
	const float ATTACK_LEFT_KICK_END_FRAME;

	// 左キックのダメージ量
	const int ATTACK_LEFT_KICK_DAMAGE;

	// 右キックの攻撃開始フレーム
	const float ATTACK_RIGHT_KICK_START_FRAME;

	// 右キックの攻撃終了フレーム
	const float ATTACK_RIGHT_KICK_END_FRAME;

	// 右キックのダメージ量
	const int ATTACK_RIGHT_KICK_DAMAGE;

	// アッパーの攻撃開始フレーム
	const float ATTACK_UPPER_START_FRAME;

	// アッパーの攻撃終了フレーム
	const float ATTACK_UPPER_END_FRAME;

	// アッパーのダメージ量
	const int ATTACK_UPPER_DAMAGE;

	// 溜めパンチの攻撃開始フレーム
	const float ATTACK_CHARGE_PUNCH_START_FRAME;

	// 溜めパンチの攻撃終了フレーム
	const float ATTACK_CHARGE_PUNCH_END_FRAME;

	// 溜めパンチのダメージ量
	const int ATTACK_CHARGE_PUNCH_DAMAGE;

	// 溜めパンチを出すためにボタン押す長さ
	const float ATTACK_CHARGE_PUNCH_TIME;

	// 必殺技の攻撃開始フレーム
	const float ATTACK_SPECIAL_PUNCH_START_FRAME;

	// 必殺技のダメージ量
	const int ATTACK_SPECIAL_PUNCH_DAMAGE;

	// 必殺技の攻撃が続く時間
	const float ATTACK_SPECIAL_PUNCH_COLLISION_TIME;

	// 回避の移動量
	const float EVASION_MOVE_POW;

	Player(const VECTOR& pos, const json& data);

	~Player()override = default;

	void Init(const VECTOR& pos) override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime)override;

	// 攻撃中の状態か取得
	const bool GetAttackState()const override;

	// 近接攻撃か取得
	const bool GetCloseRangeAttackState()const override;

	// 攻撃種類を取得
	const std::vector<int> GetTotalAttackTypes()const;

	// 攻撃を受けている状態かを取得
	const bool GetHitState()const override;

	// スーパーアーマー状態かを取得
	const bool GetIsSuperArmor()const override;

	// コンボ中の状態かを取得
	bool GetComboState();

	// 攻撃のヒット処理
	void AttackHit(const int damage, const int state)override;

	// 飛び道具のヒット処理
	void ProjectileHit(const int damage)override;

	// 今の状態を取得
	const int GetState()const override { return static_cast<int>(state_); }

	// ダメージ量を取得
	const int GetDamage()const override { return damage_; }

	// 溜め攻撃のカウンタを取得
	float GetChargeCnt()const { return attackChargePunchCnt_; };

	// 溜め攻撃のカウンタを設定
	void SetChargeCnt(const float cnt) { attackChargePunchCnt_ = cnt; }

	// 溜め攻撃のカウンタを加算
	void AddChargeCnt(const float cnt) { attackChargePunchCnt_ += cnt; }

	// 攻撃を入力しているか取得
	const std::map<PlayerState, bool>& GetIsCombo()const { return isCombo_; }

	// 攻撃を入力しているか設定
	void SetIsCombo(int i, bool isCombo)
	{
		isCombo_[static_cast<PlayerState>(i)] = isCombo;
	}

	// 状態遷移
	void ChangeState(const PlayerState state);

	// 必殺技が当たった時の敵の座標を取得
	const VECTOR GetSpecialAttackHitEnemyPos()const { return specialAttackHitEnemyPos_; }

	// 必殺技が当たった時の敵の座標を設定
	void SetSpecialAttackHitEnemyPos(const VECTOR specialAttackHitEnemyPos) { specialAttackHitEnemyPos_ = specialAttackHitEnemyPos; }

private:

	// 入力用のコンポーネント
	std::unique_ptr<InputComponent> inputComponent_;

	// 入力用コントローラー
	std::unique_ptr<InputController> inputController_;

	// 攻撃中の状態
	const std::vector<PlayerState> attackState_ =
	{
		{PlayerState::ATTACK_JAB},
		{PlayerState::ATTACK_STRAIGHT},
		{PlayerState::ATTACK_HOOK},
		{PlayerState::ATTACK_LEFT_KICK},
		{PlayerState::ATTACK_RIGHT_KICK},
		{PlayerState::ATTACK_UPPER},
		{PlayerState::ATTACK_CHARGE_PUNCH},
		{PlayerState::ATTACK_SPECIAL_PUNCH}
	};

	// 近接攻撃中の状態
	const std::vector<PlayerState> closeRangeAttackState_ =
	{
		{PlayerState::ATTACK_JAB},
		{PlayerState::ATTACK_STRAIGHT},
		{PlayerState::ATTACK_HOOK},
		{PlayerState::ATTACK_LEFT_KICK},
		{PlayerState::ATTACK_RIGHT_KICK},
		{PlayerState::ATTACK_UPPER},
		{PlayerState::ATTACK_CHARGE_PUNCH}
	};

	// コンボ中の判定
	const std::vector<PlayerState> comboState_ =
	{
		{PlayerState::ATTACK_JAB},
		{PlayerState::ATTACK_STRAIGHT},
		{PlayerState::ATTACK_HOOK},
		{PlayerState::ATTACK_LEFT_KICK},
		{PlayerState::ATTACK_RIGHT_KICK},
	};

	// 攻撃を受けている状態
	const std::vector<PlayerState> hitState_ =
	{
		{PlayerState::HIT_HEAD},
		{PlayerState::HIT_BODY}
	};

	// スーパーアーマー状態
	const std::vector<PlayerState> superArmorState_ =
	{
		{PlayerState::EVASION},
		{PlayerState::POWER_CHARGE},
		{PlayerState::ATTACK_SPECIAL_PUNCH}
	};

	// 頭にヒットする敵の攻撃
	const std::vector<EnemyState> hitHeadEnemyState_ =
	{
		{EnemyState::ATTACK_PUNCH}
	};

	// 頭にヒットするボスの攻撃
	const std::vector<BossState> hitHeadBossState_ =
	{
		{BossState::ATTACK_PUNCH}
	};

	// 体にヒットする敵の攻撃
	const std::vector<EnemyState> hitBodyEnemyState_ =
	{
		{EnemyState::ATTACK_KICK}
	};

	// 体にヒットするボスの攻撃
	const std::vector<BossState> hitBodyBossState_ =
	{
		{BossState::ATTACK_KICK}
	};

	// その場で死ぬときの敵の攻撃
	const std::vector<EnemyState> deathEnemyState_ =
	{
		{EnemyState::ATTACK_PUNCH},
		{EnemyState::ATTACK_KICK}
	};

	// その場で死ぬときのボスの攻撃
	const std::vector<BossState> deathBossState_ =
	{
		{BossState::ATTACK_PUNCH},
		{BossState::ATTACK_KICK},
		{BossState::ATTACK_PROJECTILE}
	};

	// 状態
	PlayerState state_;

	// 攻撃を入力しているか
	std::map<PlayerState, bool> isCombo_;

	// 必殺技が当たった時の敵の座標
	VECTOR specialAttackHitEnemyPos_;

	// 溜めパンチのカウンタ
	float attackChargePunchCnt_;

	// 必殺技の衝突判定が続く時間のカウンタ
	float attackSpecialPunchCollisionCnt_;

	// カメラの角度
	float cameraAngleY_;

	// アイコンの画像
	int iconImg_;

	// HPバーの画像
	int hpBarImg_;

	// 足音
	int footStepsSE_;

	// ジャブの音
	int jabSE_;

	// ストレートの音
	int straightSE_;

	// フックの音
	int hookSE_;

	// アッパーの音
	int upperSE_;

	// ため攻撃の音
	int chargePunchSE_;

	// 左キックの音
	int leftKickSE_;

	// 右キックの音
	int rightKickSE_;

	// 必殺技の音
	int specialAttackSE_;

	// 状態遷移
	std::unordered_map<PlayerState, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();
	void ChangeJab();
	void ChangeStraight();
	void ChangeHook();
	void ChangeLeftKick();
	void ChangeRightKick();
	void ChangeUpper();
	void ChangeChargePunch();
	void ChangeSpecialAttack();
	void ChangePowerCharge();
	void ChangeEvasion();
	void ChangeHitHead();
	void ChangeHitBody();
	void ChangeDeath();

	// 状態の更新
	std::function<void(const float deltaTime)> stateUpdate_;
	void UpdateIdle(const float deltaTime);
	void UpdateRun(const float deltaTime);
	void UpdateJab(const float deltaTime);
	void UpdateStraight(const float deltaTime);
	void UpdateHook(const float deltaTime);
	void UpdateLeftKick(const float deltaTime);
	void UpdateRightKick(const float deltaTime);
	void UpdateUpper(const float deltaTime);
	void UpdateChargePunch(const float deltaTime);
	void UpdateSpecialAttack(const float deltaTime);
	void UpdatePowerCharge(const float deltaTime);
	void UpdateEvasion(const float deltaTime);
	void UpdateHitHead(const float deltaTime);
	void UpdateHitBody(const float deltaTime);
	void UpdateDeath(const float deltaTime);

	// 機能の初期化
	void InitComponent()override;

	// パラメータの初期化
	void InitParameter()override;

	// 画像の初期化
	void InitImage()override;

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
	virtual void AttackHitCheck(const int type);

	// 回転処理
	void Rotation();

	// HPが0になったら死亡アニメーションに遷移
	void DeathAnim(int state)override;

};

