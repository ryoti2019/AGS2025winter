#include <fstream>
#include "../Application.h"
#include "../Lib/nlohmann/json.hpp"
#include "../Manager/SceneManager.h"
#include "../Manager/ActorManager.h"
#include "../Scene/GameClearScene.h"
#include "../Object/GameClearPlayer.h"
#include "../Object/Stage.h"
#include "GameClearActorCreate.h"

GameClearActorCreate::GameClearActorCreate()
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
	std::shared_ptr<GameClearScene> gameClearScene =
		std::dynamic_pointer_cast<GameClearScene>(SceneManager::GetInstance().GetNowScene());

	// NULLチェック
	if (!gameClearScene) return;

	// アクターマネージャーを取得
	std::shared_ptr<ActorManager> actorManager = gameClearScene->GetActorManager();

	// ステージ
	const auto& stageData = objectData[3]["StageData"];

	// ステージを生成
	actorManager->CreateActor<Stage>(stageData, { 0.0f,0.0f,0.0f });
	int cnt = actorManager->GetDeActiveActorData().find(ActorType::STAGE)-> second[0].use_count();
	//actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });
	auto s = actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });
	cnt = s.use_count();
	int b = 2;
	b = 3;
	// プレイヤー
	const auto& playerData = objectData[0]["PlayerData"];

	// プレイヤーを生成
	actorManager->CreateActor<GameClearPlayer>(playerData, { -8700.0f,-19500.0f,-140000.0f });
	actorManager->ActiveData(ActorType::PLAYER, { -8700.0f,-19500.0f,-140000.0f });

}
