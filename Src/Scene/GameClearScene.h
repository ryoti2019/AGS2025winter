#pragma once
#include "SceneBase.h"

class GameClearActorCreate;

class GameClearScene : public SceneBase
{

public:

	// コンストラクタ
	GameClearScene();

	// デストラクタ
	//~GameClearScene() override = default;
	~GameClearScene() override
	{
		int a = actorManager_.use_count();
		int b = actorCreate_.use_count();
	}

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime) override;

	// アクターの管理クラスを取得
	const std::shared_ptr<ActorManager>& GetActorManager() const { return actorManager_; }

	// ボス登場用のボスのみの生成クラスを取得
	const std::shared_ptr<GameClearActorCreate>& GetActorCreate() const { return actorCreate_; }

private:

	// アクターの管理クラス
	std::shared_ptr<ActorManager> actorManager_;

	// タイトルシーンのアクター生成クラス
	std::shared_ptr<GameClearActorCreate> actorCreate_;

	// ゲームクリアの画像
	int gameClearImg_;

	// Press A Button or Spaceの画像
	int pressAButtonOrSpaceImg_;

	// Press A Button or Space用のカウンタ
	int frameCount_;

	// 画像の初期化
	void InitImage()override;

	// BGMとSEの初期化
	void InitBGMAndSE()override;


};

