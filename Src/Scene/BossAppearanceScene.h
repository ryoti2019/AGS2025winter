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

	// サウンドの最大音量
	static constexpr int SOUND_MAX = 255;

	// BGMの音量の割合
	static constexpr float SOUND_BGM_VOLUME = 0.5f;

	// ボスの名前を描画が始まる時間
	static constexpr float BOSS_NAME_DRAW_START_TIME = 6.0f;

	// ボスが現れた!!用の画像のスクリーンY座標
	static constexpr int BOSS_APPEAR_TEXT_Y = 500;

	// 1枚目の画像のアルファ値の増加量
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

	// ボス登場用のボスのみの生成クラス
	std::shared_ptr<BossAppearanceCreate> actorCreate_;

	// ボスが現れた!!用の画像
	int appearanceImg_;

	// ボスが現れた!!用の後ろから出てくる画像2
	int appearance2Img_;

	// 1枚目の画像の透明度
	int img1Alpha_;

	// 2枚目の画像の透明度
	int img2Alpha_;

	// 1枚目の画像のスケール
	float img1Scale_;

	// 2枚目の画像のスケール
	float img2Scale_;

	// 画像の初期化
	void InitImage()override;

	// BGMとSEの初期化
	void InitBGMAndSE()override;

};

