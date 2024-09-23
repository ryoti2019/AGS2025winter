#include <DxLib.h>
#include "../Manager/ActorManager.h"
#include "GameScene.h"

GameScene::GameScene()
{
	// アクター管理クラス
	actorManager_ = std::make_shared<ActorManager>();
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
}

void GameScene::Update(const float deltaTime)
{
	actorManager_->Update(deltaTime);
}

void GameScene::Draw()
{
	actorManager_->Draw();

	DrawFormatString(0, 0, 0xff0000, "GameScene");
}