#include <fstream>
#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

	// 外部ファイルの読み込み
	std::ifstream ifs;

	ifs.open(Application::PATH_JSON + "Resource.json");

	if (!ifs)
	{
		// 外部ファイルの読み込み失敗
		return;
	}

	// ファイルストリームからjsonオブジェクトに変換
	nlohmann::json objectJson = nlohmann::json::parse(ifs);

	const nlohmann::json_abi_v3_11_3::json& objectData = objectJson["ResourceData"];

	// スコープが切れる際に 自動的にファイルクローズして貰えますが、
	// お行儀良く、明示的にファイルストリームを閉じる
	ifs.close();

	// 画像のロード
	ImgLoad(objectData);

	// モデルとアニメーションのロード
	ModelAndAnimationLoad(objectData);

	// エフェクトのロード
	EffectLoad(objectData);

	// BGMとSEのロード
	BGMAndSELoad(objectData);

}

void ResourceManager::ImgLoad(const nlohmann::json_abi_v3_11_3::json& objectData)
{

	// プレイヤーの画像データ
	const auto& playerImageData = objectData[0]["ImageData"]["Player"];

	// ステージの画像データの初期化
	InitResource(Application::PATH_IMAGE, playerImageData);

	// ステージの画像データ
	const auto& stageImageData = objectData[0]["ImageData"]["Stage"];

	// ステージの画像データの初期化
	InitResource(Application::PATH_IMAGE, stageImageData);

	// タイトルシーンの画像データ
	const auto& titleSceneImageData = objectData[0]["ImageData"]["TitleScene"];

	// タイトルシーンの画像データの初期化
	InitResource(Application::PATH_IMAGE, titleSceneImageData);

	// ゲームシーンの画像データ
	const auto& gameSceneImageData = objectData[0]["ImageData"]["GameScene"];

	// ゲームシーンの画像データの初期化
	InitResource(Application::PATH_IMAGE, gameSceneImageData);

}

void ResourceManager::ModelAndAnimationLoad(const nlohmann::json_abi_v3_11_3::json& objectData)
{

	// プレイヤーのモデルとアニメーションデータ
	const auto& playerModelData = objectData[0]["ModelData"]["Player"];

	// プレイヤーのモデルとアニメーションの初期化
	InitResource(Application::PATH_MODEL, playerModelData);

	// 敵のモデルとアニメーションデータ
	const auto& enemyModelData = objectData[0]["ModelData"]["Enemy"];

	// 敵のモデルとアニメーションの初期化
	InitResource(Application::PATH_MODEL, enemyModelData);

	// ボスのモデルとアニメーションデータ
	const auto& bossModelData = objectData[0]["ModelData"]["Boss"];

	// ボスのモデルとアニメーションの初期化
	InitResource(Application::PATH_MODEL, bossModelData);

	// ステージのモデルとアニメーションデータ
	const auto& stageModelData = objectData[0]["ModelData"]["Stage"];

	// ステージのモデルとアニメーションの初期化
	InitResource(Application::PATH_MODEL, stageModelData);

}

void ResourceManager::EffectLoad(const nlohmann::json_abi_v3_11_3::json& objectData)
{

	// ボスのエフェクトデータ
	const auto& bossEffectData = objectData[0]["EffectData"]["Boss"];

	// ボスのエフェクトデータの初期化
	InitResource(Application::PATH_EFFECT, bossEffectData);

}

void ResourceManager::BGMAndSELoad(const nlohmann::json_abi_v3_11_3::json& objectData)
{

	// タイトルシーンの画像データ
	const auto& playerSoundData = objectData[0]["SoundData"]["Player"];

	// タイトルシーンの画像データの初期化
	InitResource(Application::PATH_SOUND, playerSoundData);

	// タイトルシーンの画像データ
	const auto& titleSceneSoundData = objectData[0]["SoundData"]["TitleScene"];

	// タイトルシーンの画像データの初期化
	InitResource(Application::PATH_SOUND, titleSceneSoundData);

	// タイトルシーンの画像データ
	const auto& gameSceneSoundData = objectData[0]["SoundData"]["GameScene"];

	// タイトルシーンの画像データの初期化
	InitResource(Application::PATH_SOUND, gameSceneSoundData);

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
		delete p.second;
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

Resource ResourceManager::Load(std::string src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(std::string src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->handleId_);
	res->duplicateModelIds_.push_back(duId);

	return duId;
}

void ResourceManager::InitResource(const std::string& pathName, const nlohmann::json_abi_v3_11_3::json jsonData)
{

	Resource res;

	for (const auto& data : jsonData)
	{

		// リソースのタイプ
		Resource::TYPE type = data["TYPE"];

		// パスの名前
		std::string path = data["PATH"];

		// 格納する番号
		std::string src = data["SRC"];

		// 格納する
		res = Resource(type, pathName + path);
		resourcesMap_.emplace(src, res);

	}

}

ResourceManager::ResourceManager(void)
{
}

Resource* ResourceManager::_Load(std::string src)
{
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return nullptr;
	}

	rPair->second.Load();

	// 念のためコピーコンストラクタ
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}