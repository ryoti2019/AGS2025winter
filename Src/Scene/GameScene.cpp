#define NOMINMAX
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/ActorManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Common/ActorCreate.h"
#include "../Object/Common/InputController.h"
#include "GameScene.h"
#include "../Object/Player.h"
#include "../Object/Area1Collision.h"
#include "../Object/Area2Collision.h"
#include "../Object/Area5Collision.h"

GameScene::GameScene()
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

	// ゲームオーバーを表示し続けるためのカウンタ
	gameOverCnt_ = 0.0f;

	// 操作説明を見ているかのフラグ
	isViewUserGuide_ = true;

	// プレイヤーが死んだときのフラグ
	isPlayerDeath_ = false;

	// コンティニューするかのフラフ
	isContinue_ = true;

	// ゲームオーバーのフラグ
	isGameOver_ = false;

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

	// ゲームオーバー画像の初期化
	gameOverImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_GAME_OVER)]).handleId_;

	// ゲームオーバー画像2の初期化
	gameOverImg2_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_GAME_OVER)]).handleId_;

	// 画像のスケール
	img1Scale_ = 2.0f;

	// 画像のスケール
	img2Scale_ = 1.0f;

	// 透明度
	img1Alpha_ = 0;

	// 透明度
	img2Alpha_ = IMG1_ALPHA_MAX;

	// コンティニュー画像の初期化
	continueImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_CONTINUE)]).handleId_;
	
	// Yesの画像の初期化
	yesImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_YES)]).handleId_;
	
	// Noの画像の初期化
	noImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_NO)]).handleId_;

}

void GameScene::InitBGMAndSE()
{

	// BGMの初期化
	bgm_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_GAME_SCENE_BGM)]).handleId_;

	// BGMのボリュームの変更
	ChangeVolumeSoundMem(SOUND_MAX * BGM_VOLUME, bgm_);

	// BGM再生
	PlaySoundMem(bgm_, DX_PLAYTYPE_LOOP, true);

	// ゲームオーバーボイス
	gameOverVoice_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_GAME_OVER_SCENE_VOICE)]).handleId_;

	// 決定ボタンSEの初期化
	decideSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_TITLE_SCENE_DECIDE_SE)]).handleId_;

	// カーソル移動のSE
	cursorMoveSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_CURSOR_MOVE_SE)]).handleId_;

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

	// ボスの登場シーンに遷移するか判定
	CheckTransitionBossAppearanceScene();

	// プレイヤーが死んだらコンティニューかゲームオーバーを選ばせる
	if (isPlayerDeath_)
	{
		SelectContinueOrGameOver(deltaTime);
	}

}

void GameScene::CheckTransitionBossAppearanceScene()
{

	// ステージがあるかチェック
	if (!actorManager_->GetActiveActorData().contains(ActorType::STAGE))return;

	// プレイヤーがあるかチェック
	if (!actorManager_->GetActiveActorData().contains(ActorType::PLAYER))return;

	// ステージを取り出す
	auto& stages = actorManager_->GetActiveActorData().at(ActorType::STAGE);

	// プレイヤーを取り出す
	auto& players = actorManager_->GetActiveActorData().at(ActorType::PLAYER);

	// Area5Collisionを探してあれば代入する
	auto area5Collision = std::find_if(stages.begin(), stages.end(), [](const std::shared_ptr<ActorBase>& ptr)
		{
			return ptr == std::dynamic_pointer_cast<Area5Collision>(ptr);
		});

	// Area5にキャスト
	auto area5 = std::dynamic_pointer_cast<Area5Collision>(*area5Collision);

	// プレイヤーを探してあれば代入する
	auto player = std::find_if(players.begin(), players.end(), [](const std::shared_ptr<ActorBase>& ptr)
		{
			return ptr == std::dynamic_pointer_cast<Player>(ptr);
		});

	// プレイヤーにキャスト
	auto p = std::dynamic_pointer_cast<Player>(*player);

	// 衝突しているか
	if (area5->GetIsCollision())
	{

		// ボスの登場シーンに遷移
		SceneManager::GetInstance().ChangeScene(SCENE_ID::BOSS_APPEARANCE);

		// プレイヤーのHPを設定
		SceneManager::GetInstance().SetPlayerSpecialAttackGauge(p->GetSpecialAttackGauge());

		// プレイヤーのHPを設定
		SceneManager::GetInstance().SetPlayerHp(p->GetHp());

	}

}

