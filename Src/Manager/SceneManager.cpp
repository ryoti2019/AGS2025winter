#include <chrono>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "ResourceManager.h"
#include "../Common/Fader.h"
#include "Camera.h"
#include "SceneManager.h"
#include "../Scene/SceneBase.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

void SceneManager::Init()
{

	// シーンID
	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	//関数ポインタの初期化
	sceneChange_.emplace(SCENE_ID::TITLE, std::bind(&SceneManager::ChangeTitleScene, this));
	sceneChange_.emplace(SCENE_ID::GAME, std::bind(&SceneManager::ChangeGameScene, this));
	//sceneChange_.emplace(SCENE_ID::GAMEOVER, std::bind(&SceneManager::ChangeGameOverScene, this));

	// フェーダーの初期化
	fader_ = std::make_unique<Fader>();
	
	// カメラの初期化
	camera_ = std::make_shared<Camera>();

	// シーンの初期化
	scene_ = std::make_unique<TitleScene>();
	scene_->Init();

	// シーン遷移中判定
	isSceneChanging_ = false;

	// 3D用の設定
	Init3D();

	// 初期シーンの設定
	DoChangeScene(SCENE_ID::TITLE);

	// ゲームパッドを使うときtrue
	isGamePad_ = false;

	// 操作説明のフラグ
	isOperation_ = true;

}

void SceneManager::Init3D()
{

	// 背景色設定
	SetBackgroundColor(255, 255, 255);

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	// ライトの設定
	SetUseLighting(true);

	// 正面から斜め下に向かったライト
	ChangeLightTypeDir({ 0.0f, -1.0f, 1.0f });

}

void SceneManager::Update(const float deltaTime)
{

	// nullの場合は通らない
	if (scene_ == nullptr)return;
	if (fader_ == nullptr)return;

	// シーン遷移以外は更新
	fader_->Update();
	if (isSceneChanging_)
	{
		Fade();
	}
	else
	{
 		scene_->Update(deltaTime);
	}

	// カメラ更新
	camera_->Update();

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();

}

void SceneManager::Draw(const float deltaTime)
{

	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(DX_SCREEN_BACK);

	// 画面を初期化
	ClearDrawScreen();

	// カメラ設定
	camera_->SetBeforeDraw(deltaTime);

	// 描画
	scene_->Draw(deltaTime);

	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();

	// 暗転・明転
	fader_->Draw();

}

void SceneManager::Destroy()
{
	delete instance_;
}

void SceneManager::ChangeScene(const SCENE_ID& nextId)
{

	// フェード処理が終わってからシーンを変える場合もあるため、
	// 遷移先シーンをメンバ変数に保持
	waitSceneId_ = nextId;

	// フェードアウト(暗転)を開始する
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;

}

SceneManager::SceneManager()
	:
	sceneId_(SCENE_ID::NONE),
	waitSceneId_(SCENE_ID::NONE),
	scene_(nullptr),
	isSceneChanging_(false),
	isGamePad_(false),
	isOperation_(false)
{
}

void SceneManager::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		// 明転中
		if (fader_->IsEnd())
		{
			// 明転が終了したら、フェード処理終了
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// 暗転中
		if (fader_->IsEnd())
		{
			// 完全に暗転してからシーン遷移
			DoChangeScene(waitSceneId_);
			// 暗転から明転へ
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}

}

void SceneManager::DoChangeScene(const SCENE_ID& sceneId)
{

	// リソースの解放
	ResourceManager::GetInstance().Release();

	// シーンを変更する
	sceneId_ = sceneId;

	// シーンを変更
	sceneChange_[sceneId_]();

	// シーンを初期化
	scene_->Init();

	// 待機中のシーンIDを初期化
	waitSceneId_ = SCENE_ID::NONE;

}

bool SceneManager::GetGamePad(void)
{
	return isGamePad_;
}

void SceneManager::SetGamePad(bool isPad)
{
	isGamePad_ = isPad;
}

void SceneManager::ChangeTitleScene()
{
	scene_ = std::make_unique<TitleScene>();
}

void SceneManager::ChangeGameScene()
{
	scene_ = std::make_unique<GameScene>();
}