#pragma once
#include <DxLib.h>
#include <functional>
#include "ActorBase.h"
#include "EnemyState.h"
#include "Player.h"

class Enemy : public ActorBase
{

public:

	// アニメーションコントローラーに渡す引数
	std::string ANIM_DATA_KEY[static_cast<int>(EnemyState::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN",
		"PUNCH",
		"KICK",
		"HIT_HEAD",
		"HIT_BODY",
		"HIT_FLY",
		"FLINCH_UP",
		"KIP_UP"
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

	// 吹っ飛ぶ時の移動量
	const float HIT_FLY_MOVE_POW;

	// 追いかける最大の時間
	const float TRACKING_MAX_TIME;

	Enemy(const VECTOR& pos, const json& data);

	~Enemy() = default;

	void Init(const VECTOR& pos)override;
	void Update(const float deltaTime)override;
	void Draw()override;

	// 攻撃中か取得
	bool GetAttackState()override;

	// 攻撃種類を取得
	const std::vector<int> GetToatlAttackTypes()const
	{

		std::vector<int> intStates;
		intStates.reserve(attackState_.size());

		 // 変換処理
		for (const auto& state : attackState_)
		{
			intStates.push_back(static_cast<int>(state));
		}

		return intStates; 

	}

	// 攻撃を受けている状態を取得
	bool GetHitState()override;

	// 攻撃のヒット処理
	void AttackHit(const int damage, const int state)override;

	// 今の状態を取得
	int GetState()override { return static_cast<int>(state_); }

	// ダメージ量を取得
	int GetDamage()override { return damage_; }

private:

	// 攻撃中の状態
	const std::vector<EnemyState>& attackState_ =
	{
		{EnemyState::PUNCH},
		{EnemyState::KICK}
	};

	// 攻撃を受けている状態
	const std::vector<EnemyState>& hitState_ =
	{
		{EnemyState::HIT_HEAD},
		{EnemyState::HIT_BODY},
		{EnemyState::HIT_FLY},
		{EnemyState::FLINCH_UP},
		{EnemyState::KIP_UP}
	};

	// 頭にヒットするプレイヤーの攻撃
	const std::vector<PlayerState>& hitHeadState_ =
	{
		{PlayerState::JAB},
		{PlayerState::STRAIGHT},
		{PlayerState::HOOK},
	};

	// 体にヒットするプレイヤーの攻撃
	const std::vector<PlayerState>& hitBodyState_ =
	{
		{PlayerState::LEFT_KICK}
	};

	// 吹っ飛ばされるプレイヤーの攻撃
	const std::vector<PlayerState>& hitFlyState_ =
	{
		{PlayerState::RIGHT_KICK}
	};

	// 上に飛ばされるプレイヤーの攻撃
	const std::vector<PlayerState>& hitFlinchUpState_ =
	{
		{PlayerState::UPPER}
	};

	// 状態
	EnemyState state_;

	// 行動を決めたかどうか
	bool isActionDecided_;

	// クールタイム
	float coolTime_;

	// 追いかけている時間
	float trackingTime_;

	// 状態遷移
	std::unordered_map<EnemyState, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();
	void ChangePunch();
	void ChangeKick();
	void ChangeHitHead();
	void ChangeHitBody();
	void ChangeHitFly();
	void ChangeFlinchUp();
	void ChangeKipUp();

	// 状態の更新
	std::function<void(const float deltaTime)> stateUpdate_;
	void UpdateIdle(const float deltaTime);
	void UpdateRun(const float deltaTime);
	void UpdatePunch(const float deltaTime);
	void UpdateKick(const float deltaTime);
	void UpdateHitHead(const float deltaTime);
	void UpdateHitBody(const float deltaTime);
	void UpdateHitFly(const float deltaTime);
	void UpdateFlinchUp(const float deltaTime);
	void UpdateKipUp(const float deltaTime);

	// 機能の初期化
	void InitFunction()override;

	// パラメータの初期化
	void InitParameter()override;

	// アニメーションの初期化
	void InitAnimation()override;

	// 関数ポインタの初期化
	void InitFunctionPointer()override;

	// 状態遷移
	void ChangeState(EnemyState state);

	// どの行動をするか決める
	void SelsectAction(const float deltaTime);

	// 移動処理
	void Move()override;

	// 攻撃処理
	void Attack(const float deltaTime)override;

	// アニメーションのフレームを固定
	void AnimationFrame()override;

	// プレイヤーの座標を取得
	std::optional<VECTOR> GetPlayerPos();

	// どのヒットアニメーションかチェックする
	void AttackHitCheck(const int state);

};

