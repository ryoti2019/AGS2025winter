#include <fstream>
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
#include "../Object/StageCollision.h"
#include "../Object/Area1Collision.h"
#include "../Object/Area2Collision.h"
#include "../Object/Area3Collision.h"
#include "../Object/Area4Collision.h"
#include "../Object/Area5Collision.h"
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
	isCollisionArea3_(false),
	isCollisionArea4_(false),
	isCollisionArea5_(false),
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

	// ステージの当たり判定を生成
	actorManager->CreateActor<StageCollision>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// ステージの当たり判定を生成
	actorManager->CreateActor<Area1Collision>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// ステージの当たり判定を生成
	actorManager->CreateActor<Area2Collision>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// ステージの当たり判定を生成
	actorManager->CreateActor<Area3Collision>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// ステージの当たり判定を生成
	actorManager->CreateActor<Area4Collision>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// ステージの当たり判定を生成
	actorManager->CreateActor<Area5Collision>(stageData, { 0.0f,0.0f,0.0f });
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
	actorManager->ActiveData(ActorType::BOSS, { -10800.0f,-10000.0f,-150000.0f });

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

			// Area1Collisionにキャスト
			auto area1Collision = std::dynamic_pointer_cast<Area1Collision>(stage);

			// エリア1に衝突しているか判定
			if (area1Collision && area1Collision->GetIsCollision() && !isCollisionArea1_)
			{

				// エリア1と衝突した
				isCollisionArea1_ = true;

				// 敵を生成
				for (int i = 0; i < AREA1_TOTAL_ENEMYS; i++)
				{
					float x = std::rand() % 10000;
					float z = std::rand() % 10000;
					actorManager->ActiveData(ActorType::ENEMY, { 9300.0f + x,-15000,23600.0f + z });
				}

			}

			// Area2Collisionにキャスト
			auto area2Collision = std::dynamic_pointer_cast<Area2Collision>(stage);

			// エリア2に衝突しているか判定
			if (area2Collision && area2Collision->GetIsCollision() && !isCollisionArea2_)
			{

				// エリア2と衝突した
				isCollisionArea2_ = true;

				// 敵を生成
				for (int i = 0; i < AREA2_TOTAL_ENEMYS; i++)
				{
					float x = std::rand() % 100;
					float z = std::rand() % 100;
					actorManager->ActiveData(ActorType::ENEMY, { -3100.0f + x,-18000.0f,-22000.0f + z });
				}

			}

			// Area3Collisionにキャスト
			auto area3Collision = std::dynamic_pointer_cast<Area3Collision>(stage);

			// エリア3に衝突しているか判定
			if (area3Collision && area3Collision->GetIsCollision() && !isCollisionArea3_)
			{

				// エリア3と衝突した
				isCollisionArea3_ = true;

				// エリア3の当たり判定を付ける
				area3Collision->SetIsCollision(true);

				// 敵を生成
				for (int i = 0; i < AREA3_TOTAL_ENEMYS; i++)
				{
					float x = std::rand() % 100;
					float z = std::rand() % 100;
					actorManager->ActiveData(ActorType::ENEMY, { -81000.0f + x,-19500.0f,-34000.0f + z });
				}

			}

			// Area4Collisionにキャスト
			auto area4Collision = std::dynamic_pointer_cast<Area4Collision>(stage);

			// エリア4に衝突しているか判定
			if (area4Collision && area4Collision->GetIsCollision() && !isCollisionArea4_)
			{

				// エリア4と衝突した
				isCollisionArea4_ = true;

				// 敵を生成
				for (int i = 0; i < AREA4_TOTAL_ENEMYS; i++)
				{
					float x = std::rand() % 1000;
					float z = std::rand() % 1000;
					actorManager->ActiveData(ActorType::ENEMY, { -60000.0f + x,-18000.0f,-68000.0f + z });
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

	// コリジョンマネージャーを取得
	std::shared_ptr<CollisionManager> collisionManager = gameScene->GetCollisionManager();

	// ステージがあるかチェック
	if (!actorManager->GetActiveActorData().contains(ActorType::STAGE)) return;

	// ステージを取り出す
	auto& stages = actorManager->GetActiveActorData().at(ActorType::STAGE);

	for (auto stage : stages)
	{

#pragma region Area1Collision

		// Area1Collisionにキャスト
		std::shared_ptr<Area1Collision> area1Collision = std::dynamic_pointer_cast<Area1Collision>(stage);

		// エリア1の敵をすべて倒したら衝突判定を消す
		if (area1Collision && deathEnemyCnt_ == AREA1_TOTAL_ENEMYS && area1Collision->GetIsCollision())
		{

			// 衝突判定を消す
			area1Collision->SetIsCollision(false);

			// 徐々に消していくようにする
			area1Collision->SetIsDissolve(true);

			// 基底クラスにキャスト
			std::shared_ptr<StageBase> stageBase = std::dynamic_pointer_cast<StageBase>(stage);

			//	collisionManagerの要素から消す
			collisionManager->StageErasure(stageBase);

			// 敵の倒したカウンタをリセット
			deathEnemyCnt_ = 0;

		}

		// 完全に透過したら非アクティブにする
		if (area1Collision && area1Collision->GetAlphaTime() >= 1.0f)
		{
			area1Collision->SetIsActive(false);
		}

#pragma endregion

#pragma region Area2Collision

		// Area2Collisionにキャスト
		std::shared_ptr<Area2Collision> area2Collision = std::dynamic_pointer_cast<Area2Collision>(stage);

		// エリア1の敵をすべて倒したら衝突判定を消す
		if (area2Collision && deathEnemyCnt_ == AREA2_TOTAL_ENEMYS && area2Collision->GetIsCollision())
		{

			// 衝突判定を消す
			area2Collision->SetIsCollision(false);

			// 徐々に消していくようにする
			area2Collision->SetIsDissolve(true);

			// 基底クラスにキャスト
			std::shared_ptr<StageBase> stageBase = std::dynamic_pointer_cast<StageBase>(stage);

			//	collisionManagerの要素から消す
			collisionManager->StageErasure(stageBase);

			// 敵の倒したカウンタをリセット
			deathEnemyCnt_ = 0;

		}

		// 完全に透過したら非アクティブにする
		if (area2Collision && area2Collision->GetAlphaTime() >= 1.0f)
		{
			area2Collision->SetIsActive(false);
		}

#pragma endregion

#pragma region Area3Collision

		// Area3Collisionにキャスト
		std::shared_ptr<Area3Collision> area3Collision = std::dynamic_pointer_cast<Area3Collision>(stage);

		// エリア1の敵をすべて倒したら衝突判定を消す
		if (area3Collision && deathEnemyCnt_ == AREA3_TOTAL_ENEMYS && area3Collision->GetIsCollision())
		{

			// 衝突判定を消す
			area3Collision->SetIsCollision(false);

			// 徐々に消していくようにする
			area3Collision->SetIsDissolve(true);

			// 基底クラスにキャスト
			std::shared_ptr<StageBase> stageBase = std::dynamic_pointer_cast<StageBase>(stage);

			//	collisionManagerの要素から消す
			collisionManager->StageErasure(stageBase);

			// 敵の倒したカウンタをリセット
			deathEnemyCnt_ = 0;

		}

		// 完全に透過したら非アクティブにする
		if (area3Collision && area3Collision->GetAlphaTime() >= 1.0f)
		{
			area3Collision->SetIsActive(false);
		}

#pragma endregion

#pragma region Area4Collision

		// Area4Collisionにキャスト
		std::shared_ptr<Area4Collision> area4Collision = std::dynamic_pointer_cast<Area4Collision>(stage);

		// エリア1の敵をすべて倒したら衝突判定を消す
		if (area4Collision && deathEnemyCnt_ == AREA4_TOTAL_ENEMYS && area4Collision->GetIsCollision())
		{

			// 衝突判定を消す
			area4Collision->SetIsCollision(false);

			// 徐々に消していくようにする
			area4Collision->SetIsDissolve(true);

			// 基底クラスにキャスト
			std::shared_ptr<StageBase> stageBase = std::dynamic_pointer_cast<StageBase>(stage);

			//	collisionManagerの要素から消す
			collisionManager->StageErasure(stageBase);

			// 敵の倒したカウンタをリセット
			deathEnemyCnt_ = 0;

		}

		// 完全に透過したら非アクティブにする
		if (area4Collision && area4Collision->GetAlphaTime() >= 1.0f)
		{
			area4Collision->SetIsActive(false);
		}

#pragma endregion

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