#define NOMINMAX
#include "../Application.h"
#include "../Manager/ActorManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Common/BossBattleActorCreate.h"
#include "BossBattleScene.h"
#include "../Object/Boss.h"

BossBattleScene::BossBattleScene()
{
}

void BossBattleScene::Init()
{

	// アクター管理クラス
	actorManager_ = std::make_shared<ActorManager>();

	// 衝突判定管理クラス
	collisionManager_ = std::make_shared<CollisionManager>();

	// 衝突判定管理クラスにカメラの情報を設定
	collisionManager_->SetCamera(SceneManager::GetInstance().GetCamera());

	// アクターの生成クラス
	actorCreate_ = std::make_shared<BossBattleActorCreate>();

	// プレイヤーに追従するモードに変更
	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::FOLLOW);

	// 画像の初期化
	InitImage();

	// BGMとSEの初期化
	InitBGMAndSE();

	// ゲームオーバーを表示し続けるためのカウンタ
	gameOverCnt_ = 0.0f;

	// プレイヤーが死んだときのフラグ
	isPlayerDeath_ = false;

	// コンティニューするかのフラフ
	isContinue_ = true;

	// ゲームオーバーのフラグ
	isGameOver_ = false;

}

void BossBattleScene::InitImage()
{

	// ゲームオーバー画像の初期化
	gameOverImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_GAME_OVER)]).handleId_;

	// ゲームオーバー画像2の初期化
	gameOverImg2_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_GAME_OVER)]).handleId_;

	// 画像のスケール
	scale_ = 2.0f;

	// 画像のスケール
	scale2_ = 1.0f;

	// 透明度
	alpha_ = 0;

	// 透明度
	alpha2_ = 255;

	// コンティニュー画像の初期化
	continueImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_CONTINUE)]).handleId_;

	// Yesの画像の初期化
	yesImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_YES)]).handleId_;

	// Noの画像の初期化
	noImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_NO)]).handleId_;

}

void BossBattleScene::InitBGMAndSE()
{

	// BGMの初期化
	bgm_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_BOSS_BATTLE_SCENE_BGM)]).handleId_;

	// BGMのボリュームの変更
	ChangeVolumeSoundMem(255 * 0.5, bgm_);

	// BGM再生
	PlaySoundMem(bgm_, DX_PLAYTYPE_LOOP, true);

	// ゲームオーバーボイス
	gameOverVoice_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_GAME_OVER_SCENE_VOICE)]).handleId_;

	// 決定ボタンSEの初期化
	decideSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_TITLE_SCENE_DECIDE_SE)]).handleId_;

	// カーソル移動のSE
	cursorMoveSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_CURSOR_MOVE_SE)]).handleId_;

}

void BossBattleScene::Update(const float deltaTime)
{

	// アクターの管理クラスの更新
	actorManager_->Update(deltaTime);

	// 衝突判定の管理クラスの更新
	collisionManager_->Update(deltaTime);

	// アクターの生成クラスの更新
	actorCreate_->Update();

	// ゲームクリアシーンに遷移するか判定
	CheckTransitionGameClearScene();

	// プレイヤーが死んだらコンティニューかゲームオーバーを選ばせる
	if (isPlayerDeath_)
	{
		SelectContinueOrGameOver(deltaTime);
	}

}

void BossBattleScene::CheckTransitionGameClearScene()
{

	// ボスがあるかチェック
	if (!actorManager_->GetActiveActorData().contains(ActorType::BOSS))return;

	// ステージを取り出す
	auto& stages = actorManager_->GetActiveActorData().at(ActorType::BOSS);

	// Area5Collisionを探してあれば代入する
	auto boss = std::find_if(stages.begin(), stages.end(), [](const std::shared_ptr<ActorBase>& ptr)
		{
			return ptr == std::dynamic_pointer_cast<Boss>(ptr);
		});

	// Area5にキャスト
	auto b = std::dynamic_pointer_cast<Boss>(*boss);

	// 衝突していればゲームクリアシーンに遷移
	if (!b->GetIsActive())
	{
		SceneManager::GetInstance().ChangeScene(SCENE_ID::GAME_CLEAR);
	}

}

void BossBattleScene::SelectContinueOrGameOver(const float deltaTime)
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
		if (gameOverCnt_ >= 5.0f)
		{

			SceneManager::GetInstance().ChangeScene(SCENE_ID::TITLE);

			// Releaseを一回だけ通らないようにするフラグをtrueにする
			SceneManager::GetInstance().SetIsFirstRelease(true);

		}

		gameOverCnt_ += deltaTime;

	}

}

void BossBattleScene::Draw(const float deltaTime)
{

	// アクターの管理クラスの更新
	actorManager_->Draw(deltaTime);

	// アクターの生成クラスの描画
	actorCreate_->Draw();

	if (isPlayerDeath_)
	{
		// ゲームオーバーの描画
		DrawGameOver();
	}

}

void BossBattleScene::DrawGameOver()
{

	// 半透明の黒い矩形を画面全体に描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128); // 半透明（128: 50%の透明度）
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ブレンドモード解除

	if (isGameOver_)
	{

		// ブレンドモードの設定
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);

		// ゲームオーバーの描画
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, gameOverImg_, true);

		// ブレンドモードを解除
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// アルファ値を増加（255を上限とする）
		alpha_ += 2;

		// 最少値は255にする
		alpha_ = std::min(alpha_, 255);

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
			DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, scale2_, 0.0, gameOverImg2_, true);

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
	else
	{

		// コンティニュー画像の描画
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 200, 0.8, 0.0, continueImg_, true);

		float yesImgScale = 1.0f;
		float noImgScale = 1.0f;

		// アニメーションの速度
		const float animationSpeed = 0.05f;

		if (isContinue_)
		{
			// YES画像のスケールを時間に応じて変更
			yesImgScale = 1.0 + 0.1f * std::sin(frameCount_ * animationSpeed);
		}
		else
		{
			// NO画像のスケールを時間に応じて変更
			noImgScale = 1.0 + 0.1f * std::sin(frameCount_ * animationSpeed);
		}

		// YES画像の描画
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2 - 300, 500, yesImgScale, 0.0, yesImg_, true);

		// NO画像の描画
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2 + 300, 500, noImgScale, 0.0, noImg_, true);

		// フレームカウントを更新
		frameCount_++;

	}

}