void GameScene::SelectContinueOrGameOver(const float deltaTime)
{

	// 右方向にスティックを倒しているか
	if (inputController_->SelectRight() && isContinue_)
	{

		isContinue_ = false;

		// カーソル移動のSEを再生
		PlaySoundMem(cursorMoveSE_, DX_PLAYTYPE_BACK);

	}

	// 左方向にスティックを倒しているか
	if (inputController_->SelectLeft() && !isContinue_)
	{
		
		isContinue_ = true;

		// カーソル移動のSEを再生
		PlaySoundMem(cursorMoveSE_, DX_PLAYTYPE_BACK);

	}

	// コンティニューする場合
	if (isContinue_ && inputController_->Decide())
	{
		SceneManager::GetInstance().ChangeScene(SCENE_ID::GAME);
	}
	else if (!isContinue_ && inputController_->Decide())
	{

		isGameOver_ = true;

		// 決定ボタンのSEを再生
		PlaySoundMem(decideSE_, DX_PLAYTYPE_BACK);

		// ゲームオーバーボイスを再生
		PlaySoundMem(gameOverVoice_, DX_PLAYTYPE_BACK);

	}

	// ゲームオーバー
	if (isGameOver_)
	{

		// ゲームオーバーになって5秒たってからタイトルに戻る
		if (gameOverCnt_ >= GAME_OVER_DELAY)
		{

			SceneManager::GetInstance().ChangeScene(SCENE_ID::TITLE);

			// Releaseを一回だけ通らないようにするフラグをtrueにする
			SceneManager::GetInstance().SetIsFirstRelease(true);

		}

		gameOverCnt_ += deltaTime;
	
	}

}

void GameScene::Draw(const float deltaTime)
{

	// アクターの管理クラスの更新
	actorManager_->Draw(deltaTime);

	// アクターの生成クラスの描画
	actorCreate_->Draw();

	// 操作説明の描画
	DrawUserGuide();

	if (isPlayerDeath_)
	{
		// ゲームオーバーの描画
		DrawGameOver();
	}

}

void GameScene::DrawUserGuide()
{

	// 操作説明をスキップされるまで描画
	if (isViewUserGuide_)
	{

		// 半透明の黒い矩形を画面全体に描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, USER_GUIDE_ALPHA);
		
		DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, GetColor(0, 0, 0), TRUE);
		
		// ブレンドモード解除
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		if (!SceneManager::GetInstance().GetGamePad())
		{
			// キーボードの操作説明の画像を描画
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, USER_GUIDE_IMAGE_SCALE, 0.0f, keyboardUserGuideImg_, true);
		}
		else if (SceneManager::GetInstance().GetGamePad())
		{
			// ゲームパッドの操作説明の画像を描画
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, USER_GUIDE_IMAGE_SCALE, 0.0f, gamePadUserGuideImg_, true);
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
				DrawRotaGraph(USER_GUIDE_POS_X, USER_GUIDE_POS_Y, USER_GUIDE_IMAGE_SCALE, 0.0, keyboardUserGuideMoveImg_, true);
			}
			else if (area1Collision && area1Collision->GetIsCollision())
			{
				// 攻撃のチュートリアル画像を描画
				DrawRotaGraph(USER_GUIDE_POS_X, USER_GUIDE_POS_Y, USER_GUIDE_IMAGE_SCALE, 0.0, keyboardUserGuideAttackImg_, true);
			}

			// Area2Collisionにキャスト
			auto area2Collision = std::dynamic_pointer_cast<Area2Collision>(stage);

			if (area2Collision && area2Collision->GetIsCollision())
			{
				// 必殺技のチュートリアル画像を描画
				DrawRotaGraph(USER_GUIDE_POS_X, USER_GUIDE_POS_Y, USER_GUIDE_IMAGE_SCALE, 0.0, keyboardUserGuideSpecialAttackImg_, true);
			}

		}
		else if (SceneManager::GetInstance().GetGamePad())
		{

			// Area1Collisionにキャスト
			auto area1Collision = std::dynamic_pointer_cast<Area1Collision>(stage);

			if (area1Collision && !area1Collision->GetIsCollision() && !area1Collision->GetIsDissolve())
			{
				// 移動のチュートリアル画像を描画
				DrawRotaGraph(USER_GUIDE_POS_X, USER_GUIDE_POS_Y, USER_GUIDE_IMAGE_SCALE, 0.0, gamePadUserGuideMoveImg_, true);
			}
			else if (area1Collision && area1Collision->GetIsCollision())
			{
				// 攻撃のチュートリアル画像を描画
				DrawRotaGraph(USER_GUIDE_POS_X, USER_GUIDE_POS_Y, USER_GUIDE_IMAGE_SCALE, 0.0, gamePadUserGuideAttackImg_, true);
			}

			// Area2Collisionにキャスト
			auto area2Collision = std::dynamic_pointer_cast<Area2Collision>(stage);

			if (area2Collision && area2Collision->GetIsCollision())
			{
				// 必殺技のチュートリアル画像を描画
				DrawRotaGraph(USER_GUIDE_POS_X, USER_GUIDE_POS_Y, USER_GUIDE_IMAGE_SCALE, 0.0, gamePadUserGuideSpecialAttackImg_, true);
			}

		}

	}

}

