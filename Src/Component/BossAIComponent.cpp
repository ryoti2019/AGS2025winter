#include <random>
#include "BossAIComponent.h"
#include "../Object/Boss.h"

BossAIComponent::BossAIComponent(const std::shared_ptr<Boss> boss)
{
	boss_ = boss;
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
	std::uniform_int_distribution<> dist_int(0, 2);
	int number = dist_int(gen);

	if (number == 0)
	{
		// 移動処理
		Move(deltaTime);
	}
	else if (number == 1)
	{
		// 近距離攻撃処理
		CloseRangeAttack(deltaTime);
	}
	else if (number == 2)
	{
		// 遠距離攻撃処理
		LongRangeAttack(deltaTime);
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
	std::uniform_int_distribution<> dist_int(0, 1);
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

	if (number == 0)
	{

		// パンチ攻撃に遷移
		boss_->ChangeState(BossState::ATTACK_PUNCH);

		// 行動を決めた
		boss_->SetIsActionDecided(true);

	}
	else if (number == 1)
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

	// 飛び道具攻撃に遷移
	boss_->ChangeState(BossState::ATTACK_PROJECTILE);

	// 行動を決めた
	boss_->SetIsActionDecided(true);

}
