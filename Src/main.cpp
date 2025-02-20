#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <DxLib.h>
#include "Application.h"

#include <istream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Object/TitlePlayer.h"
#include "Manager/SceneManager.h"

// WinMain関数
//---------------------------------
int WINAPI WinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{


	//----------------------------------------------------------
	// 
	//// シーン管理初期化
	SceneManager::CreateInstance();

	// 外部ファイルの読み込み
	std::ifstream ifs;

	ifs.open(Application::PATH_JSON + "ObjectData.json");

	if (!ifs)
	{
		// 外部ファイルの読み込み失敗
		return 0;
	}

	// ファイルストリームからjsonオブジェクトに変換
	nlohmann::json objectJson = nlohmann::json::parse(ifs);

	const auto& objectData = objectJson["ObjectData"];

	// スコープが切れる際に 自動的にファイルクローズして貰えますが、
	// お行儀良く、明示的にファイルストリームを閉じる
	ifs.close();

	const auto& playerData = objectData[0]["PlayerData"];

	VECTOR p;
	auto a = std::make_shared<TitlePlayer>(p, playerData);
	a->Init(p);

	//----------------------------------------------------------
	
	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// インスタンスの生成
	Application::CreateInstance();

	// インスタンスの取得
	Application& instance = Application::GetInstance();

	if (instance.IsInitFail())
	{
		// 初期化失敗
		return -1;
	}

	// 実行
	instance.Run();

	// 解放
	instance.Destroy();

	return 0;

}