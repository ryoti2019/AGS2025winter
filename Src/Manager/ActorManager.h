#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include "../Lib/nlohmann/json.hpp"
#include "CollisionManager.h"
#include "../Manager/SceneManager.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/BossBattleScene.h"
#include "../Scene/BossAppearanceScene.h"
#include "../Scene/GameClearScene.h"
#include "../Object/ActorBase.h"
#include "../Common/Vector2F.h"

class ActorBase;

enum class ActorType;

class ActorManager
{

public:

	ActorManager();

	~ActorManager() = default;

	void Init();
	void Update(const float deltaTime);
	void Draw(const float deltaTime);

	// Actorの派生クラスの実体の作成
	template <typename actor>
	void CreateActor(const json& data, const VECTOR& pos);

	// アクティブになったものを格納
	std::shared_ptr<ActorBase> ActiveData(const ActorType type, const VECTOR& pos);

	// 非アクティブになったものを格納
	void DeactiveData(const std::shared_ptr<ActorBase>& actor);

	const std::unordered_map<ActorType, std::vector<std::shared_ptr<ActorBase>>>& GetDeActiveActorData() const{ return deactiveActorData_; };

	const std::unordered_map<ActorType, std::vector<std::shared_ptr<ActorBase>>>& GetActiveActorData() const{ return activeActorData_; };

private:

	// すべてのアクターをまとめたデータ
	std::unordered_map<ActorType, std::vector<std::shared_ptr<ActorBase>>> activeActorData_;

	// すべてのアクターをまとめたデータ
	std::unordered_map<ActorType, std::vector<std::shared_ptr<ActorBase>>> deactiveActorData_;

	// CreateActorでシーンがあるか確認するリスト
	const std::vector<std::shared_ptr<SceneBase>> scenes_ =
	{
		{
			{std::shared_ptr<GameScene>()},
			{std::shared_ptr<BossAppearanceScene>()}
		}
	};

};

template<typename T>
inline void ActorManager::CreateActor(const json& data, const VECTOR& pos)
{

	std::shared_ptr<ActorBase> actor = std::make_shared<T>(pos, data);

	// ポインタを使うときはクラッシュしないようにNULLチェックを行うようにする
	if (!actor) return;

	// deactiveActorData_の中にすでに同じ型が生成されているかチェックする
	auto deactorElem = deactiveActorData_.find(actor->GetActorType());

	// 生成されていない場合は、新しくvector配列の箱を作りその中に要素を入れていく
	if (deactorElem == deactiveActorData_.end())
	{
		int cnt = actor.use_count();
		std::vector<std::shared_ptr<ActorBase>> data;
		data.emplace_back(actor);
		deactiveActorData_.emplace(actor->GetActorType(), data);
	}
	// 生成されている場合はすでにある箱の中に要素を入れていく
	else
	{
		deactorElem->second.emplace_back(actor);
	}

}