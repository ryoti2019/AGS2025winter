#pragma once
#include "SceneBase.h"

class GameClearActorCreate;

class GameClearScene : public SceneBase
{

public:

	// コンストラクタ
	GameClearScene();

	// デストラクタ
	~GameClearScene()override = default;

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime) override;

	// アクターの管理クラスを取得
	const std::shared_ptr<ActorManager>& GetActorManager() const { return actorManager_; }

	// ボス登場用のボスのみの生成クラスを取得
	const std::shared_ptr<GameClearActorCreate>& GetActorCreate() const { return actorCreate_; }

private:

	// サウンドの最大音量
	static constexpr int SOUND_MAX = 255;

	// BGMの音量の割合
	static constexpr float SOUND_BGM_VOLUME = 0.5f;

	// ゲームクリア画像のスクリーンX座標
	static constexpr int GAME_CLEAR_X = 850;

	// ゲームクリア画像のスクリーンX座標
	static constexpr int GAME_CLEAR_Y = 200;
	
	// ゲームクリア画像のスケール
	static constexpr double GAME_CLEAR_SCALE = 2.0;

	// ゲームクリア画像のアニメーションスピード
	static constexpr float GAME_CLEAR_ANIM_SPEED = 0.05f;

	// ゲームクリア画像の通常スケール
	static constexpr float GAME_CLEAR_IMG_SCALE = 1.0f;

	// ゲームクリア画像のスケール変化量
	static constexpr float SCALE_AMPLITUDE = 0.1f;

	// Press A Button or Spaceの画像のスクリーンのX座標
	static constexpr float PRESS_A_BUTTON_OR_SPACE_X = 850;

	// Press A Button or Spaceの画像のスクリーンのY座標
	static constexpr float PRESS_A_BUTTON_OR_SPACE_Y = 500;

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

