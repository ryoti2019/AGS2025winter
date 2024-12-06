#include <fstream>
#include <memory>
#include "../Lib/nlohmann/json.hpp"
#include "../Application.h"
#include "../Component/InputComponent.h"
#include "../Object/Common/Transform.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ActorManager.h"
#include "../Scene/GameScene.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Stage.h"
#include "ActorCreate.h"

ActorCreate::ActorCreate()
	:
	isCollisionArea1_(false),
	isCollisionArea2_(false)
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
	actorManager->CreateActor<Player>(playerData, { -80000.0f,-19500.0f,25900.0f });
	actorManager->ActiveData(ActorType::PLAYER, { -80000.0f,-19500.0f,25900.0f });

	// 敵
	const auto& enemyData = objectData[1]["EnemyData"];

	// 敵を生成
	for (int i = 0; i < 50; i++)
	{
		float x = std::rand() % 10;
		float z = std::rand() % 10;
		actorManager->CreateActor<Enemy>(enemyData, { 0.0f,-1500.0f,0.0f });
	}

}

void ActorCreate::Update()
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

	// 敵
	const auto& enemyData = objectData[1]["EnemyData"];

	// プレイヤー
	const auto& players = actorManager->GetActiveActorData().find(ActorType::PLAYER);

	// プレイヤーの中身が入っているか確認
	if (players == actorManager->GetActiveActorData().end()) return;

	for (auto& player : players->second)
	{

		if (!isCollisionArea1_ && HitCheck_Sphere_Sphere(player->GetTransform()->pos, player->GetCollisionData().bodyCollisionRadius, { 9300.0f,-18000.0f,23600.0f }, 10000.0f))
		{

			// エリア1と衝突した
			isCollisionArea1_ = true;

			// 敵を生成
			for (int i = 0; i < 10; i++)
			{
				float x = std::rand() % 10000;
				float z = std::rand() % 10000;
				actorManager->ActiveData(ActorType::ENEMY, { 9300.0f + x,-15000,23600.0f + z });
			}

		}

		if (!isCollisionArea2_ && HitCheck_Sphere_Sphere(player->GetTransform()->pos, player->GetCollisionData().bodyCollisionRadius, { -3100.0f,-18000.0f,-22000.0f }, 10000.0f))
		{

			// エリア1と衝突した
			isCollisionArea2_ = true;

			// 敵を生成
			for (int i = 0; i < 10; i++)
			{
				float x = std::rand() % 10000;
				float z = std::rand() % 10000;
				actorManager->ActiveData(ActorType::ENEMY, { -27000.0f + x,-15000,-22000.0f + z });
			}

		}

	}

}

void ActorCreate::Draw()
{
	DrawSphere3D({ 9300.0f,-18000.0f,23600.0f }, 10000.0f, 10, 0xff0000, 0xff0000, false);
	DrawSphere3D({ -3100.0f,-18000.0f,-22000.0f }, 10000.0f, 10, 0xff0000, 0xff0000, false);
}
