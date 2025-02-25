#define NOMINMAX
#include <cmath>
#include "../Application.h"
#include "../Manager/ActorManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Common/BossAppearanceCreate.h"
#include "BossAppearanceScene.h"

BossAppearanceScene::BossAppearanceScene()
{

}

void BossAppearanceScene::Init()
{

	// アクター管理クラス
	actorManager_ = std::make_shared<ActorManager>();

	// 衝突判定管理クラス
	collisionManager_ = std::make_shared<CollisionManager>();

	// 衝突判定管理クラスにカメラの情報を設定
	collisionManager_->SetCamera(SceneManager::GetInstance().GetCamera());

	// アクターの生成クラス
	actorCreate_ = std::make_shared<BossAppearanceCreate>();
	
	// カメラモードをボスの登場用のモードに変更
	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::APPEARANCE);

	// 画像の初期化
	InitImage();

	// BGMとSEの初期化
	InitBGMAndSE();

}

void BossAppearanceScene::InitImage()
{

	// ボスが現れた!!用の画像
	appearanceImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_BOSS_APPEARANCE)]).handleId_;

	// ボスが現れた!!用の画像
	appearance2Img_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_BOSS_APPEARANCE)]).handleId_;

	// 1枚目の画像のスケール
	img1Scale_ = 2.0f;

	// 2枚目の画像のスケール
	img2Scale_ = 1.0f;

	// 1枚目の透明度
	img1Alpha_ = 0;

	// 2枚目の透明度
	img2Alpha_ = 255;

}

void BossAppearanceScene::InitBGMAndSE()
{

	// BGMの初期化
	bgm_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_BOSS_APPEARANCE_SCENE_BGM)]).handleId_;

	// BGMのボリュームの変更
	ChangeVolumeSoundMem(SOUND_MAX * SOUND_BGM_VOLUME, bgm_);

	// BGM再生
	PlaySoundMem(bgm_, DX_PLAYTYPE_LOOP, true);

}

void BossAppearanceScene::Update(const float deltaTime)
{

	// アクターの管理クラスの更新
	actorManager_->Update(deltaTime);

	// 衝突判定の管理クラスの更新
	collisionManager_->Update(deltaTime);

	// アクターの生成クラスの更新
	actorCreate_->Update();

	// カメラを生成
	std::weak_ptr<Camera> camera = SceneManager::GetInstance().GetCamera();

	// ボスの登場シーンが終わっっているか
	if (camera.lock()->GetIsEndBossAppearanceScene())
	{
		SceneManager::GetInstance().ChangeScene(SCENE_ID::BOSS_BATTLE);
	}

}

void BossAppearanceScene::Draw(const float deltaTime)
{

	// アクターの管理クラスの更新
	actorManager_->Draw(deltaTime);

	if (SceneManager::GetInstance().GetCamera().lock()->GetElapsedTime() >= BOSS_NAME_DRAW_START_TIME && SceneManager::GetInstance().GetCamera().lock()->GetIsBossAppearanceCameraMove3())
	{

		// ブレンドモードの設定
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, img1Alpha_);

		// ボスが現れた!!用の画像の描画
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, BOSS_APPEAR_TEXT_Y, img1Scale_, 0.0, appearanceImg_, true);

		// ブレンドモードを解除
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// アルファ値を増加（255を上限とする）
		img1Alpha_ += IMG1_ALPHA_INCREMENT;

		// 最大値は255にする
		img1Alpha_ = std::min(img1Alpha_, IMG1_ALPHA_MAX);

		// 画像のスケールを減少
		img1Scale_ -= IMG1_SCALE_DECREASE;
		
		// 最小値は1.0fにする
		img1Scale_ = std::max(img1Scale_, IMG1_SCALE_MIN);

		// 1枚目の画像のスケールが1.0になったら後ろから画像を出す
		if (img1Scale_ == IMG1_SCALE_MIN && img2Scale_ <= IMG2_SCALE_MAX)
		{

			// ブレンドモードの設定
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, img2Alpha_);

			// ボスが現れた!!用の画像の描画
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2, BOSS_APPEAR_TEXT_Y, img2Scale_, 0.0, appearance2Img_, true);

			// ブレンドモードを解除
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			// アルファ値を減少（255を上限とする）
			img2Alpha_ -= IMG2_ALPHA_DECREASE; // アルファ値を増やす速度を調整可能
			
			// 最小値は0にする
			img2Alpha_ = std::max(img2Alpha_, 0);

			// スケール値を増加
			img2Scale_ += IMG2_SCALE_INCREMENT;

			// 最大値は3.0fにする
			img1Scale_ = std::min(img1Scale_, IMG2_SCALE_MAX);

		}

	}

}