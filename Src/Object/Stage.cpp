#include "../Lib/ImGui/imgui.h"
#include "../Common/Quaternion.h"
#include "../Object/Common/Transform.h"
#include "Stage.h"

Stage::Stage(const VECTOR& pos, const json& data)
	:
	ActorBase(pos,data)
{

	// モデルID
	modelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_MODEL)]);

	// モデル制御の基本情報を生成
	stageCollision_ = std::make_unique<Transform>();
	area1Collision_ = std::make_unique<Transform>();
	area2Collision_ = std::make_unique<Transform>();
	area3Collision_ = std::make_unique<Transform>();
	area4Collision_ = std::make_unique<Transform>();
	area5Collision_ = std::make_unique<Transform>();

	// 衝突判定のモデルID
	stageCollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_COLLISION_MODEL)]);
	area1CollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA1_COLLISION_MODEL)]);
	area2CollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA2_COLLISION_MODEL)]);
	area3CollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA3_COLLISION_MODEL)]);
	area4CollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA4_COLLISION_MODEL)]);
	area5CollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA5_COLLISION_MODEL)]);

	// ステージの衝突判定しているかどうか
	isStageCollision_ = true;
	isArea1Collision_ = false;
	isArea2Collision_ = false;
	isArea3Collision_ = false;
	isArea4Collision_ = false;
	isArea5Collision_ = false;

	// モデル制御の基本情報を初期化
	CollisionInit(stageCollision_,stageCollisionModelId_, isStageCollision_);
	CollisionInit(area1Collision_, area1CollisionModelId_, isArea1Collision_);
	CollisionInit(area2Collision_, area2CollisionModelId_, isArea2Collision_);
	CollisionInit(area3Collision_, area3CollisionModelId_, isArea3Collision_);
	CollisionInit(area4Collision_, area4CollisionModelId_, isArea4Collision_);
	CollisionInit(area5Collision_, area5CollisionModelId_, isArea5Collision_);

	// 共通部分は基底クラスで初期化
	ActorBase::Init(pos);

}

void Stage::Init(const VECTOR& pos)
{

	// 共通部分は基底クラスで初期化
	ActorBase::Init(pos);

	// 描画用のコンポーネント
	drawComponent_ = std::make_unique<DrawComponent>(std::static_pointer_cast<Stage>(GetThis()));

}

void Stage::CollisionInit(std::unique_ptr<Transform>& transform, const int modelId, const bool isCollision)
{

	// 初期化
	transform->SetModel(modelId);
	transform->pos = { 0.0f, -20000.0f, 0.0f };
	transform->scl = { 10.0f,10.0f,10.0f };
	transform->quaRot = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(0.0f),Utility::Deg2RadF(0.0f) });
	transform->quaRotLocal = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(180.0f),Utility::Deg2RadF(0.0f) });
	transform->Update();

	// 衝突判定データ
	CollisionModel data;

	data.isActive = isCollision;
	data.modelId = transform->modelId;

	// 衝突モデルリストに追加
	collisionModels_.emplace_back(data);

}