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
	const auto& stageData = objectData[static_cast<int>(ActorType::STAGE)]["StageData"];

	// ステージを生成
	actorManager->CreateActor<Stage>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// ステージの当たり判定を生成
	actorManager->CreateActor<StageCollision>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// ステージの当たり判定を生成
	actorManager->CreateActor<Area5Collision>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

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
	const auto& playerData = objectData[static_cast<int>(ActorType::PLAYER)]["PlayerData"];

	// プレイヤーを生成
	actorManager->CreateActor<Player>(playerData, PLAYER_INIT_POS);
	actorManager->ActiveData(ActorType::PLAYER, PLAYER_INIT_POS);

	// 敵
	const auto& enemyData = objectData[static_cast<int>(ActorType::ENEMY)]["EnemyData"];

	// 敵を生成
	for (int i = 0; i < CREATE_ENEMYS; i++)
	{
		actorManager->CreateActor<Enemy>(enemyData, { 0.0f,0.0f,0.0f });
	}

	// ボス
	const auto& bossData = objectData[static_cast<int>(ActorType::BOSS)]["BossData"];

	// ボスを生成
	actorManager->CreateActor<Boss>(bossData, BOSS_INIT_POS);
	actorManager->ActiveData(ActorType::BOSS, BOSS_INIT_POS);

}

void BossBattleActorCreate::Update()
{

	// 基底クラスから使いたい型へキャストする
	std::shared_ptr<BossBattleScene> bossBattleScene =
		std::dynamic_pointer_cast<BossBattleScene>(SceneManager::GetInstance().GetNowScene());

	// NULLチェック
	if (!bossBattleScene) return;

	// アクターマネージャーを取得
	std::shared_ptr<ActorManager> actorManager = bossBattleScene->GetActorManager();

	// ボス
	const auto& bosses = actorManager->GetActiveActorData().find(ActorType::BOSS);

	// プレイヤーの中身が入っているか確認
	if (bosses == actorManager->GetActiveActorData().end()) return;

	for (auto& boss : bosses->second)
	{

		// Bossにキャスト
		std::shared_ptr<Boss> b = std::dynamic_pointer_cast<Boss>(boss);
		
		// BossがCALLになったら敵を生成する
		if (b->GetState() == static_cast<int>(BossState::CALL) && b->GetAnimationController()->GetStepAnim() >= b->CREATE_ENEMY_FRAME && b->GetCreateEnemyCoolTimeCnt() == 0.0f)
		{

			// 敵を生成
			for (int i = 0; i < ENEMY_CREATE_NUM; i++)
			{
			
				// 座標をランダムで指定
				float x = GenerateRandNumber();
				float z = GenerateRandNumber();
				actorManager->ActiveData(ActorType::ENEMY,
					{ ENEMY_POS.x + x, ENEMY_POS.y, ENEMY_POS.z + z });
			
			}
		
		}

	}

}

void BossBattleActorCreate::Draw()
{
}
