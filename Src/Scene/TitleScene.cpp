#include <DxLib.h>
#include "../Application.h"
#include "../Manager/ActorManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Common/TitleActorCreate.h"
#include "TitleScene.h"

TitleScene::TitleScene()
{
}

void TitleScene::Init()
{

	// アクター管理クラス
	actorManager_ = std::make_shared<ActorManager>();

	// アクターの生成クラス
	actorCreate_ = std::make_shared<TitleActorCreate>();

	// カメラモードをボスの登場用のモードに変更
	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::TITLE);

	// 画像の初期化
	InitImage();

	// BGMとSEの初期化
	InitBGMAndSE();

}
void TitleScene::InitImage()
{

	// タイトルロゴの画像の初期化
	titleLogoImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_TITLE_LOGO)]).handleId_;

	// Press A Button or Spaceの画像の初期化
	pressAButtonOrSpaceImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_PRESS_A_BUTTON_OR_SPACE)]).handleId_;

	// Press A Button or Space用のカウンタの初期化
	frameCount_ = 0.0f;

}

void TitleScene::InitBGMAndSE()
{

	// BGMの初期化
	bgm_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_TITLE_SCENE_BGM)]).handleId_;

	// BGMのボリュームの変更
	ChangeVolumeSoundMem(SOUND_MAX * SOUND_BGM_VOLUME, bgm_);

	// BGM再生
	PlaySoundMem(bgm_, DX_PLAYTYPE_LOOP, true);

	// 決定ボタンSEの初期化
	decideSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_TITLE_SCENE_DECIDE_SE)]).handleId_;

}

void TitleScene::Update(const float deltaTime)
{

	// アクターの管理クラスの更新
	actorManager_->Update(deltaTime);

	// アクターの生成クラスの更新
	actorCreate_->Update();

	InputManager& ins = InputManager::GetInstance();

	// シーン遷移
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE))
	{

		// 決定ボタンSEの再生
		PlaySoundMem(decideSE_, DX_PLAYTYPE_BACK, true);

		// ゲームシーンへ遷移
		SceneManager::GetInstance().ChangeScene(SCENE_ID::GAME);

	}
	else if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
	{

		// 決定ボタンSEの再生
		PlaySoundMem(decideSE_, DX_PLAYTYPE_BACK, true);

		// ゲームシーンへ遷移
		SceneManager::GetInstance().ChangeScene(SCENE_ID::GAME);

		// ゲームパッドだけ使えるように変更
		SceneManager::GetInstance().SetGamePad(true);

	}

}

void TitleScene::Draw(const float deltaTime)
{

	// アクターの管理クラスの更新
	actorManager_->Draw(deltaTime);

	// タイトルロゴの画像の描画
	DrawRotaGraph(TITLE_LOGO_X, TITLE_LOGO_Y, TITLE_LOGO_SCALE, 0.0, titleLogoImg_, true);

	float scale = TITLE_LOGO_SCALE;  // 拡大率
	const float animationSpeed = 0.05f; // アニメーションの速度

	// スケールを時間に応じて変更（sin波で変化）
	scale = TITLE_LOGO_SCALE + SCALE_AMPLITUDE * std::sin(frameCount_ * animationSpeed);

	// Press A Button or Spaceの画像の描画
	DrawRotaGraph(PRESS_A_BUTTON_OR_SPACE_X, PRESS_A_BUTTON_OR_SPACE_Y, scale, 0.0, pressAButtonOrSpaceImg_, true);

	// フレームカウントを更新
	frameCount_++;

}