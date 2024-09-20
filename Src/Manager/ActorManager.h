#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
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
	void Draw();

	// Actorの派生クラスの実体の作成
	template <typename actor>
	void CreateActor();

	// アクティブになったものを格納
	std::shared_ptr<ActorBase> ActiveData(const ActorType type, const VECTOR& pos);

	// 非アクティブになったものを格納
	void DeactiveData(const std::shared_ptr<ActorBase>& actor);

	const auto& const GetActiveActorData() { return activeActorData_; };

private:

	// すべてのアクターをまとめたデータ
	std::unordered_map<ActorType, std::vector<std::shared_ptr<ActorBase>>> activeActorData_;

	// すべてのアクターをまとめたデータ
	std::unordered_map<ActorType, std::vector<std::shared_ptr<ActorBase>>> deactiveActorData_;

};

template<typename T>
inline void ActorManager::CreateActor()
{

	const VECTOR pos = { 0.0f,0.0f,0.0f };
	std::shared_ptr<ActorBase> actor = std::make_shared<T>(pos);

	// ポインタを使うときはクラッシュしないようにNULLチェックを行うようにする
	if (!actor) return;

	//actor->Init(pos);

	// deactiveActorData_の中にすでに同じ型が生成されているかチェックする
	auto deactorElem = deactiveActorData_.find(actor->GetActorType());

	// 生成されていない場合は、新しくvector配列の箱を作りその中に要素を入れていく
	if (deactorElem == deactiveActorData_.end())
	{
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