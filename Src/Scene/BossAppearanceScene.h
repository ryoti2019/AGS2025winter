#pragma once
#include "SceneBase.h"

class ActorManager;
class BossAppearanceCreate;
class CollisionManager;

class BossAppearanceScene : public SceneBase
{

public:

	BossAppearanceScene();

	~BossAppearanceScene() = default;

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime) override;

	// アクターの管理クラスを取得
	const std::shared_ptr<ActorManager>& GetActorManager() const { return actorManager_; }

	// 衝突判定の管理クラスを取得
	const std::shared_ptr<CollisionManager>& GetCollisionManager() const { return collisionManager_; }

	// ボス登場用のボスのみの生成クラスを取得
	const std::shared_ptr<BossAppearanceCreate>& GetActorCreate() const { return actorCreate_; }

private:

	// アクターの管理クラス
	std::shared_ptr<ActorManager> actorManager_;

	// 衝突判定の管理クラス
	std::shared_ptr<CollisionManager> collisionManager_;

	// ボス登場用のボスのみの生成クラス
	std::shared_ptr<BossAppearanceCreate> actorCreate_;

};

