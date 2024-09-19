#include <chrono>
#include <DxLib.h>
#include "ResourceManager.h"
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
	sceneChange_.emplace(SCENE_ID::GAMEOVER, std::bind(&SceneManager::ChangeGameOverScene, this));

	// 初期化
	scene_ = std::make_unique<TitleScene>();
	scene_->Init();

	// シーン遷移中判定
	isSceneChanging_ = false;

	// 初期シーンの設定
	DoChangeScene(SCENE_ID::TITLE);

	// ゲームパッドを使うときtrue
	isGamePad_ = false;

	// 操作説明のフラグ
	isOperation_ = true;

}

void SceneManager::Update(const float deltaTime)
{

	// nullの場合は通らない
	if (scene_ == nullptr)
	{
		return;
	}

	// シーン遷移以外は更新
	if (!isSceneChanging_)
	{
		scene_->Update(deltaTime);
	}

}

void SceneManager::Draw()
{

	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(DX_SCREEN_BACK);

	// 画面を初期化
	ClearDrawScreen();

	// 描画
	scene_->Draw();

}

void SceneManager::Destroy()
{
	delete instance_;
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

void SceneManager::DoChangeScene(const SCENE_ID& sceneId)
{

	// リソースの解放
	ResourceManager::GetInstance().Release();

	// シーンを変更する
	sceneId_ = sceneId;

	//// 現在のシーンを解放
	//if (scene_ != nullptr)
	//{
	//	scene_->Release();
	//}

	// シーンを変更
	sceneChange_[sceneId_]();

	// シーンを初期化
	scene_->Init();

	// 待機中のシーンIDを初期化
	waitSceneId_ = SCENE_ID::NONE;

}

void SceneManager::ChangeTitleScene()
{
	scene_ = std::make_unique<TitleScene>();
}

void SceneManager::ChangeGameScene()
{
	scene_ = std::make_unique<GameScene>();
}

void SceneManager::ChangeGameOverScene()
{
}
