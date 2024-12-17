#include "StageCollision.h"

StageCollision::StageCollision(const VECTOR& pos, const json& data)
	:
	StageBase(pos, data)
{

	// モデルID
	modelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_COLLISION_MODEL)]);

	// ステージの衝突判定しているかどうか
	isStageCollision_ = true;

	// 制限エリアかの判定
	isRestrictedArea_ = false;

	// 共通部分は基底クラスで初期化
	ActorBase::Init(pos);

}

void StageCollision::Draw(const float deltaTime)
{
}