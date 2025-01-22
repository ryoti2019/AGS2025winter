#pragma once
#include "SceneBase.h"

class BossBattleActorCreate;

class BossBattleScene : public SceneBase
{

public:

	// コンストラクタ
	BossBattleScene();

	// デストラクタ
	~BossBattleScene()override = default;

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime) override;

	// アクターの生成クラスを取得
	const std::shared_ptr<BossBattleActorCreate>& GetActorCreate() const { return actorCreate_; }

private:

	// アクターの生成クラス
	std::shared_ptr<BossBattleActorCreate> actorCreate_;

	// BGMとSEの初期化
	void InitBGMAndSE()override;

};

