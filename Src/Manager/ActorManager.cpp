#include "ActorManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "../Utility/Utility.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Stage.h"

ActorManager::ActorManager()
	:
	minEnemy_(nullptr),
	minDistance_(10000.0f)
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

	//// 敵がロックオンしているものがあるか確認する処理
	//for (const auto& data : activeActorData_)
	//{
	//	for (const auto& actor : data.second)
	//	{

	//		// 敵
	//		const auto& enemys = activeActorData_.find(ActorType::ENEMY);

	//		// 中身が入っているか確認
	//		if (enemys == activeActorData_.end())continue;

	//		for (const std::shared_ptr<ActorBase>& enemy : enemys->second)
	//		{
	//			// 敵の中で1つでもtrueだったら処理しない
	//			if (enemy->GetIsLockOn())return;
	//		}
	//	}
	//}

	//// 敵をロックオンするための処理
	//for (const auto& data : activeActorData_)
	//{
	//	for (const auto& actor : data.second)
	//	{

	//		// プレイヤー
	//		const auto& players = activeActorData_.find(ActorType::PLAYER);

	//		// 敵
	//		const auto& enemys = activeActorData_.find(ActorType::ENEMY);

	//		// 中身が入っているか確認
	//		if (players == activeActorData_.end())continue;
	//		if (enemys == activeActorData_.end())continue;

	//		for (const std::shared_ptr<ActorBase>& player : players->second)
	//		{
	//			for (const std::shared_ptr<ActorBase>& enemy : enemys->second)
	//			{

	//				// ポインタが入っているか確認
	//				if (!player)return;
	//				if (!enemy)return;

	//				// プレイヤーから敵に向けてのベクトル
	//				const VECTOR& vec = VSub(enemy->GetPos(), player->GetPos());

	//				// ベクトルの距離
	//				const float distance = Utility::MagnitudeF(vec);

	//				// 一番小さい距離と比べて今比較しているほうが小さかったら今のを一番小さい値に設定する
	//				if (distance > minDistance_)continue;

	//				minDistance_ = distance;
	//				minEnemy_ = enemy;
	//				

	//				// この敵をロックオンする
	//				minEnemy_->SetIsLockOn(true);

	//				// カメラにこの敵を追従させるためTransformを渡す
	//				SceneManager::GetInstance().GetCamera().lock()->SetEnemy(&minEnemy_->GetTransform());

	//				// カメラにロックオン機能をONにする
 //					SceneManager::GetInstance().GetCamera().lock()->SetLockOn(true);

	//				// 決まったら距離を初期化する
	//				minDistance_ = 10000.0f;
	//				return;

	//			}
	//		}
	//	}
	//}

}

void ActorManager::Draw()
{

	// activeActorData_の描画
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