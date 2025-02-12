#pragma once
#include <memory>
#include "../Scene/SceneBase.h"

class ActorCreate;

class GameScene :public SceneBase
{

public:

	// 1枚目の画像スケール
	static constexpr float SCALE = 2.0f;

	// 2枚目の画像スケール
	static constexpr float SCALE2 = 1.0f;

	// 2枚目の画像の透明度
	static constexpr int ALPHA_MAX = 255;

	// 音の最大音量
	static constexpr int SOUND_MAX = 255;

	// BGMの音量の割合
	static constexpr float BGM_VOLUME = 0.5f;

	// ゲームオーバーに遷移するまでの時間
	static constexpr float GAME_OVER_DELAY = 5.0f;

	// 操作説明の時の後ろの画面の透明度
	static constexpr int USER_GUIDE_ALPHA = 200;

	// 操作説明画像のスケール
	static constexpr float USER_GUIDE_IMAGE_SCALE = 0.5f;

	// 操作説明のX座標
	static constexpr int USER_GUIDE_POS_X = 150;

	// 操作説明のY座標
	static constexpr int USER_GUIDE_POS_Y = 200;

	// ゲームオーバーの時の後ろの画面の透明度
	static constexpr int GAME_OVER_ALPHA = 128;

	// 画像1の透明度の増える量
	static constexpr int ALPHA_INCREMENT = 2;

	// 画像1の透明度の減る量
	static constexpr float ALPHA_DECREMENRT = -0.02f;

	// 画像1のスケールの最大値
	static constexpr float SCALE_MAX = 1.0f;

	// 画像2のスケールの最大値
	static constexpr float SCALE2_MAX = 3.0f;

	// 画像1の透明度の増える量
	static constexpr float ALPHA2_INCREMENT = 0.05f;

	// 画像1の透明度の減る量
	static constexpr int ALPHA2_DECREMENRT = -10;

	// コンティニュー画像のY座標
	static constexpr int CONTINUE_POS_Y = 200;

	// コンティニュー画像のスケール
	static constexpr float CONTINUE_IMAGE_SCALE = 0.8f;

	// YES画像のスケール
	static constexpr float YES_IMAGE_SCALE = 1.0f;

	// NO画像のスケール
	static constexpr float NO_IMAGE_SCALE = 1.0f;

	// YES画像のX座標のオフセット
	static constexpr int YES_POS_X_OFFSET = 300;

	// NO画像のX座標のオフセット
	static constexpr int NO_POS_X_OFFSET = 300;

	// YES画像のY座標
	static constexpr int YES_POS_Y = 500;

	// NO画像のY座標
	static constexpr int NO_POS_Y = 500;

	// YESとNO画像のスケールの変動量
	static constexpr float SCLAE_VARIATION = 0.1f;

	// YESとNO画像のアニメーション速度
	static constexpr float ANIM_SPEED = 0.05f;

	// コンストラクタ
	GameScene();

	// デストラクタ
	~GameScene()override = default;

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime) override;

	// アクターの生成クラスを取得
	const std::shared_ptr<ActorCreate>& GetActorCreate() const { return actorCreate_; }

	// 操作説明を見ているかのフラグ
	const bool GetIsViewUserGuide()const { return isViewUserGuide_; }

	// プレイヤーが死んだときのフラグを取得
	const bool GetIsPlayerDeath() const { return isPlayerDeath_; }

	// プレイヤーが死んだときのフラグを設定
	void SetIsPlayerDeath(const bool isPlayerDeath) { isPlayerDeath_ = isPlayerDeath; }

private:

	// アクターの生成クラス
	std::shared_ptr<ActorCreate> actorCreate_;

	// キーボードの操作説明
	int keyboardUserGuideImg_;

	// キーボードの移動の操作説明
	int keyboardUserGuideMoveImg_;

	// キーボードの攻撃の操作説明
	int keyboardUserGuideAttackImg_;

	// キーボードの必殺技の操作説明
	int keyboardUserGuideSpecialAttackImg_;

	// ゲームパッドの操作説明
	int gamePadUserGuideImg_;

	// ゲームパッドの移動の操作説明
	int gamePadUserGuideMoveImg_;

	// ゲームパッドの攻撃の操作説明
	int gamePadUserGuideAttackImg_;

	// ゲームパッドの必殺技の操作説明
	int gamePadUserGuideSpecialAttackImg_;

	// ゲームオーバーの画像
	int gameOverImg_;

	// ゲームオーバーの画像2
	int gameOverImg2_;

	// 画像の透明度
	int alpha_;

	// 2枚目の画像の透明度
	int alpha2_;

	// 画像のスケール
	float scale_;

	// 2枚目の画像のスケール
	float scale2_;

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

	// 操作説明を見ているかのフラグ
	bool isViewUserGuide_;

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

	// ボスの登場シーンに遷移するか判定
	void CheckTransitionBossAppearanceScene();

	// コンティニューかゲームオーバーを選ばせる処理
	void SelectContinueOrGameOver(const float deltaTime);

	// 操作説明の描画
	void DrawUserGuide();
	
	// ゲームオーバーの描画
	void DrawGameOver();

};