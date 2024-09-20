#include <DxLib.h>
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
}

void GameScene::Draw()
{
	DrawFormatString(0, 0, 0xff0000, "GameScene");
}