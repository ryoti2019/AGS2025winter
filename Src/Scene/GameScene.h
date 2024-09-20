#pragma once
#include <memory>
#include "../Scene/SceneBase.h"

class ActorManager;

class GameScene :public SceneBase
{

public:

	// コンストラクタ
	GameScene();

	// デストラクタ
	~GameScene();

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw() override;

private:

	// アクターの管理クラス
	std::shared_ptr<ActorManager> actorManager_;

};