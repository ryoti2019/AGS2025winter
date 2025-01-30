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

	// アクターの生成クラス
	std::shared_ptr<BossBattleActorCreate> actorCreate_;

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