void GameScene::DrawGameOver()
{

	// 半透明の黒い矩形を画面全体に描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, GAME_OVER_ALPHA); // 半透明（128: 50%の透明度）
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ブレンドモード解除

	if (isGameOver_)
	{

		// ブレンドモードの設定
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, img1Alpha_);

		// ゲームオーバーの描画
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, GAME_OVER_IMG_SCALE, 0.0, gameOverImg_, true);

		// ブレンドモードを解除
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// アルファ値を増加（255を上限とする）
		img1Alpha_ += IMG1_ALPHA_INCREMENT;

		// 最少値は255にする
		img1Alpha_ = std::min(img1Alpha_, IMG1_ALPHA_MAX);

		// 画像のスケールを減少
		img1Scale_ -= IMG1_ALPHA_DECREASE;

		// 最大値は1.0fにする
		img1Scale_ = std::max(img1Scale_, IMG1_SCALE_MAX);

		// 1枚目の画像のスケールが1.0になったら後ろから画像を出す
		if (img1Scale_ == IMG1_SCALE_MAX && img2Scale_ <= IMG2_SCALE_MAX)
		{

			// ブレンドモードの設定
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, img2Alpha_);

			// ボスが現れた!!用の画像の描画
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, img2Scale_, 0.0, gameOverImg2_, true);

			// ブレンドモードを解除
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			// アルファ値を増加（255を上限とする）
			img2Alpha_ -= IMG2_ALPHA_DECREASE; // アルファ値を増やす速度を調整可能

			// 最小値は0にする
			img2Alpha_ = std::max(img2Alpha_, 0);

			// スケール値を増加
			img2Scale_ += IMG2_ALPHA_INCREMENT;

			// 最大値は3.0fにする
			img1Scale_ = std::min(img1Scale_, IMG2_SCALE_MAX);

		}

	}
	else
	{

		// コンティニュー画像の描画
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, CONTINUE_POS_Y, CONTINUE_IMAGE_SCALE, 0.0, continueImg_, true);

		float yesImgScale = YES_IMAGE_SCALE;
		float noImgScale = NO_IMAGE_SCALE;

		// アニメーションの速度
		const float animationSpeed = ANIM_SPEED;

		if (isContinue_)
		{
			// YES画像のスケールを時間に応じて変更
			yesImgScale = YES_IMAGE_SCALE + SCLAE_VARIATION * std::sin(frameCount_ * animationSpeed);
		}
		else
		{
			// NO画像のスケールを時間に応じて変更
			noImgScale = NO_IMAGE_SCALE + SCLAE_VARIATION * std::sin(frameCount_ * animationSpeed);
		}

		// YES画像の描画
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2 - YES_POS_X_OFFSET, YES_POS_Y, yesImgScale, 0.0, yesImg_, true);

		// NO画像の描画
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2 + NO_POS_X_OFFSET, NO_POS_Y, noImgScale, 0.0, noImg_, true);
	
		// フレームカウントを更新
		frameCount_++;
	
	}

}