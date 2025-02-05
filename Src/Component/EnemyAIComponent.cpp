#include <random>
#include "EnemyAIComponent.h"
#include "../Object/Enemy.h"

EnemyAIComponent::EnemyAIComponent(const std::shared_ptr<Enemy> enemy)
{
	enemy_ = enemy;
	actor_ = enemy;
}

void EnemyAIComponent::Update(const float deltaTime)
{

	// どの行動をするか決める
	if (!enemy_->GetIsActionDecided() && enemy_->GetCoolTime() <= 0.0f)
	{
		SelectAction(deltaTime);
	}

}

void EnemyAIComponent::SelectAction(const float deltaTime)
{

	// ヒット中は行動できない
	if (enemy_->GetHitState())return;

	// 乱数

	// 非決定的な乱数生成器
	std::random_device rd;

	// メルセンヌ・ツイスタ法による乱数生成器
	std::mt19937 gen(rd());

	// 指定の範囲でランダムな数を取得
	std::uniform_int_distribution<> dist_int(0, 1);
	int number = dist_int(gen);

	if (number == 0)
	{
		// 移動処理
		Move(deltaTime);
	}
	else if (number == 1)
	{
		//攻撃処理
		Attack(deltaTime);
	}

}

void EnemyAIComponent::Move(const float deltaTime)
{

	// プレイヤーの座標
	std::optional<VECTOR> playerPos = enemy_->GetPlayerPos();
	enemy_->SetTargetPos(playerPos.value());

	// 敵からプレイヤーへのベクトル
	VECTOR vec = VSub(enemy_->GetTargetPos(), enemy_->GetTransform()->pos);

	// ベクトルの長さ
	float length = Utility::Magnitude(vec);

	// プレイヤーの近くに移動
	if (length >= enemy_->ACTIVATION_DISTANCE)
	{

		enemy_->ChangeState(EnemyState::RUN);

		// 行動を決めた
		enemy_->SetIsActionDecided(true);

	}

}

void EnemyAIComponent::Attack(const float deltaTime)
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
	std::optional<VECTOR> playerPos = enemy_->GetPlayerPos();

	// 相手の座標
	enemy_->SetTargetPos(playerPos.value());

	// 敵からプレイヤーへのベクトル
	VECTOR vec = VSub(enemy_->GetTargetPos(), enemy_->GetTransform()->pos);

	// ベクトルの長さ
	float length = Utility::Magnitude(vec);

	// プレイヤーとの距離が遠かったら攻撃できない
	if (length >= enemy_->ACTIVATION_DISTANCE) return;

	if (number == 0)
	{

		enemy_->ChangeState(EnemyState::ATTACK_PUNCH);

		// 行動を決めた
		enemy_->SetIsActionDecided(true);

	}
	else if (number == 1)
	{

		enemy_->ChangeState(EnemyState::ATTACK_KICK);

		// 行動を決めた
		enemy_->SetIsActionDecided(true);

	}

}