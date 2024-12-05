#pragma once
#include <DxLib.h>
#include <functional>
#include "../Component/AIComponent.h"
#include "ActorBase.h"
#include "EnemyState.h"
#include "Player.h"

class AIComponent;

class Enemy : public ActorBase
{

public:

	// アニメーションコントローラーに渡す引数
	std::string ANIM_DATA_KEY[static_cast<int>(EnemyState::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN",
		"ATTACK_PUNCH",
		"ATTACK_KICK",
		"HIT_HEAD",
		"HIT_BODY",
		"HIT_FLY",
		"HIT_FLINCH_UP",
		"HIT_KNOCK_BACK",
		"KIP_UP",
		"DEATH"
	};

	// パンチの攻撃開始フレーム
	const float PUNCH_ATTACK_START_FRAME;

	// パンチの攻撃終了フレーム
	const float PUNCH_ATTACK_END_FRAME;

	// キックの攻撃開始フレーム
	const float KICK_ATTACK_START_FRAME;

	// キックの攻撃終了フレーム
	const float KICK_ATTACK_END_FRAME;

	// クールタイム
	const float COOL_TIME;

	// プレイヤーから一定距離離れたら移動してくる距離
	const float ACTIVATION_DISTANCE;

	// 吹っ飛ぶ時の上方向の力
	const float HIT_FLY_UP_VEC_POW;

	// 吹っ飛ぶ時の移動量
	const float HIT_FLY_MOVE_POW;

	// 追いかける最大の時間
	const float TRACKING_MAX_TIME;

	// まっすぐ飛んでいく時間
	const float KNOCK_BACK_TIME;

	// まっすぐ飛んでいくとき調整する高さ
	const float KNOCK_BACK_HEIGHT_OFFSET;

	// 上に飛んでいくときの上方向の力
	const float FLINCH_UP_UP_VEC_POW;

	// 少し上に飛んでいくときの上方向の力
	const float FLINCH_UP_UP_VEC_SMALL_POW;

	// 上に飛んでいくときのスピード
	const float FLINCH_UP_SPEED;

	// 上に飛んでいくときのX軸の角度
	const float FLINCH_UP_ANGLE_X;

	// 上に飛んでいくときの重力を緩くする強さ
	const float FLINCH_UP_GRAVITY_SCALE;

	// 移動中の動かす割合
	const float MOVE_RATE;

	Enemy(const VECTOR& pos, const json& data);

	~Enemy() = default;

	void Init(const VECTOR& pos)override;
	void Update(const float deltaTime)override;
	void Draw()override;

	// 攻撃中か取得
	const bool GetAttackState()const override;

	// 攻撃種類を取得
	const std::vector<int> GetTotalAttackTypes()const;

	// 攻撃を受けている状態を取得
	const bool GetHitState()const override;

	// 攻撃のヒット処理
	void AttackHit(const int damage, const int state)override;

	// 今の状態を取得
	const int GetState()const override { return static_cast<int>(state_); }

	// ダメージ量を取得
	const int GetDamage()const override { return damage_; }

	// 行動を決めたかどうかを取得
	const bool GetIsActionDecided()const { return isActionDecided_; }

	// 行動を決めたかどうかを設定
	void SetIsActionDecided(const bool isActionDecided) { isActionDecided_ = isActionDecided; }

	// クールタイムを取得
	const float GetCoolTime()const { return coolTime_; }

	// クールタイムを設定
	void SetCoolTime(const float coolTime) { coolTime_ = coolTime; }

	// プレイヤーの座標を取得
	std::optional<VECTOR> GetPlayerPos();

	// 状態遷移
	void ChangeState(EnemyState state);

private:

	// 入力用のコンポーネント
	std::unique_ptr<AIComponent> aiComponent_;

	// 攻撃中の状態
	const std::vector<EnemyState>& attackState_ =
	{
		{EnemyState::ATTACK_PUNCH},
		{EnemyState::ATTACK_KICK}
	};

	// 攻撃を受けている状態
	const std::vector<EnemyState>& hitState_ =
	{
		{EnemyState::HIT_HEAD},
		{EnemyState::HIT_BODY},
		{EnemyState::HIT_FLY},
		{EnemyState::HIT_FLINCH_UP},
		{EnemyState::HIT_KNOCK_BACK},
		{EnemyState::KIP_UP}
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
		{PlayerState::ATTACK_RIGHT_KICK}
	};

	// 状態
	EnemyState state_;

	// 行動を決めたかどうか
	bool isActionDecided_;

	// すでに角度が変わっているかどうか
	bool isChangeAngle_;

	// クールタイム
	float coolTime_;

	// 追いかけている時間
	float trackingTime_;

	// 敵がまっすく飛んでいくときのカウンタ
	float knockBackCnt_;

	// 状態遷移
	std::unordered_map<EnemyState, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();
	void ChangePunch();
	void ChangeKick();
	void ChangeHitHead();
	void ChangeHitBody();
	void ChangeHitFly();
	void ChangeHitFlinchUp();
	void ChangeHitKnockBack();
	void ChangeKipUp();
	void ChangeDeath();

	// 状態の更新
	std::function<void(const float deltaTime)> stateUpdate_;
	void UpdateIdle(const float deltaTime);
	void UpdateRun(const float deltaTime);
	void UpdatePunch(const float deltaTime);
	void UpdateKick(const float deltaTime);
	void UpdateHitHead(const float deltaTime);
	void UpdateHitBody(const float deltaTime);
	void UpdateHitFly(const float deltaTime);
	void UpdateHitFlinchUp(const float deltaTime);
	void UpdateHitKnockBack(const float deltaTime);
	void UpdateKipUp(const float deltaTime);
	void UpdateDeath(const float deltaTime);

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

	// どのヒットアニメーションかチェックする
	void AttackHitCheck(const int state);

	// HPが0になったら死亡アニメーションに遷移
	void DeathAnim(int state)override;

};

