#pragma once
#include <memory>
#include "../Manager/Camera.h"
#include "../Manager/ResourceManager.h"
#include "../Object/Common/InputController.h"

class ActorManager;
class CollisionManager;

class SceneBase
{

public:

	// コンストラクタ
	SceneBase();

	// デストラクタ
	virtual ~SceneBase() = 0;

	// 初期化処理
	virtual void Init() = 0;

	// 更新ステップ
	virtual void Update(const float deltaTime) = 0;

	// 描画処理
	virtual void Draw(const float deltaTime) = 0;

	// アクターの管理クラスを取得
	const std::shared_ptr<ActorManager>& GetActorManager() const { return actorManager_; }

	// 衝突判定の管理クラスを取得
	const std::shared_ptr<CollisionManager>& GetCollisionManager() const { return collisionManager_; }

protected:

	// リソース管理
	ResourceManager& resMng_;

	//カメラ
	std::unique_ptr<Camera> camera_;

	// アクターの管理クラス
	std::shared_ptr<ActorManager> actorManager_;

	// 衝突判定の管理クラス
	std::shared_ptr<CollisionManager> collisionManager_;

	// 入力用コントローラー
	std::unique_ptr<InputController> inputController_;

	// BGM
	int bgm_;

	// 決定ボタンのSE
	int decideSE_;

	// 画像の初期化
	virtual void InitImage();

	// BGMとSEの初期化
	virtual void InitBGMAndSE();

private:

};