#include <DxLib.h>
#include "../Manager/ActorManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Common/ActorCreate.h"
#include "GameScene.h"
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

	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::FOLLOW);

}

void GameScene::Update(const float deltaTime)
{

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

	DrawFormatString(0, 0, 0xff0000, "GameScene");
}