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

	// 吹っ飛ぶ時の上方向の力
	const float HIT_FLY_UP_VEC_POW;

	// 吹っ飛ぶ時の移動量
	const float HIT_FLY_MOVE_POW;

	// 上に飛んでいくときの上方向の力
	const float FLINCH_UP_UP_VEC_POW;

	// 少し上に飛んでいくときの上方向の力
	const float FLINCH_UP_UP_VEC_SMALL_POW;

	// 上に飛んでいくときのスピード
	const float FLINCH_UP_SPEED;

	// 上に飛んでいくときの重力を緩くする強さ
	const float FLINCH_UP_GRAVITY_SCALE;

	// 上に飛んでいくときのX軸の角度
	const float FLINCH_UP_ANGLE_X;

	// まっすぐ飛んでいくとき調整する高さ
	const float KNOCK_BACK_HEIGHT_OFFSET;

	// まっすぐ飛んでいく時間
	const float KNOCK_BACK_TIME;

	// パンチのダメージ量
	int ATTACK_PUNCH_DAMAGE;

	// キックのダメージ量
	int ATTACK_KICK_DAMAGE;

	EnemyBase(const VECTOR& pos, const json& data);

	virtual ~EnemyBase()override = default;

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

	// すでに角度が変わっているかどうか
	bool isChangeAngle_;

	// 敵がまっすく飛んでいくときのカウンタ
	float knockBackCnt_;

private:

};

