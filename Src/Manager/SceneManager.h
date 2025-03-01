#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
#include <DxLib.h>

class Fader;
class Camera;
class SceneBase;
class GameScene;
class ActorManager;

// シーン管理用
enum class SCENE_ID
{
	NONE,
	TITLE,
	GAME,
	BOSS_APPEARANCE,
	BOSS_BATTLE,
	GAME_OVER,
	GAME_CLEAR
};

class SceneManager
{

public:

	// インスタンスの生成
	static void CreateInstance();

	// インスタンスの取得
	static SceneManager& GetInstance() { return *instance_; };

	void Init();
	void Init3D();
	void Update(const float deltaTime);
	void Draw(const float deltaTime);

	// リソースの破棄
	void Destroy();

	// 状態遷移
	void ChangeScene(const SCENE_ID& nextId);

	// カメラの取得
	std::weak_ptr<Camera> GetCamera(void) const { return camera_; };

	// シーンIDの取得
	const SCENE_ID& GetSceneID() const { return sceneId_; };

	// シーン遷移
	void DoChangeScene(const SCENE_ID& sceneId);

	// 今のシーンを取得
	const std::shared_ptr<SceneBase>& GetNowScene() const { return scene_; };

	// ゲームパッドで操作するフラグを取得する
	bool GetGamePad(void);

	// ゲームパッドで操作するフラグを取得する
	void SetGamePad(const bool isPad);

	// Releaseを1回だけ通らないようにするフラグ
	const void SetIsFirstRelease(const bool isFirstRelease) { isFirstRelease_ = isFirstRelease; }

	// プレイヤーの必殺技ゲージを取得
	const int GetPlayerSpecialAttackGauge()const { return playerSpecialAttackGauge_; }

	// プレイヤーの必殺技ゲージを設定
	void SetPlayerSpecialAttackGauge(const int playerSpecialAttackGauge) { playerSpecialAttackGauge_ = playerSpecialAttackGauge; }

	// プレイヤーのHPを取得
	const int GetPlayerHp()const { return playerHp_; }

	// プレイヤーのHPを設定
	void SetPlayerHp(const int playerHp) { playerHp_ = playerHp; }

private:

	// 正面から斜め下方向
	static constexpr VECTOR LIGHT_DIR = { 0.0f, -1.0f, 1.0f };

	// 背景色
	static constexpr int COLOR_MAX = 255;

	// 静的インスタンス
	static SceneManager* instance_;

	// シーンID 
	SCENE_ID sceneId_;

	// 待機中のシーンID
	SCENE_ID waitSceneId_;

	// フェード
	std::unique_ptr<Fader> fader_;

	// カメラ
	std::shared_ptr<Camera> camera_;

	// シーンの基底クラス
	std::shared_ptr<SceneBase> scene_;

	// 状態遷移
	std::unordered_map<SCENE_ID, std::function<void(void)>> sceneChange_;
	void ChangeTitleScene();
	void ChangeGameScene();
	void ChangeBossAppearanceScene();
	void ChangeBossBattleScene();
	void ChangeGameOverScene();
	void ChangeGameClearScene();

	// シーン遷移中判定
	bool isSceneChanging_;

	// ゲームパッドで操作するためのフラグ
	bool isGamePad_;

	// Releaseを1回だけ通らないようにするフラグ
	bool isFirstRelease_;

	// プレイヤーの必殺技ゲージ
	int playerSpecialAttackGauge_;

	// プレイヤーのHP
	int playerHp_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager();
	// コピーコンストラクタも同様
	SceneManager(const SceneManager&);
	// デストラクタも同様
	~SceneManager() = default;

	// フェード
	void Fade(void);

};