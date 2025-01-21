#include "GameClearScene.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ActorManager.h"
#include "../Object/Common/GameClearActorCreate.h"

GameClearScene::GameClearScene()
{
}

void GameClearScene::Init()
{

	// アクター管理クラス
	actorManager_ = std::make_shared<ActorManager>();

	// アクターの生成クラス
	actorCreate_ = std::make_shared<GameClearActorCreate>();

	// カメラモードをボスの登場用のモードに変更
	//SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::GAME_CLEAR);

	// 画像の初期化
	//InitImage();

	// BGMとSEの初期化
	//InitBGMAndSE();

}

void GameClearScene::InitImage()
{
}

void GameClearScene::InitBGMAndSE()
{
}

void GameClearScene::Update(const float deltaTime)
{

	// アクターの管理クラスの更新
	actorManager_->Update(deltaTime);

	// アクターの生成クラスの更新
	actorCreate_->Update();

}

void GameClearScene::Draw(const float deltaTime)
{

	// アクターの管理クラスの更新
	actorManager_->Draw(deltaTime);

	// タイトルロゴの画像の描画
	DrawRotaGraph(850, 300, 1.0, 0.0, gameClearImg_, true);

	float scale = 1.0f;  // 拡大率
	const float animationSpeed = 0.05f; // アニメーションの速度

	// スケールを時間に応じて変更（sin波で変化）
	scale = 1.0 + 0.1f * std::sin(frameCount_ * animationSpeed);

	// Press A Button or Spaceの画像の描画
	DrawRotaGraph(850, 500, scale, 0.0, pressAButtonOrSpaceImg_, true);

	// フレームカウントを更新
	frameCount_++;

}