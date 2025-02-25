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
	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::GAME_CLEAR);

	// 画像の初期化
	InitImage();

	// BGMとSEの初期化
	InitBGMAndSE();

}

void GameClearScene::InitImage()
{

	// ゲームクリア画像の初期化
	gameClearImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_GAME_CLEAR)]).handleId_;

	// Press A Button or Spaceの画像の初期化
	pressAButtonOrSpaceImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_PRESS_A_BUTTON_OR_SPACE)]).handleId_;

	// Press A Button or Space用のカウンタの初期化
	frameCount_ = 0.0f;

}

void GameClearScene::InitBGMAndSE()
{

	// BGMの初期化
	bgm_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_GAME_CLEAR_SCENE_BGM)]).handleId_;

	// BGMのボリュームの変更
	ChangeVolumeSoundMem(SOUND_MAX * SOUND_BGM_VOLUME, bgm_);

	// BGM再生
	PlaySoundMem(bgm_, DX_PLAYTYPE_LOOP, true);

	// 決定ボタンSEの初期化
	decideSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_TITLE_SCENE_DECIDE_SE)]).handleId_;

}

void GameClearScene::Update(const float deltaTime)
{

	// アクターの管理クラスの更新
	actorManager_->Update(deltaTime);

	// アクターの生成クラスの更新
	actorCreate_->Update();

	// シーン遷移
	if (inputController_->Decide())
	{

		// 決定ボタンSEの再生
		PlaySoundMem(decideSE_, DX_PLAYTYPE_BACK, true);

		// タイトルシーンに遷移
		SceneManager::GetInstance().ChangeScene(SCENE_ID::TITLE);

		// Releaseを一回だけ通らないようにするフラグをtrueにする
		SceneManager::GetInstance().SetIsFirstRelease(true);

		// ゲームパッドだけ使えるようにするのをリセット
		SceneManager::GetInstance().SetGamePad(false);

		StopSoundMem(bgm_);

	}

}

void GameClearScene::Draw(const float deltaTime)
{

	// アクターの管理クラスの更新
	actorManager_->Draw(deltaTime);

	// ゲームクリア画像の描画
	DrawRotaGraph(GAME_CLEAR_X, GAME_CLEAR_Y, GAME_CLEAR_SCALE, 0.0, gameClearImg_, true);

	// 拡大率
	float scale = GAME_CLEAR_IMG_SCALE;

	// スケールを時間に応じて変更（sin波で変化）
	scale = GAME_CLEAR_IMG_SCALE + SCALE_AMPLITUDE * std::sin(frameCount_ * GAME_CLEAR_ANIM_SPEED);

	// Press A Button or Spaceの画像の描画
	DrawRotaGraph(PRESS_A_BUTTON_OR_SPACE_X, PRESS_A_BUTTON_OR_SPACE_Y, scale, 0.0, pressAButtonOrSpaceImg_, true);

	// フレームカウントを更新
	frameCount_++;

}