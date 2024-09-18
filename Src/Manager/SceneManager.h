#pragma once
#include <functional>
#include <memory>
#include <unordered_map>

class SceneBase;
class ActorManager;
class GameScene;

// シーン管理用
enum class SCENE_ID
{
	NONE,
	TITLE,
	GAME,
	GAMEOVER,
};

class SceneManager
{

public:

	// インスタンスの生成
	static void CreateInstance();

	// インスタンスの取得
	static SceneManager& GetInstance() { return *instance_; };

	void Init();
	void Update(const float deltaTime);
	void Draw();

	// リソースの破棄
	void Destroy();

	// 状態遷移
	void ChangeScene(const SCENE_ID& nextId) 
	{

		// フェード処理が終わってからシーンを変える場合もあるため、
		// 遷移先シーンをメンバ変数に保持
		waitSceneId_ = nextId;

		isSceneChanging_ = true;

	};

	// シーンIDの取得
	const SCENE_ID& GetSceneID() const { return sceneId_; };

	// シーン遷移
	void DoChangeScene(const SCENE_ID& sceneId);

	// 今のシーンを取得
	const std::shared_ptr<SceneBase>& GetNowScene() const { return scene_; };

private:

	// 静的インスタンス
	static SceneManager* instance_;

	// シーンID 
	SCENE_ID sceneId_;

	// 待機中のシーンID
	SCENE_ID waitSceneId_;

	// 状態遷移
	std::unordered_map<SCENE_ID, std::function<void(void)>> sceneChange_;
	void ChangeTitleScene();
	void ChangeGameScene();
	void ChangeGameOverScene();

	// シーンの基底クラス
	std::shared_ptr<SceneBase> scene_;

	// シーン遷移中判定
	bool isSceneChanging_;

	// ゲームパッドで操作するためのフラグ
	bool isGamePad_;

	// 操作説明のフラグ
	bool isOperation_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager();
	// コピーコンストラクタも同様
	SceneManager(const SceneManager&);
	// デストラクタも同様
	~SceneManager() = default;

};