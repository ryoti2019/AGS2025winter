#pragma once
#include "ActorBase.h"

class Stage : public ActorBase
{

public:

	struct CollisionModel
	{

		// モデルID
		int modelId;

		// 衝突判定するかのフラグ
		bool isActive;

	};

	Stage(const VECTOR& pos, const json& data);

	~Stage() = default;

	void Init(const VECTOR& pos)override;

	// 衝突モデルリストを取得
	const std::vector<CollisionModel>& GetCollisionModels()const { return collisionModels_; }

private:

	// ステージのコリジョンモデル
	std::shared_ptr<Transform> collision_;

	// エリア1のコリジョンモデル
	std::shared_ptr<Transform> area1Collision_;

	// 衝突モデルリスト
	std::vector<CollisionModel> collisionModels_;

};

