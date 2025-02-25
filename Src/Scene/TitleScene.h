#pragma once
#include "../Scene/SceneBase.h"

class TitleActorCreate;

class TitleScene : public SceneBase
{

public:

	// コンストラクタ
	TitleScene();

	// デストラクタ
	~TitleScene()override = default;

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime) override;

	// アクターの管理クラスを取得
	const std::shared_ptr<ActorManager>& GetActorManager() const { return actorManager_; }

	// ボス登場用のボスのみの生成クラスを取得
	const std::shared_ptr<TitleActorCreate>& GetActorCreate() const { return actorCreate_; }

private:

	// サウンドの最大音量
	static constexpr int SOUND_MAX = 255;

	// BGMの音量の割合
	static constexpr float SOUND_BGM_VOLUME = 0.5f;

	// タイトルロゴのスクリーンX座標
	static constexpr int TITLE_LOGO_X = 850;

	// タイトルロゴのスクリーンY座標
	static constexpr int TITLE_LOGO_Y = 300;

	// タイトルロゴのスケール
	static constexpr float TITLE_LOGO_SCALE = 1.0f;

	// YESとNO画像のスケール変化量
	static constexpr float SCALE_AMPLITUDE = 0.1f;

	// Press A Button or Spaceの画像のスクリーンX座標
	static constexpr float PRESS_A_BUTTON_OR_SPACE_X = 850;

	// Press A Button or Spaceの画像のスクリーンY座標
	static constexpr float PRESS_A_BUTTON_OR_SPACE_Y = 500;

	// アクターの管理クラス
	std::shared_ptr<ActorManager> actorManager_;

	// タイトルシーンのアクター生成クラス
	std::shared_ptr<TitleActorCreate> actorCreate_;

	// タイトルロゴの画像
	int titleLogoImg_;

	// Press A Button or Spaceの画像
	int pressAButtonOrSpaceImg_;

	// Press A Button or Spaceのカウンタ
	int frameCount_;

	// 画像の初期化
	void InitImage()override;

	// BGMとSEの初期化
	void InitBGMAndSE()override;

};

