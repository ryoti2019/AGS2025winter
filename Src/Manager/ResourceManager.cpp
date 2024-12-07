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

	// プレイヤーのフックアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Hook.mv1");
	resourcesMap_.emplace(SRC::PLAYER_HOOK, res);

	// プレイヤーの左キックアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/LeftKick.mv1");
	resourcesMap_.emplace(SRC::PLAYER_LEFT_KICK, res);

	// プレイヤーの右キックアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/RightKick.mv1");
	resourcesMap_.emplace(SRC::PLAYER_RIGHT_KICK, res);

	// プレイヤーのアッパーアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Upper.mv1");
	resourcesMap_.emplace(SRC::PLAYER_UPPER, res);

	// プレイヤーの溜めパンチアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/ChargePunch.mv1");
	resourcesMap_.emplace(SRC::PLAYER_CHARGE_PUNCH, res);

	// プレイヤーの必殺技アニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/SpecialAttack.mv1");
	resourcesMap_.emplace(SRC::PLAYER_SPECIAL_PUNCH, res);

	// プレイヤーの必殺技の溜めアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/PowerUp.mv1");
	resourcesMap_.emplace(SRC::PLAYER_POWER_CHARGE, res);

	// プレイヤーの攻撃が頭に当たった時のアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/HitHead.mv1");
	resourcesMap_.emplace(SRC::PLAYER_HIT_HEAD, res);

	// プレイヤーの攻撃が体に当たった時のアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/HitBody.mv1");
	resourcesMap_.emplace(SRC::PLAYER_HIT_BODY, res);

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

	// 敵のパンチアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Punch.mv1");
	resourcesMap_.emplace(SRC::ENEMY_PUNCH, res);

	// 敵のキックアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Kick.mv1");
	resourcesMap_.emplace(SRC::ENEMY_KICK, res);

	// 敵の攻撃が頭に当たった時のアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/HitHead.mv1");
	resourcesMap_.emplace(SRC::ENEMY_HIT_HEAD, res);

	// 敵の攻撃が体に当たった時のアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/HitBody.mv1");
	resourcesMap_.emplace(SRC::ENEMY_HIT_BODY, res);

	// 敵の攻撃が当たった時上方向に飛んでいくアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/HitFly.mv1");
	resourcesMap_.emplace(SRC::ENEMY_HIT_FLY, res);

	// 敵の攻撃が当たった時に怯むアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/FlinchUp.mv1");
	resourcesMap_.emplace(SRC::ENEMY_HIT_FLINCH, res);

	// 敵の攻撃が当たった時にまっすぐ飛んでいくアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/KnockBack.mv1");
	resourcesMap_.emplace(SRC::ENEMY_HIT_KNOCK_BACK, res);

	// 敵の起き上がるアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/KipUp.mv1");
	resourcesMap_.emplace(SRC::ENEMY_KIP_UP, res);

	// 敵のノーマル死亡アニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/Death.mv1");
	resourcesMap_.emplace(SRC::ENEMY_NORMAL_DEATH, res);

#pragma endregion

#pragma region ボスのアニメーション

	// ボスのモデル
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Boss/Boss.mv1");
	resourcesMap_.emplace(SRC::BOSS, res);

	// ボスの待機アニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Boss/Idle.mv1");
	resourcesMap_.emplace(SRC::BOSS_IDLE, res);

	// ボスの走るアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Boss/Run.mv1");
	resourcesMap_.emplace(SRC::BOSS_RUN, res);

	// ボスのパンチアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Boss/Punch.mv1");
	resourcesMap_.emplace(SRC::BOSS_PUNCH, res);

	// ボスのキックアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Boss/Kick.mv1");
	resourcesMap_.emplace(SRC::BOSS_KICK, res);

	// ボスのソニックブームアニメーション
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Boss/SonicBoom.mv1");
	resourcesMap_.emplace(SRC::BOSS_SONIC_BOOM, res);

#pragma endregion

#pragma region ステージ

	// ステージのモデル
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/Building.mv1");
	resourcesMap_.emplace(SRC::STAGE_MODEL, res);

	// ステージの衝突判定モデル
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/Collision.mv1");
	resourcesMap_.emplace(SRC::STAGE_COLLISION_MODEL, res);

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