#pragma once
#include "SceneBase.h"

class ActorManager;
class BossAppearanceCreate;
class CollisionManager;

class BossAppearanceScene : public SceneBase
{

public:

	// コンストラクタ
	BossAppearanceScene();

	// デストラクタ
	~BossAppearanceScene()override = default;

	void Init() override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime) override;

	// ボス登場用のボスのみの生成クラスを取得
	const std::shared_ptr<BossAppearanceCreate>& GetActorCreate() const { return actorCreate_; }

private:

	// ボス登場用のボスのみの生成クラス
	std::shared_ptr<BossAppearanceCreate> actorCreate_;

	// ボスが現れた!!用の画像
	int appearanceImg_;

	// ボスが現れた!!用の後ろから出てくる画像2
	int appearance2Img_;

	// 画像の透明度
	int alpha_;

	// 2枚目の画像の透明度
	int alpha2_;

	// 画像のスケール
	float scale_;

	// 2枚目の画像のスケール
	float scale2_;

	// 画像の初期化
	void InitImage()override;

	// BGMとSEの初期化
	void InitBGMAndSE()override;

};

