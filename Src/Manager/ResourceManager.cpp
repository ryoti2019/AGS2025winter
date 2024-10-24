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

	Resource res;

#pragma region プレイヤーのアニメーション

	// プレイヤーのモデル
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Player.mv1");
	resourcesMap_.emplace(SRC::PLAYER, res);

	// プレイヤーの待機アニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Idle.mv1");
	resourcesMap_.emplace(SRC::PLAYER_IDLE, res);

	// プレイヤーの走るアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Run.mv1");
	resourcesMap_.emplace(SRC::PLAYER_RUN, res);

	// プレイヤーのジャブアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Jab.mv1");
	resourcesMap_.emplace(SRC::PLAYER_JAB, res);

	// プレイヤーのストレートアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Straight.mv1");
	resourcesMap_.emplace(SRC::PLAYER_STRAIGHT, res);

	// プレイヤーのキックアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Kick.mv1");
	resourcesMap_.emplace(SRC::PLAYER_KICK, res);

	// プレイヤーのアッパーアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Upper.mv1");
	resourcesMap_.emplace(SRC::PLAYER_UPPER, res);

#pragma endregion

#pragma region 敵のアニメーション

	// 敵のモデル
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Enemy.mv1");
	resourcesMap_.emplace(SRC::ENEMY , res);

	// 敵の待機アニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Idle.mv1");
	resourcesMap_.emplace(SRC::ENEMY_IDLE, res);

	// 敵の走るアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Run.mv1");
	resourcesMap_.emplace(SRC::ENEMY_RUN, res);

#pragma endregion

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

Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
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

ResourceManager::ResourceManager(void)
{
}

Resource* ResourceManager::_Load(SRC src)
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