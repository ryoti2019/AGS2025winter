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
	SoundLoad(objectData);

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

	// ボスの登場シーンの画像データ
	const auto& bossAppearanceSceneImageData = objectData[0]["ImageData"]["BossAppearanceScene"];

	// ボスの登場シーンの画像データの初期化
	InitResource(Application::PATH_IMAGE, bossAppearanceSceneImageData);

	// ゲームクリアシーンの画像データ
	const auto& gameClearSceneImageData = objectData[0]["ImageData"]["GameClearScene"];

	// ゲームクリアシーンの画像データの初期化
	InitResource(Application::PATH_IMAGE, gameClearSceneImageData);

	// ゲームオーバーシーンの画像データ
	const auto& gameOverSceneImageData = objectData[0]["ImageData"]["GameOverScene"];

	// ゲームオーバーシーンの画像データの初期化
	InitResource(Application::PATH_IMAGE, gameOverSceneImageData);

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

	// プレイヤーのエフェクトデータ
	const auto& playerEffectData = objectData[0]["EffectData"]["Player"];

	// プレイヤーのエフェクトデータの初期化
	InitResource(Application::PATH_EFFECT, playerEffectData);

	// 敵のエフェクトデータ
	const auto& enemyEffectData = objectData[0]["EffectData"]["Enemy"];

	// 敵のエフェクトデータの初期化
	InitResource(Application::PATH_EFFECT, enemyEffectData);

	// ボスのエフェクトデータ
	const auto& bossEffectData = objectData[0]["EffectData"]["Boss"];

	// ボスのエフェクトデータの初期化
	InitResource(Application::PATH_EFFECT, bossEffectData);

}

void ResourceManager::SoundLoad(const nlohmann::json_abi_v3_11_3::json& objectData)
{

	// プレイヤーのサウンドデータ
	const auto& playerSoundData = objectData[0]["SoundData"]["Player"];

	// プレイヤーのサウンドデータの初期化
	InitResource(Application::PATH_SOUND, playerSoundData);

	// タイトルシーンのサウンドデータ
	const auto& titleSceneSoundData = objectData[0]["SoundData"]["TitleScene"];

	// タイトルシーンのサウンドデータの初期化
	InitResource(Application::PATH_SOUND, titleSceneSoundData);

	// ゲームシーンのサウンドデータ
	const auto& gameSceneSoundData = objectData[0]["SoundData"]["GameScene"];

	// ゲームシーンのサウンドデータの初期化
	InitResource(Application::PATH_SOUND, gameSceneSoundData);

	// ボスの登場シーンのサウンドデータ
	const auto& bossAppearanceSceneSoundData = objectData[0]["SoundData"]["BossAppearanceScene"];

	// ボスの登場シーンのサウンドデータの初期化
	InitResource(Application::PATH_SOUND, bossAppearanceSceneSoundData);

	// ボスバトルシーンのサウンドデータ
	const auto& bossBattleSceneSoundData = objectData[0]["SoundData"]["BossBattleScene"];

	// ボスバトルシーンのサウンドデータの初期化
	InitResource(Application::PATH_SOUND, bossBattleSceneSoundData);

	// ゲームクリアシーンのサウンドデータ
	const auto& gameClearSceneSoundData = objectData[0]["SoundData"]["GameClearScene"];

	// ゲームクリアシーンのサウンドデータの初期化
	InitResource(Application::PATH_SOUND, gameClearSceneSoundData);

	// ゲームオーバーシーンのサウンドデータ
	const auto& gameOverSceneSoundData = objectData[0]["SoundData"]["GameOverScene"];

	// ゲームオーバーシーンのサウンドデータの初期化
	InitResource(Application::PATH_SOUND, gameOverSceneSoundData);

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		if (p.second->resType_ != Resource::TYPE::ANIM_MODEL)
		{
			p.second->Release();
			delete p.second;
		}
	}

	// DxLibの解放処理不具合対策のため、
	// アニメーションモデルのメモリ解放を最後に行う
	for (auto& p : loadedMap_)
	{
		if (p.second->resType_ == Resource::TYPE::ANIM_MODEL)
		{
			p.second->Release();
			delete p.second;
		}
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
	//Resource ret = *res;
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

	if (src == "MODEL_ENEMY" || src == "ANIM_ENEMY_IDLE")
	{
		int a = 1;
		a = 2;
	}

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