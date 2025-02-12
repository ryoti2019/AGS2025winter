#include <random>
#include "BossAIComponent.h"
#include "../Object/Boss.h"

BossAIComponent::BossAIComponent(const std::shared_ptr<Boss> boss)
{
	boss_ = boss;
	actor_ = boss;
}

void BossAIComponent::Update(const float deltaTime)
{

	// どの行動をするか決める
	if (!boss_->GetIsActionDecided() && boss_->GetCoolTime() <= 0.0f)
	{
		SelectAction(deltaTime);
	}

}

void BossAIComponent::SelectAction(const float deltaTime)
{

	// ヒット中は行動できない
	if (boss_->GetHitState())return;

	// 乱数

	// 非決定的な乱数生成器
	std::random_device rd;

	// メルセンヌ・ツイスタ法による乱数生成器
	std::mt19937 gen(rd());

	// 指定の範囲でランダムな数を取得
	std::uniform_int_distribution<> dist_int(0, static_cast<int>(ACTION_TYPE::MAX) - 1);
	int number = dist_int(gen);

	if (number == static_cast<int>(ACTION_TYPE::MOVE))
	{
		// 移動処理
		Move(deltaTime);
	}
	else if (number == static_cast<int>(ACTION_TYPE::CLOSE_RANGE))
	{
		// 近距離攻撃処理
		CloseRangeAttack(deltaTime);
	}
	else if (number == static_cast<int>(ACTION_TYPE::LONG_RANGE))
	{
		// 遠距離攻撃処理
		LongRangeAttack(deltaTime);
	}
	else if (number == static_cast<int>(ACTION_TYPE::CALL) && boss_->GetCreateEnemyCoolTimeCnt() == 0.0f)
	{

		// 雑魚を呼ぶ行動
		boss_->ChangeState(BossState::CALL);

		// 行動を決めた
		boss_->SetIsActionDecided(true);

	}

}

void BossAIComponent::Move(const float deltaTime)
{

	// プレイヤーの座標
	std::optional<VECTOR> playerPos = boss_->GetPlayerPos();
	boss_->SetTargetPos(playerPos.value());

	// 敵からプレイヤーへのベクトル
	VECTOR vec = VSub(boss_->GetTargetPos(), boss_->GetTransform()->pos);

	// ベクトルの長さ
	float length = Utility::Magnitude(vec);

	// プレイヤーの近くに移動
	if (length >= boss_->ACTIVATION_DISTANCE)
	{

		boss_->ChangeState(BossState::RUN);

		// 行動を決めた
		boss_->SetIsActionDecided(true);

	}

}

void BossAIComponent::CloseRangeAttack(const float deltaTime)
{

	// 乱数

	// 非決定的な乱数生成器
	std::random_device rd;

	// メルセンヌ・ツイスタ法による乱数生成器
	std::mt19937 gen(rd());

	// 指定の範囲でランダムな数を取得
	std::uniform_int_distribution<> dist_int(0, static_cast<int>(CLOSE_RANGE_TYPE::MAX) - 1);
	int number = dist_int(gen);

	// プレイヤーの座標
	std::optional<VECTOR> playerPos = boss_->GetPlayerPos();

	// 相手の座標
	boss_->SetTargetPos(playerPos.value());

	// 敵からプレイヤーへのベクトル
	VECTOR vec = VSub(boss_->GetTargetPos(), boss_->GetTransform()->pos);

	// ベクトルの長さ
	float length = Utility::Magnitude(vec);

	// プレイヤーとの距離が遠かったら攻撃できない
	if (length >= boss_->ACTIVATION_DISTANCE) return;

	// 方向を角度に変換する
	float angle = atan2f(vec.x, vec.z);

	// プレイヤー方向に回転
	boss_->LazyRotation(angle);

	// ラジアンからデグリー
	float goalDeg = Utility::Rad2DegF(angle);

	// 向きたい角度を360度内に収める
	goalDeg = Utility::DegIn360(goalDeg);

	// ボスのラジアン角
	auto rad = boss_->GetTransform()->quaRot.ToEuler();

	// ボスのデグリー角
	auto deg = Utility::Rad2DegF(rad.y);

	// ボスの角度を360度に収める
	deg = Utility::DegIn360(deg);

	// 目的の角度と自分の角度の差を測る
	float sub = goalDeg - deg;

	// 絶対値を求める
	sub = abs(sub);

	// 回転しきるまで処理しない
	if (sub >= ANGLE_TOLERANCE)return;

	if (number == static_cast<int>(CLOSE_RANGE_TYPE::PUNCH))
	{

		// パンチ攻撃に遷移
		boss_->ChangeState(BossState::ATTACK_PUNCH);

		// 行動を決めた
		boss_->SetIsActionDecided(true);

	}
	else if (number == static_cast<int>(CLOSE_RANGE_TYPE::KICK))
	{

		// キック攻撃に遷移
		boss_->ChangeState(BossState::ATTACK_KICK);

		// 行動を決めた
		boss_->SetIsActionDecided(true);

	}

}

void BossAIComponent::LongRangeAttack(const float deltaTime)
{

	// プレイヤーの座標
	std::optional<VECTOR> playerPos = boss_->GetPlayerPos();

	// 相手の座標
	boss_->SetTargetPos(playerPos.value());

	// 敵からプレイヤーへのベクトル
	VECTOR vec = VSub(boss_->GetTargetPos(), boss_->GetTransform()->pos);

	// ベクトルの長さ
	float length = Utility::Magnitude(vec);

	// プレイヤーとの距離が遠かったら攻撃できない
	if (length < boss_->LONG_RANGE_ATTACK_DISTANCE) return;

	// 方向を角度に変換する
	float angle = atan2f(vec.x, vec.z);

	// プレイヤー方向に回転
	boss_->LazyRotation(angle);

	// ラジアンからデグリー
	float goalDeg = Utility::Rad2DegF(angle);

	// 向きたい角度を360度内に収める
	goalDeg = Utility::DegIn360(goalDeg);

	// ボスのラジアン角
	auto rad = boss_->GetTransform()->quaRot.ToEuler();

	// ボスのデグリー角
	auto deg = Utility::Rad2DegF(rad.y);

	// ボスの角度を360度に収める
	deg = Utility::DegIn360(deg);

	// 目的の角度と自分の角度の差を測る
	float sub = goalDeg - deg;

	// 絶対値を求める
	sub = abs(sub);

	// 回転しきるまで処理しない
	if (sub >= ANGLE_TOLERANCE)return;

	// 飛び道具攻撃に遷移
	boss_->ChangeState(BossState::ATTACK_PROJECTILE);

	// 行動を決めた
	boss_->SetIsActionDecided(true);

}
