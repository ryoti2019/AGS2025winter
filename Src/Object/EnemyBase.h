#pragma once
#include "ActorBase.h"

class EnemyBase : public ActorBase
{

public:

	// パンチの攻撃開始フレーム
	float PUNCH_ATTACK_START_FRAME;

	// パンチの攻撃終了フレーム
	float PUNCH_ATTACK_END_FRAME;

	// キックの攻撃開始フレーム
	float KICK_ATTACK_START_FRAME;

	// キックの攻撃終了フレーム
	float KICK_ATTACK_END_FRAME;

	// クールタイム
	const float COOL_TIME;

	// プレイヤーから一定距離離れたら移動してくる距離
	const float ACTIVATION_DISTANCE;

	// 追いかける最大の時間
	const float TRACKING_MAX_TIME;

	EnemyBase(const VECTOR& pos, const json& data);

	virtual ~EnemyBase() = default;

	void Init();

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

protected:

	// 行動を決めたかどうか
	bool isActionDecided_;

	// クールタイム
	float coolTime_;

	// 追いかけている時間
	float trackingTime_;

private:

};

