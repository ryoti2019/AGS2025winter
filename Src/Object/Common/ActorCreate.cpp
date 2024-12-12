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
#include "../Object/Boss.h"
#include "../Object/Stage.h"
#include "ActorCreate.h"

ActorCreate::ActorCreate()
	:
	AREA1_TOTAL_ENEMYS(10),
	AREA2_TOTAL_ENEMYS(10),
	AREA3_TOTAL_ENEMYS(10),
	AREA4_TOTAL_ENEMYS(10),
	AREA5_TOTAL_ENEMYS(10),
	isCollisionArea1_(false),
	isCollisionArea2_(false),
	deathEnemyCnt_(0)
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
	const auto& stageData = objectData[3]["StageData"];

	// ステージを生成
	actorManager->CreateActor<Stage>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// プレイヤー
	const auto& playerData = objectData[0]["PlayerData"];

	// プレイヤーを生成
	actorManager->CreateActor<Player>(playerData, { -80000.0f,-19500.0f,25900.0f });
	actorManager->ActiveData(ActorType::PLAYER, { -80000.0f,-19500.0f,25900.0f });

	//actorManager->CreateActor<Player>(playerData, { -10800.0f,-19500.0f,-132000.0f });
	//actorManager->ActiveData(ActorType::PLAYER, { -10800.0f,-19500.0f,-132000.0f });

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
	actorManager->CreateActor<Boss>(bossData, { -10800.0f,-10000.0f,-150000.0f });
	//actorManager->ActiveData(ActorType::BOSS, { -10800.0f,-10000.0f,-150000.0f });

}

void ActorCreate::Update()
{

	// エリアの衝突判定の処理
	AreaCollision();

	// エリアの衝突判定をなくす処理
	DeactiveAreaCollision();

	// 敵を何体倒したかカウントする処理
	CountDeathEnemys();

}

void ActorCreate::Draw()
{
	DrawSphere3D({ 9300.0f,-18000.0f,23600.0f }, 10000.0f, 10, 0xff0000, 0xff0000, false);
	DrawSphere3D({ -3100.0f,-18000.0f,-22000.0f }, 10000.0f, 10, 0xff0000, 0xff0000, false);
}

void ActorCreate::AreaCollision()
{

	// 基底クラスから使いたい型へキャストする
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	// NULLチェック
	if (!gameScene) return;

	// アクターマネージャーを取得
	std::shared_ptr<ActorManager> actorManager = gameScene->GetActorManager();

	// プレイヤー
	const auto& players = actorManager->GetActiveActorData().find(ActorType::PLAYER);

	// ステージ
	const auto& stages = actorManager->GetActiveActorData().find(ActorType::STAGE);

	// プレイヤーの中身が入っているか確認
	if (players == actorManager->GetActiveActorData().end()) return;

	// ステージの中身が入っているか確認
	if (stages == actorManager->GetActiveActorData().end()) return;

	for (auto& player : players->second)
	{
		for (auto& stage : stages->second)
		{

			// ステージに情報を変更
			auto stageData = std::static_pointer_cast<Stage>(stage->GetThis());

			// エリア1に衝突しているか判定
			if (!isCollisionArea1_ && HitCheck_Sphere_Sphere(player->GetTransform()->pos, player->GetCollisionData().bodyCollisionRadius, { 9300.0f,-18000.0f,23600.0f }, 10000.0f))
			{

				// エリア1と衝突した
				isCollisionArea1_ = true;

				// エリア1の当たり判定を付ける
				stageData->SetIsArea1Collision(true);

				// 敵を生成
				for (int i = 0; i < AREA1_TOTAL_ENEMYS; i++)
				{
					float x = std::rand() % 10000;
					float z = std::rand() % 10000;
					actorManager->ActiveData(ActorType::ENEMY, { 9300.0f + x,-15000,23600.0f + z });
				}

			}

			// エリア2に衝突しているか判定
			if (!isCollisionArea2_ && HitCheck_Sphere_Sphere(player->GetTransform()->pos, player->GetCollisionData().bodyCollisionRadius, { -3100.0f,-18000.0f,-22000.0f }, 10000.0f))
			{

				// エリア2と衝突した
				isCollisionArea2_ = true;

				// エリア1の当たり判定を付ける
				stageData->SetIsArea2Collision(true);

				// 敵を生成
				for (int i = 0; i < AREA2_TOTAL_ENEMYS; i++)
				{
					float x = std::rand() % 100;
					float z = std::rand() % 100;
					actorManager->ActiveData(ActorType::ENEMY, { -27000.0f + x,-15000,-22000.0f + z });
				}

			}
		}

	}

}

void ActorCreate::DeactiveAreaCollision()
{

	// 基底クラスから使いたい型へキャストする
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	// NULLチェック
	if (!gameScene) return;

	// アクターマネージャーを取得
	std::shared_ptr<ActorManager> actorManager = gameScene->GetActorManager();

	// ステージ
	const auto& stages = actorManager->GetActiveActorData().find(ActorType::STAGE);

	// ステージの中身が入っているか確認
	if (stages == actorManager->GetActiveActorData().end()) return;

	for (auto& stage : stages->second)
	{

		// ステージに情報を変更
		auto stageData = std::static_pointer_cast<Stage>(stage->GetThis());

		// エリア1の敵をすべて倒したら衝突判定を消す
		if (deathEnemyCnt_ == AREA1_TOTAL_ENEMYS)
		{
			stageData->SetIsArea1Collision(false);
			deathEnemyCnt_ = 0;
		}

		// エリア2の敵をすべて倒したら衝突判定を消す
		if (deathEnemyCnt_ == AREA2_TOTAL_ENEMYS)
		{
			stageData->SetIsArea2Collision(false);
			deathEnemyCnt_ = 0;
		}

	}

}

void ActorCreate::CountDeathEnemys()
{

	// 基底クラスから使いたい型へキャストする
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	// NULLチェック
	if (!gameScene) return;

	// アクターマネージャーを取得
	std::shared_ptr<ActorManager> actorManager = gameScene->GetActorManager();

	// 敵
	const auto& enemys = actorManager->GetActiveActorData().find(ActorType::ENEMY);

	// プレイヤーの中身が入っているか確認
	if (enemys == actorManager->GetActiveActorData().end()) return;

	for (auto& enemy : enemys->second)
	{

		// ステージに情報を変更
		auto enemyData = std::static_pointer_cast<Enemy>(enemy->GetThis());

		// 死んだらカウンタを増やす
		if (enemyData->GetHp() <= 0 && enemyData->GetIsAlive())
		{
			deathEnemyCnt_++;
			enemyData->SetIsAlive(false);
		}

	}

}