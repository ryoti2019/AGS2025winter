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

	// 操作説明を見ているかのフラグ
	bool isViewUserGuide_;

	// 画像の初期化
	void InitImage()override;

	// BGMとSEの初期化
	void InitBGMAndSE()override;

};