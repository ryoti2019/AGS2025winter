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

	// 画像のスケール
	scale_ = 2.0f;

	// 画像のスケール
	scale2_ = 1.0f;

	// 透明度
	alpha_ = 0;

	// 透明度
	alpha2_ = 255;

}

void BossAppearanceScene::InitBGMAndSE()
{

	// BGMの初期化
	bgm_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_BOSS_APPEARANCE_SCENE_BGM)]).handleId_;

	// BGMのボリュームの変更
	ChangeVolumeSoundMem(255 * 0.5, bgm_);

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

	if (SceneManager::GetInstance().GetCamera().lock()->GetElapsedTime() >= 6.0f && SceneManager::GetInstance().GetCamera().lock()->GetIsBossAppearanceCameraMove3())
	{

		// ブレンドモードの設定
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);

		// ボスが現れた!!用の画像の描画
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 500, scale_, 0.0, appearanceImg_, true);

		// ブレンドモードを解除
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// アルファ値を増加（255を上限とする）
		alpha_ += 2;

		// 最少値は255にする
		alpha_ = std::min(alpha_,255);

		// 画像のスケールを減少
		scale_ -= 0.02f;
		
		// 最大値は1.0fにする
		scale_ = std::max(scale_, 1.0f);

		// 1枚目の画像のスケールが1.0になったら後ろから画像を出す
		if (scale_ == 1.0f && scale2_ <= 3.0f)
		{

			// ブレンドモードの設定
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha2_);

			// ボスが現れた!!用の画像の描画
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 500, scale2_, 0.0, appearance2Img_, true);

			// ブレンドモードを解除
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			// アルファ値を増加（255を上限とする）
			alpha2_ -= 10; // アルファ値を増やす速度を調整可能
			
			// 最小値は0にする
			alpha2_ = std::max(alpha2_, 0);

			// スケール値を増加
			scale2_ += 0.05f;

			// 最大値は3.0fにする
			scale_ = std::min(scale_, 3.0f);

		}

	}

}