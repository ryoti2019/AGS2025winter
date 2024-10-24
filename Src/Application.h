#pragma once
#include <string>
#include <chrono>

class Application
{

public:

	// スクリーンサイズ
	static constexpr int SCREEN_SIZE_X = 1280;
	static constexpr int SCREEN_SIZE_Y = 720;

	// データパス関連
	//-------------------------------------------
	static const std::string PATH_IMAGE;
	static const std::string PATH_MODEL;
	static const std::string PATH_EFFECT;
	static const std::string PATH_SOUND;
	static const std::string PATH_JSON;
	//-------------------------------------------

	// 明示的にインスタンスを生成する
	static void CreateInstance();

	// 静的インスタンスの取得
	static Application& GetInstance();

	// 初期化
	void Init();

	// ゲームループの開始
	void Run();

	// リソースの破棄
	void Destroy();

	// 初期化成功／失敗の判定
	bool IsInitFail() const { return isInitFail_; }

	// 解放成功／失敗の判定
	bool IsReleaseFail() const {return isReleaseFail_; }

	// デルタタイムの取得
	float GetDeltaTime() const { return deltaTime_; };

private:

	// 静的インスタンス
	static Application* instance_;

	// 初期化失敗
	bool isInitFail_;

	// 解放失敗
	bool isReleaseFail_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	Application();

	// コピーコンストラクタも同様
	Application(const Application&) = delete;

	// デストラクタも同様
	~Application() = default;

};

