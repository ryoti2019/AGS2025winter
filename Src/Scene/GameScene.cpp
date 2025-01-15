#include <DxLib.h>
#include "../Application.h"
#include "../Manager/ActorManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Common/ActorCreate.h"
#include "../Object/Common/InputController.h"
#include "GameScene.h"
#include "../Object/Area1Collision.h"
#include "../Object/Area2Collision.h"
#include "../Object/Area5Collision.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{

	// アクター管理クラス
	actorManager_ = std::make_shared<ActorManager>();

	// 衝突判定管理クラス
	collisionManager_ = std::make_shared<CollisionManager>();

	// 衝突判定管理クラスにカメラの情報を設定
	collisionManager_->SetCamera(SceneManager::GetInstance().GetCamera());

	// アクターの生成クラス
	actorCreate_ = std::make_shared<ActorCreate>();

	// プレイヤーに追従するモードに変更
	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::FOLLOW);

	// 画像の初期化
	InitImage();

	// BGMとSEの初期化
	InitBGMAndSE();

	// 操作説明を見ているかのフラグ
	isViewUserGuide_ = true;

}

void GameScene::InitImage()
{

	// ゲームパッドの操作説明の画像の初期化
	gamePadUserGuideImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_GAME_PAD_USER_GUIDE)]).handleId_;

	// ゲームパッドの移動の説明の画像の初期化
	gamePadUserGuideMoveImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_GAME_PAD_USER_GUIDE_MOVE)]).handleId_;

	// ゲームパッドの攻撃の説明の画像の初期化
	gamePadUserGuideAttackImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_GAME_PAD_USER_GUIDE_ATTACK)]).handleId_;

	// ゲームパッドの必殺技の説明の画像の初期化
	gamePadUserGuideSpecialAttackImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_GAME_PAD_USER_GUIDE_SPECIAL_ATTACK)]).handleId_;

	// キーボードの操作説明の画像の初期化
	keyboardUserGuideImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_KEYBOARD_USER_GUIDE)]).handleId_;

	// キーボードの移動の説明の画像の初期化
	keyboardUserGuideMoveImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_KEYBOARD_USER_GUIDE_MOVE)]).handleId_;

	// キーボードの攻撃の説明の画像の初期化
	keyboardUserGuideAttackImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_KEYBOARD_USER_GUIDE_ATTACK)]).handleId_;

	// キーボードの必殺技の説明の画像の初期化
	keyboardUserGuideSpecialAttackImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_KEYBOARD_USER_GUIDE_SPECIAL_ATTACK)]).handleId_;

}

void GameScene::InitBGMAndSE()
{

	// BGMの初期化
	bgm_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_GAME_SCENE_BGM)]).handleId_;

	// BGM再生
	PlaySoundMem(bgm_, DX_PLAYTYPE_LOOP, true);

}

void GameScene::Update(const float deltaTime)
{

	// 操作説明をスキップ
	if (inputController_->Decide())
	{
		isViewUserGuide_ = false;
	}

	if (isViewUserGuide_)return;

	// アクターの管理クラスの更新
	actorManager_->Update(deltaTime);

	// 衝突判定の管理クラスの更新
	collisionManager_->Update(deltaTime);

	// アクターの生成クラスの更新
	actorCreate_->Update();

	// ステージがあるかチェック
	if (!actorManager_->GetActiveActorData().contains(ActorType::STAGE))return;

	// ステージを取り出す
	auto& stages = actorManager_->GetActiveActorData().at(ActorType::STAGE);

	// Area5Collisionを探してあれば代入する
	auto area5Collision = std::find_if(stages.begin(), stages.end(), [](const std::shared_ptr<ActorBase>& ptr)
		{
			return ptr == std::dynamic_pointer_cast<Area5Collision>(ptr);
		});

	// Area5にキャスト
	auto area5 = std::dynamic_pointer_cast<Area5Collision>(*area5Collision);

	// 衝突していればボスの登場シーンに遷移
	if (area5->GetIsCollision())
	{
		SceneManager::GetInstance().ChangeScene(SCENE_ID::BOSS_APPEARANCE);
	}

}

void GameScene::Draw(const float deltaTime)
{

	// アクターの管理クラスの更新
	actorManager_->Draw(deltaTime);

	// アクターの生成クラスの描画
	actorCreate_->Draw();

	// 操作説明をスキップされるまで描画
	if (isViewUserGuide_)
	{

		// 半透明の黒い矩形を画面全体に描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200); // 半透明（128: 50%の透明度）
		DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ブレンドモード解除
		
		if (!SceneManager::GetInstance().GetGamePad())
		{
			// キーボードの操作説明の画像を描画
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 0.5f, 0.0f, keyboardUserGuideImg_, true);
		}
		else if (SceneManager::GetInstance().GetGamePad())
		{
			// ゲームパッドの操作説明の画像を描画
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 0.5f, 0.0f, gamePadUserGuideImg_, true);
		}

		return;
	}

	// ステージがあるかチェック
	if (!actorManager_->GetActiveActorData().contains(ActorType::STAGE))return;

	// ステージを取り出す
	auto& stages = actorManager_->GetActiveActorData().at(ActorType::STAGE);

	for (auto& stage : stages)
	{

		if (!SceneManager::GetInstance().GetGamePad())
		{

			// Area1Collisionにキャスト
			auto area1Collision = std::dynamic_pointer_cast<Area1Collision>(stage);

			if (area1Collision && !area1Collision->GetIsCollision() && !area1Collision->GetIsDissolve())
			{
				// 移動のチュートリアル画像を描画
				DrawRotaGraph(150, 200, 0.5, 0.0, keyboardUserGuideMoveImg_, true);
			}
			else if (area1Collision && area1Collision->GetIsCollision())
			{
				// 攻撃のチュートリアル画像を描画
				DrawRotaGraph(150, 200, 0.5, 0.0, keyboardUserGuideAttackImg_, true);
			}

			// Area2Collisionにキャスト
			auto area2Collision = std::dynamic_pointer_cast<Area2Collision>(stage);

			if (area2Collision && area2Collision->GetIsCollision())
			{
				// 必殺技のチュートリアル画像を描画
				DrawRotaGraph(150, 200, 0.5, 0.0, keyboardUserGuideSpecialAttackImg_, true);
			}

		}
		else if (SceneManager::GetInstance().GetGamePad())
		{

			// Area1Collisionにキャスト
			auto area1Collision = std::dynamic_pointer_cast<Area1Collision>(stage);

			if (area1Collision && !area1Collision->GetIsCollision() && !area1Collision->GetIsDissolve())
			{
				// 移動のチュートリアル画像を描画
				DrawRotaGraph(150, 200, 0.5, 0.0, gamePadUserGuideMoveImg_, true);
			}
			else if (area1Collision && area1Collision->GetIsCollision())
			{
				// 攻撃のチュートリアル画像を描画
				DrawRotaGraph(150, 200, 0.5, 0.0, gamePadUserGuideAttackImg_, true);
			}

			// Area2Collisionにキャスト
			auto area2Collision = std::dynamic_pointer_cast<Area2Collision>(stage);

			if (area2Collision && area2Collision->GetIsCollision())
			{
				// 必殺技のチュートリアル画像を描画
				DrawRotaGraph(150, 200, 0.5, 0.0, gamePadUserGuideSpecialAttackImg_, true);
			}

		}

	}

}