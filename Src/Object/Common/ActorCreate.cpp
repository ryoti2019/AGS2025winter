#include <fstream>
#include <memory>
#include "../Lib/nlohmann/json.hpp"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ActorManager.h"
#include "../Scene/GameScene.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Stage.h"
#include "ActorCreate.h"

ActorCreate::ActorCreate()
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
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	// NULLチェック
	if (!gameScene) return;

	// アクターマネージャーを取得
	std::shared_ptr<ActorManager> actorManager = gameScene->GetActorManager();

	// ステージ
	const auto& stageData = objectData[2]["StageData"];

	// ステージを生成
	actorManager->CreateActor<Stage>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// プレイヤー
	const auto& playerData = objectData[0]["PlayerData"];

	// プレイヤーを生成
	actorManager->CreateActor<Player>(playerData, { 0.0f,-1960.0f,0.0f });
	actorManager->ActiveData(ActorType::PLAYER, { 0.0f,-1960.0f,0.0f });

	// 敵
	const auto& enemyData = objectData[1]["EnemyData"];

	// 敵を生成
	for (int i = 0; i < 5; i++)
	{
		float x = std::rand() % 10000;
		float z = std::rand() % 10000;
		actorManager->CreateActor<Enemy>(enemyData, { 0.0f,-1500.0f,0.0f });
		actorManager->ActiveData(ActorType::ENEMY, { -5000.0f + x,0.0f,-5000.0f + z });
	}

}
