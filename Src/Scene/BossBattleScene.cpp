#include "../Manager/ActorManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Common/BossBattleActorCreate.h"
#include "BossBattleScene.h"

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

}

void BossBattleScene::Update(const float deltaTime)
{

	// アクターの管理クラスの更新
	actorManager_->Update(deltaTime);

	// 衝突判定の管理クラスの更新
	collisionManager_->Update(deltaTime);

	// アクターの生成クラスの更新
	actorCreate_->Update();

}

void BossBattleScene::Draw(const float deltaTime)
{

	// アクターの管理クラスの更新
	actorManager_->Draw(deltaTime);

	// アクターの生成クラスの描画
	actorCreate_->Draw();

	DrawFormatString(0, 0, 0xff0000, "BossBattleScene");

}
