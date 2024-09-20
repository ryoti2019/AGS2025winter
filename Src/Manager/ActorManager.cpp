#include "ActorManager.h"

ActorManager::ActorManager()
{

}

void ActorManager::Init()
{
}

void ActorManager::Update(const float deltaTime)
{
}

void ActorManager::Draw()
{
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

	// deactiveActorData_の先頭部分を削除
	deactiveActorData_[type].erase(deactiveActorData_[type].begin());

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

	// アクティブ状態になったものを返す
	return active;

}

void ActorManager::DeactiveData(const std::shared_ptr<ActorBase>& actor)
{
}
