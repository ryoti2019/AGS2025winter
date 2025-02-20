#include "ActorManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "../Utility/Utility.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Stage.h"

ActorManager::ActorManager()
{
}

void ActorManager::Init()
{
}

void ActorManager::Update(const float deltaTime)
{

	// activeActorData_に非アクティブ状態のものがあれば
	// deactiveActorData_に格納する
	int num = 0;
	for (auto& data : activeActorData_)
	{
		num = 0;
		for (const std::shared_ptr<ActorBase>& actor : data.second)
		{
			if (actor && !actor->GetIsActive())
			{

				// 非アクティブになったものを格納
				DeactiveData(actor);

				// 非アクティブになったものを削除
				data.second.erase(data.second.begin() += num);

			}
			num++;
		}
	}

	// activeActorData_の更新
	for (const auto& data : activeActorData_)
	{
		for (const std::shared_ptr<ActorBase>& actor : data.second)
		{
			actor->Update(deltaTime);
		}
	}

}

void ActorManager::Draw(const float deltaTime)
{

	// activeActorData_の描画
	for (auto& data : activeActorData_)
	{
		for (const std::shared_ptr<ActorBase>& actor : data.second)
		{
			actor->Draw(deltaTime);
		}
	}

}

std::shared_ptr<ActorBase> ActorManager::ActiveData(const ActorType type, const VECTOR& pos)
{

	// 中身が0の場合は、NULLを返す
	if (deactiveActorData_[type].size() == 0)return nullptr;

	// deactiveActorData_の中にすでに同じ型が生成されているかチェックする
	auto deactorElem = deactiveActorData_.find(type);

	// 生成されていない場合は、NULLを返す
	if (deactorElem == deactiveActorData_.end()) return nullptr;

	// deactiveActorData_の先頭部分を削除してactiveActorData_に格納する

	// deactiveActorData_の先頭部分
	std::shared_ptr<ActorBase> active = deactiveActorData_[type].front();

	auto base = SceneManager::GetInstance().GetNowScene();

	// 衝突判定の管理クラス
	std::shared_ptr<CollisionManager> collisionManager;

	switch (SceneManager::GetInstance().GetSceneID())
	{
	case SCENE_ID::GAME:
		collisionManager = base->GetCollisionManager();
		// 衝突判定の管理クラスに登録
		collisionManager->Register(active);
		break;
	case SCENE_ID::BOSS_APPEARANCE:
		collisionManager = base->GetCollisionManager();
		// 衝突判定の管理クラスに登録
		collisionManager->Register(active);
		break;
	case SCENE_ID::BOSS_BATTLE:
		collisionManager = base->GetCollisionManager();
		// 衝突判定の管理クラスに登録
		collisionManager->Register(active);
		break;
	}
	
	// アクティブ状態にする
	active->Init(pos);
	active->SetIsActive(true);
	// activeActorData_に格納
	auto actorElem = activeActorData_.find(type);

	if (actorElem == activeActorData_.end())
	{
		std::vector<std::shared_ptr<ActorBase>> data;
		data.emplace_back(active);
		activeActorData_.emplace(type, data);
	}
	// 生成されている場合はすでにある箱の中に要素を入れていく
	else
	{
		actorElem->second.emplace_back(active);
	}

	// deactiveActorData_の先頭部分を削除
	deactiveActorData_[type].erase(deactiveActorData_[type].begin());

	// アクティブ状態になったものを返す
	return active;

}

void ActorManager::DeactiveData(const std::shared_ptr<ActorBase>& actor)
{

	ActorType type = actor->GetActorType();

	// activeActorData_の中にすでに同じ型が生成されているかチェックする
	auto actorElem = activeActorData_.find(type);

	// 生成されていない場合は、NULLを返す
	if (actorElem == activeActorData_.end()) return;

	// activeActorData_の先頭部分を削除してdeactiveActorData_に格納する

	// activeActorData_に格納
	deactiveActorData_[type].emplace_back(actor);

}