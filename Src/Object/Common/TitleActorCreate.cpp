#include <fstream>
#include "../Lib/nlohmann/json.hpp"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ActorManager.h"
#include "../Scene/TitleScene.h"
#include "../Object/TitlePlayer.h"
#include "../Object/Stage.h"
#include "TitleActorCreate.h"

TitleActorCreate::TitleActorCreate()
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
	std::shared_ptr<TitleScene> titleScene =
		std::dynamic_pointer_cast<TitleScene>(SceneManager::GetInstance().GetNowScene());

	// NULLチェック
	if (!titleScene) return;

	// アクターマネージャーを取得
	std::shared_ptr<ActorManager> actorManager = titleScene->GetActorManager();

	// ステージ
	//const auto& stageData = objectData[3]["StageData"];

	//// ステージを生成
	//actorManager->CreateActor<Stage>(stageData, { 0.0f,0.0f,0.0f });
	//actorManager->ActiveData(ActorType::STAGE, { stageData["POS"]["x"], stageData["POS"]["y"] ,stageData["POS"]["z"] });

	// プレイヤー
	const auto& playerData = objectData[0]["PlayerData"];

	// プレイヤーを生成
	actorManager->CreateActor<TitlePlayer>(playerData, { -80000.0f,-19500.0f,25900.0f });
	actorManager->ActiveData(ActorType::PLAYER, { -80000.0f,-19500.0f,25900.0f });

}