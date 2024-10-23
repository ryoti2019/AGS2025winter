#include <fstream>
#include "../Lib/nlohmann/json.hpp"
#include "../Application.h"
#include "ActorManager.h"
#include "ResourceManager.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"

ActorManager::ActorManager()
{

	// 外部ファイルの読み込み
	std::ifstream ifs;

	ifs.open(Application::PATH_JSON + "ObjectData.json");

	if (!ifs)
	{
		// 外部ファイルの読み込み失敗
		return;
	}

	// ファイルストリームからjsonオブジェクトに変換
	nlohmann::json objectJson = nlohmann::json::parse(ifs);

	const auto& objectData = objectJson["ObjectData"];

	// スコープが切れる際に 自動的にファイルクローズして貰えますが、
	// お行儀良く、明示的にファイルストリームを閉じる
	ifs.close();

	// プレイヤー
	const auto& playerData = objectData[0]["PlayerData"];

	// プレイヤーを生成
	CreateActor<Player>(playerData);
	ActiveData(ActorType::PLAYER, { 0.0f,0.0f,0.0f });

	// 敵を生成
	for (int i = 0; i < 50; i++)
	{
		float x = std::rand() % 10000;
		float z = std::rand() % 10000;
		CreateActor<Enemy>(playerData);
		ActiveData(ActorType::ENEMY, { -5000.0f + x,0.0f,-5000.0f + z });
	}

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

	for (auto& data : activeActorData_)
	{
		for (const std::shared_ptr<ActorBase>& actor : data.second)
		{
			actor->Update(deltaTime);
		}
	}

}

void ActorManager::Draw()
{

	for (auto& data : activeActorData_)
	{
		for (const std::shared_ptr<ActorBase>& actor : data.second)
		{
			actor->Draw();
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

	ActorType type = actor->GetActorType();

	// activeActorData_の中にすでに同じ型が生成されているかチェックする
	auto actorElem = activeActorData_.find(type);

	// 生成されていない場合は、NULLを返す
	if (actorElem == activeActorData_.end()) return;

	// activeActorData_の先頭部分を削除してdeactiveActorData_に格納する

	// activeActorData_に格納
	deactiveActorData_[type].emplace_back(actor);

}