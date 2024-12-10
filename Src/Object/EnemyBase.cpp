#include "../Manager/SceneManager.h"
#include "../Manager/ActorManager.h"
#include "../Scene/GameScene.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(const VECTOR& pos, const json& data)
	:
	ActorBase(pos, data),
	COOL_TIME(data["COOL_TIME"]),
	ACTIVATION_DISTANCE(data["ACTIVATION_DISTANCE"]),
	TRACKING_MAX_TIME(data["TRACKING_MAX_TIME"]),
	HIT_FLY_UP_VEC_POW(data["TRACKING_MAX_TIME"]),
	HIT_FLY_MOVE_POW(data["TRACKING_MAX_TIME"]),
	FLINCH_UP_UP_VEC_POW(data["TRACKING_MAX_TIME"]),
	FLINCH_UP_UP_VEC_SMALL_POW(data["TRACKING_MAX_TIME"]),
	FLINCH_UP_SPEED(data["TRACKING_MAX_TIME"]),
	FLINCH_UP_GRAVITY_SCALE(data["TRACKING_MAX_TIME"]),
	FLINCH_UP_ANGLE_X(data["TRACKING_MAX_TIME"]),
	KNOCK_BACK_HEIGHT_OFFSET(data["TRACKING_MAX_TIME"]),
	KNOCK_BACK_TIME(data["TRACKING_MAX_TIME"])
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
	// ゲームシーンの情報を持ってくる
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	// NULLチェック
	if (!gameScene)
	{
		return std::nullopt;
	}

	// アクターマネージャーを取得
	auto actorManager = gameScene->GetActorManager();

	// 追従対象
	auto players = actorManager->GetActiveActorData().find(ActorType::PLAYER);

	// プレイヤーの座標を取得
	for (const auto& player : players->second)
	{
		return player->GetTransform()->pos;
	}
}
