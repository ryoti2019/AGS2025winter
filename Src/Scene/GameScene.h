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