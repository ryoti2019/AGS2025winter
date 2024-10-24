#include <DxLib.h>
#include "../Manager/ActorManager.h"
#include "../Manager/CollisionManager.h"
#include "GameScene.h"

GameScene::GameScene()
{

	// アクター管理クラス
	actorManager_ = std::make_shared<ActorManager>();

	// 衝突判定管理クラス
	collisionManager_ = std::make_shared<CollisionManager>();

}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
}

void GameScene::Update(const float deltaTime)
{

	// アクターの管理クラスの更新
	actorManager_->Update(deltaTime);

	// 衝突判定の管理クラスの更新
	collisionManager_->Update();

}

void GameScene::Draw()
{
	actorManager_->Draw();

	DrawFormatString(0, 0, 0xff0000, "GameScene");
}