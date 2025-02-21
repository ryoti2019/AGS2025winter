#include "../Manager/SceneManager.h"
#include "../Manager/ActorManager.h"
#include "../Scene/GameScene.h"
#include "../Scene/BossBattleScene.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(const VECTOR& pos, const json& data)
	:
	ActorBase(pos, data),
	COOL_TIME(data["COOL_TIME"]),
	ACTIVATION_DISTANCE(data["ACTIVATION_DISTANCE"]),
	TRACKING_MAX_TIME(data["TRACKING_MAX_TIME"]),
	HIT_FLY_UP_VEC_POW(data["HIT_FLY_UP_VEC_POW"]),
	HIT_FLY_MOVE_POW(data["HIT_FLY_MOVE_POW"]),
	FLINCH_UP_UP_VEC_POW(data["FLINCH_UP_UP_VEC_POW"]),
	FLINCH_UP_UP_VEC_SMALL_POW(data["FLINCH_UP_UP_VEC_SMALL_POW"]),
	FLINCH_UP_SPEED(data["FLINCH_UP_SPEED"]),
	FLINCH_UP_GRAVITY_SCALE(data["FLINCH_UP_GRAVITY_SCALE"]),
	FLINCH_UP_ANGLE_X(data["FLINCH_UP_ANGLE_X"]),
	KNOCK_BACK_HEIGHT_OFFSET(data["KNOCK_BACK_HEIGHT_OFFSET"]),
	KNOCK_BACK_TIME(data["KNOCK_BACK_TIME"])
{
}

void EnemyBase::Init()
{

	// クールタイム
	coolTime_ = 0.0f;

	// 行動を決めたかどうか
	isActionDecided_ = false;

}

std::optional<VECTOR> EnemyBase::GetPlayerPos()
{

	// ゲームシーンの情報を取得
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	// ボスバトルシーンの情報を取得
	std::shared_ptr<BossBattleScene> bossBattleScene =
		std::dynamic_pointer_cast<BossBattleScene>(SceneManager::GetInstance().GetNowScene());

	// NULLチェック
	if (!gameScene && !bossBattleScene)return std::nullopt;

	// アクターマネージャーを取得
	if (gameScene)
	{

		std::shared_ptr<ActorManager> actorManager = gameScene->GetActorManager();

		// 追従対象
		auto players = actorManager->GetActiveActorData().find(ActorType::PLAYER);

		// プレイヤーの座標を取得
		for (const auto& player : players->second)
		{
			return player->GetTransform()->pos;
		}

	}

	if (bossBattleScene)
	{
		std::shared_ptr<ActorManager> actorManager = bossBattleScene->GetActorManager();

		// 追従対象
		auto players = actorManager->GetActiveActorData().find(ActorType::PLAYER);

		// プレイヤーの座標を取得
		for (const auto& player : players->second)
		{
			return player->GetTransform()->pos;
		}

	}

	return std::nullopt;

}