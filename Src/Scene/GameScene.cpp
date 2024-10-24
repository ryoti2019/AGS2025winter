#include <DxLib.h>
#include "../Manager/ActorManager.h"
#include "../Manager/CollisionManager.h"
#include "GameScene.h"

GameScene::GameScene()
{

	// �A�N�^�[�Ǘ��N���X
	actorManager_ = std::make_shared<ActorManager>();

	// �Փ˔���Ǘ��N���X
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

	// �A�N�^�[�̊Ǘ��N���X�̍X�V
	actorManager_->Update(deltaTime);

	// �Փ˔���̊Ǘ��N���X�̍X�V
	collisionManager_->Update();

}

void GameScene::Draw()
{
	actorManager_->Draw();

	DrawFormatString(0, 0, 0xff0000, "GameScene");
}