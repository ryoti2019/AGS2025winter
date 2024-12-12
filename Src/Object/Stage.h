#pragma once
#include "ActorBase.h"

class Stage : public ActorBase
{

public:

	// 衝突判定データ
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

	// ステージの当たり判定を設定
	void SetIsArea1Collision(const bool isCollision)
	{
		isArea1Collision_ = isCollision;
		collisionModels_.at(1) = { area1CollisionModelId_ ,isArea1Collision_ };
	}
	void SetIsArea2Collision(const bool isCollision)
	{
		isArea2Collision_ = isCollision;
		collisionModels_.at(2) = { area2CollisionModelId_ ,isArea2Collision_ };
	}
	void SetIsArea3Collision(const bool isCollision) 
	{
		isArea3Collision_ = isCollision;
		collisionModels_.at(3) = { area3CollisionModelId_ ,isArea3Collision_ };

	}
	void SetIsArea4Collision(const bool isCollision) 
	{
		isArea4Collision_ = isCollision;
		collisionModels_.at(4) = { area4CollisionModelId_ ,isArea4Collision_ };

	}
	void SetIsArea5Collision(const bool isCollision)
	{
		isArea5Collision_ = isCollision;
		collisionModels_.at(5) = { area5CollisionModelId_ ,isArea5Collision_ };

	}

private:

	// ステージのコリジョンモデル
	std::unique_ptr<Transform> stageCollision_;
	std::unique_ptr<Transform> area1Collision_;
	std::unique_ptr<Transform> area2Collision_;
	std::unique_ptr<Transform> area3Collision_;
	std::unique_ptr<Transform> area4Collision_;
	std::unique_ptr<Transform> area5Collision_;

	// ステージモデルID
	int stageCollisionModelId_;
	int area1CollisionModelId_;
	int area2CollisionModelId_;
	int area3CollisionModelId_;
	int area4CollisionModelId_;
	int area5CollisionModelId_;

	// ステージの衝突判定しているかどうか
	bool isStageCollision_;
	bool isArea1Collision_;
	bool isArea2Collision_;
	bool isArea3Collision_;
	bool isArea4Collision_;
	bool isArea5Collision_;

	// 衝突モデルリスト
	std::vector<CollisionModel> collisionModels_;

	// モデル制御の基本情報を初期化
	void CollisionInit(std::unique_ptr<Transform>& transform, const int modelId, const bool isCollision);

};

