#pragma once
#include <memory>
#include "../Scene/SceneBase.h"

class ActorCreate;

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

	// アクターの生成クラスを取得
	const std::shared_ptr<ActorCreate>& GetActorCreate() const { return actorCreate_; }

private:

	// アクターの生成クラス
	std::shared_ptr<ActorCreate> actorCreate_;

};