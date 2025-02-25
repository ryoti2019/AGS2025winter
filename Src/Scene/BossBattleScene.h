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

	// プレイヤーが死んだときのフラグを取得
	const bool GetIsPlayerDeath() const { return isPlayerDeath_; }

	// プレイヤーが死んだときのフラグを設定
	void SetIsPlayerDeath(const bool isPlayerDeath) { isPlayerDeath_ = isPlayerDeath; }

private:

	// サウンドの最大音量
	static constexpr int SOUND_MAX = 255;

	// BGMの音量の割合
	static constexpr float SOUND_BGM_VOLUME = 0.5f;

	// ゲームオーバーに遷移するまでの時間
	static constexpr float GAME_OVER_TRANSITION_TIME = 5.0f;

	// ゲームオーバーの時の後ろの画面の透明度
	static constexpr int BOSS_BATTLE_ALPHA = 128;

	// ゲームオーバーの画像のスケール
	static constexpr double BOSS_BATTLE_IMG_SCALE = 1.0;

	// 1枚目の画像の透明度の増加量
	static constexpr int IMG1_ALPHA_INCREMENT = 2;

	// 1枚目の画像のアルファの最大値
	static constexpr int IMG1_ALPHA_MAX = 255;

	// 1枚目の画像のスケールの減少量
	static constexpr float IMG1_SCALE_DECREASE = 0.02f;

	// 1枚目の画像のスケールの最小値
	static constexpr float IMG1_SCALE_MIN = 1.0f;

	// 2枚目の画像のスケールの最大値
	static constexpr float IMG2_SCALE_MAX = 3.0f;

	// 2枚目の画像のアルファ値の減少量
	static constexpr int IMG2_ALPHA_DECREASE = 10;

	// 2枚目の画像のスケールの減少量
	static constexpr float IMG2_SCALE_INCREMENT = 0.05f;

	// コンティニュー画像のスクリーンY座標
	static constexpr int CONTINUE_IMG_Y = 200;

	// コンティニュー画像のスケール
	static constexpr double CONTINUE_SCALE = 0.8;

	// YESとNO画像の通常スケール
	static constexpr float YES_NO_IMG_SCALE = 1.0f;

	// YESとNO画像のアニメーションスピード
	static constexpr float YES_NO_IMG_ANIM_SPEED = 0.05f;

	// YESとNO画像のスケール変化量
	static constexpr float SCALE_AMPLITUDE = 0.1f;

	// YESとNO画像のスクリーンX座標のオフセット
	static constexpr int YES_NO_IMG_X_OFFSET = 300;

	// YESとNO画像のスクリーンY座標
	static constexpr int YES_NO_IMG_Y = 500;

	// アクターの生成クラス
	std::shared_ptr<BossBattleActorCreate> actorCreate_;

	// ゲームオーバーの画像
	int gameOverImg_;

	// ゲームオーバーの画像2
	int gameOverImg2_;

	// 画像の透明度
	int img1Alpha_;

	// 2枚目の画像の透明度
	int img2Alpha_;

	// 画像のスケール
	float img1Scale_;

	// 2枚目の画像のスケール
	float img2Scale_;

	// コンティニューの画像
	int continueImg_;

	// Yesの画像
	int yesImg_;

	// Noの画像
	int noImg_;

	// 画像のカウンタ
	int frameCount_;

	// ゲームオーバーボイス
	int gameOverVoice_;

	// カーソル移動のSE
	int cursorMoveSE_;

	// ゲームオーバーを表示し続けるためのカウンタ
	float gameOverCnt_;

	// プレイヤーが死んだときのフラグ
	bool isPlayerDeath_;

	// コンティニューするかのフラグ
	bool isContinue_;

	// ゲームオーバーのフラグ
	bool isGameOver_;

	// 画像の初期化
	void InitImage()override;

	// BGMとSEの初期化
	void InitBGMAndSE()override;

	// ゲームクリアシーンに遷移するか判定
	void CheckTransitionGameClearScene();

	// コンティニューかゲームオーバーを選ばせる処理
	void SelectContinueOrGameOver(const float deltaTime);

	// ゲームオーバーの描画
	void DrawGameOver();

};

