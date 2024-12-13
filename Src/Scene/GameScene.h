#pragma once
#include <memory>
#include "../Scene/SceneBase.h"

class ActorManager;
class ActorCreate;
class CollisionManager;

class GameScene :public SceneBase
{

public:

	// コンストラクタ
	GameScene();

	// デストラクタ
	~GameScene();

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime) override;

	// アクターの管理クラスを取得
	const std::shared_ptr<ActorManager>& GetActorManager() const { return actorManager_; }

	// 衝突判定の管理クラスを取得
	const std::shared_ptr<CollisionManager>& GetCollisionManager() const { return collisionManager_; }

	// アクターの生成クラスを取得
	const std::shared_ptr<ActorCreate>& GetActorCreate() const { return actorCreate_; }

private:

	// アクターの管理クラス
	std::shared_ptr<ActorManager> actorManager_;

	// 衝突判定の管理クラス
	std::shared_ptr<CollisionManager> collisionManager_;

	// アクターの生成クラス
	std::shared_ptr<ActorCreate> actorCreate_;

};