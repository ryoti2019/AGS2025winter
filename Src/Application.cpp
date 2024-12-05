#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Manager/ResourceManager.h"
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
#include "../Lib/ImGuiWrapper.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

const std::string Application::PATH_IMAGE = "Data/Image/";
const std::string Application::PATH_MODEL = "Data/Model/";
const std::string Application::PATH_EFFECT = "Data/Effect/";
const std::string Application::PATH_SOUND = "Data/Music/";
const std::string Application::PATH_JSON = "Data/Json/";

void Application::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	//instance_->Init();
}

Application& Application::GetInstance()
{
	return *instance_;
}

void Application::Init()
{
}

void Application::Run()
{

	auto& inputManager = InputManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& imGuiWrapper = ImGuiWrapper::GetInstance();

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		// 本番は上にする
		// デルタタイム
		//auto nowTime = std::chrono::system_clock::now();
		//deltaTime_ = static_cast<float>(
			//std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
			
		//preTime_ = nowTime;
		
		// デバッグの時使う
		deltaTime_ = 1.0f / 60.0f;

		inputManager.Update();
		imGuiWrapper.Update();
		sceneManager.Update(deltaTime_);

		sceneManager.Draw(deltaTime_);

		// 頂点バッファに溜まった頂点データを描画する
		// DxLibは特定条件のもと、まとめて描画する機能がある
		// そのため、ScreenFlip関数で溜まっていたDxLib分の描画が実行され、
		// imGuiの描画が上書きされてしまう場合がある
		// それを無くすために、RenderVertex関数で、溜め分を描画しておく
		// 処理速度的にはちょっと懸念
		RenderVertex();

		imGuiWrapper.Draw();

		ScreenFlip();

	}

}

void Application::Destroy()
{

	InputManager::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();
	SceneManager::GetInstance().Destroy();
	ImGuiWrapper::GetInstance().Destroy();

	Effkseer_End();

	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

	delete instance_;

}

Application::Application()
	:
	isInitFail_(false),
	isReleaseFail_(false),
	preTime_(std::chrono::system_clock::now()),
	deltaTime_(1.0f / 60.0f)
{

	// アプリケーションの初期設定
	SetWindowText("AGS2025winter");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	// Effekseerの初期化
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
	}
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// キー制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// リソース管理初期化
	ResourceManager::CreateInstance();

	// シーン管理初期化
	SceneManager::CreateInstance();

	// デバッグ描画初期化
	ImGuiWrapper::CreateInstance();

}