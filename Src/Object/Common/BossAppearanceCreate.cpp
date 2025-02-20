#include <fstream>
#include "../Lib/nlohmann/json.hpp"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ActorManager.h"
#include "../Scene/BossAppearanceScene.h"
#include "BossAppearanceCreate.h"
#include "../Object/BossAppearanceBoss.h"
#include "../Object/BossAppearancePlayer.h"
#include "../Object/Stage.h"
#include "../Object/StageCollision.h"

BossAppearanceCreate::BossAppearanceCreate()
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
	std::shared_ptr<BossAppearanceScene> bossAppearanceScene =
		std::dynamic_pointer_cast<BossAppearanceScene>(SceneManager::GetInstance().GetNowScene());

	// NULLチェック
	if (!bossAppearanceScene) return;

	// アクターマネージャーを取得
	std::shared_ptr<ActorManager> actorManager = bossAppearanceScene->GetActorManager();

	// ステージ
	const auto& stageData = objectData[static_cast<int>(ActorType::STAGE)]["StageData"];

	// ステージを生成
	actorManager->CreateActor<Stage>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// ステージの当たり判定を生成
	actorManager->CreateActor<StageCollision>(stageData, { 0.0f,0.0f,0.0f });
	actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// プレイヤー
	const auto& playerData = objectData[static_cast<int>(ActorType::PLAYER)]["PlayerData"];

	// プレイヤーを生成
	actorManager->CreateActor<BossAppearancePlayer>(playerData, PLAYER_POS);
	actorManager->ActiveData(ActorType::PLAYER, PLAYER_POS);

	// ボス
	const auto& bossData = objectData[static_cast<int>(ActorType::BOSS)]["BossData"];

	// ボスを生成
	actorManager->CreateActor<BossAppearanceBoss>(bossData, BOSS_POS);
	actorManager->ActiveData(ActorType::BOSS, BOSS_POS);

}

void BossAppearanceCreate::Update()
{
}

void BossAppearanceCreate::Draw()
{
}
