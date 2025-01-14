#pragma once
#include "../Scene/SceneBase.h"

class TitleActorCreate;

class TitleScene : public SceneBase
{

public:

	// コンストラクタ
	TitleScene();

	// デストラクタ
	~TitleScene() = default;

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime) override;

	// アクターの管理クラスを取得
	const std::shared_ptr<ActorManager>& GetActorManager() const { return actorManager_; }

	// ボス登場用のボスのみの生成クラスを取得
	const std::shared_ptr<TitleActorCreate>& GetActorCreate() const { return actorCreate_; }

private:

	// アクターの管理クラス
	std::shared_ptr<ActorManager> actorManager_;

	// タイトルシーンのアクター生成クラス
	std::shared_ptr<TitleActorCreate> actorCreate_;

	// タイトルロゴの画像
	int titleLogoImg_;

	// Press A Bottonの画像
	int pressABottonImg_;

	// Press A Botton用のカウンタ
	int frameCount_;

	// 画像の初期化
	void InitImage()override;

	// BGMとSEの初期化
	void InitBGMAndSE()override;

};

