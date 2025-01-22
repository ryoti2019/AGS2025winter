#pragma once
#include "SceneBase.h"

class ActorManager;
class BossAppearanceCreate;
class CollisionManager;

class BossAppearanceScene : public SceneBase
{

public:

	// コンストラクタ
	BossAppearanceScene();

	// デストラクタ
	~BossAppearanceScene()override = default;

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime) override;

	// ボス登場用のボスのみの生成クラスを取得
	const std::shared_ptr<BossAppearanceCreate>& GetActorCreate() const { return actorCreate_; }

private:

	// ボス登場用のボスのみの生成クラス
	std::shared_ptr<BossAppearanceCreate> actorCreate_;

	// BGMとSEの初期化
	void InitBGMAndSE()override;

};

