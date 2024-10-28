#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include "../Lib/nlohmann/json.hpp"
#include "CollisionManager.h"
#include "../Scene/GameScene.h"
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
	void CreateActor(const json& data);

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
	
	// 距離が一番短い敵のポインタ
	std::shared_ptr<ActorBase> minEnemy_;

	// 一番短い距離を保持するポインタ
	float minDistance_;

};

template<typename T>
inline void ActorManager::CreateActor(const json& data)
{

	const VECTOR pos = { 0.0f,0.0f,0.0f };
	std::shared_ptr<ActorBase> actor = std::make_shared<T>(pos, data);

	// ポインタを使うときはクラッシュしないようにNULLチェックを行うようにする
	if (!actor) return;

	//actor->Init(pos);

	auto a = SceneManager::GetInstance().GetNowScene();
	// 基底クラスから使いたい型へキャストする
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(a);

	// NULLチェック
	if (!gameScene) return;

	// コリジョンマネージャーを取得
	std::shared_ptr<CollisionManager> collisionManager = gameScene->GetCollisionManager();

	// 衝突判定の管理クラスに登録
	collisionManager->Register(actor);

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