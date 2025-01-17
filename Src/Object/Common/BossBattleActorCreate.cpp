#include <fstream>
#include "../Lib/nlohmann/json.hpp"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ActorManager.h"
#include "../Scene/BossBattleScene.h"
#include "BossBattleActorCreate.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Boss.h"
#include "../Object/Stage.h"
#include "../Object/StageCollision.h"
#include "../Object/Area5Collision.h"

BossBattleActorCreate::BossBattleActorCreate()
{

	// 外部ファイルの読み込み
	std::ifstream ifs;

	ifs.open(Application::PATH_JSON + "ObjectData.json");

	if (!ifs)
	{
		// 外部ファイルの読み込み失敗
		return;
	}

	// ファイルストリームからjsonオブジェクトに変換
	nlohmann::json objectJson = nlohmann::json::parse(ifs);

	const auto& objectData = objectJson["ObjectData"];

	// スコープが切れる際に 自動的にファイルクローズして貰えますが、
	// お行儀良く、明示的にファイルストリームを閉じる
	ifs.close();

	// 基底クラスから使いたい型へキャストする
	std::shared_ptr<BossBattleScene> bossBattleScene =
		std::dynamic_pointer_cast<BossBattleScene>(SceneManager::GetInstance().GetNowScene());

	// NULLチェック
	if (!bossBattleScene) return;

	// アクターマネージャーを取得
	std::shared_ptr<ActorManager> actorManager = bossBattleScene->GetActorManager();

	// ステージ
	const auto& stageData = objectData[3]["StageData"];

	// ステージを生成
	actorManager->CreateActor<Stage>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// ステージの当たり判定を生成
	actorManager->CreateActor<StageCollision>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// ステージの当たり判定を生成
	actorManager->CreateActor<Area5Collision>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// 基底クラスから使いたい型へキャストする
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	// ステージ
	const auto& stages = actorManager->GetActiveActorData().find(ActorType::STAGE);

	// ステージの中身が入っているか確認
	if (stages == actorManager->GetActiveActorData().end()) return;

	for (auto& stage : stages->second)
	{
		// Area5Collisionにキャスト
		auto area5Collision = std::dynamic_pointer_cast<Area5Collision>(stage);

		// エリア5に当たり判定をつける
		if (area5Collision)
		{
			area5Collision->SetIsCollision(true);
		}

	}

	// プレイヤー
	const auto& playerData = objectData[0]["PlayerData"];

	// プレイヤーを生成
	actorManager->CreateActor<Player>(playerData, { -10800.0f,-19500.0f,-120000.0f });
	actorManager->ActiveData(ActorType::PLAYER, { -10800.0f,-19500.0f,-120000.0f });

	// 敵
	const auto& enemyData = objectData[1]["EnemyData"];

	// 敵を生成
	for (int i = 0; i < 50; i++)
	{
		float x = std::rand() % 10;
		float z = std::rand() % 10;
		actorManager->CreateActor<Enemy>(enemyData, { 0.0f,0.0f,0.0f });
	}

	// ボス
	const auto& bossData = objectData[2]["BossData"];

	// ボスを生成
	actorManager->CreateActor<Boss>(bossData, { -10800.0f,-19500.0f,-140000.0f });
	actorManager->ActiveData(ActorType::BOSS, { -10800.0f,-19500.0f,-140000.0f });

}

void BossBattleActorCreate::Update()
{
}

void BossBattleActorCreate::Draw()
{
}
