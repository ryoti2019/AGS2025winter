#include "Area5Collision.h"

Area5Collision::Area5Collision(const VECTOR& pos, const json& data)
	:
	StageBase(pos, data)
{

	// モデルID
	modelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA5_COLLISION_MODEL)]);

	// ステージの衝突判定しているかどうか
	isStageCollision_ = false;

	// 制限エリアとの当たり判定を出す座標
	restrictedAreaPos_ = { -4000.0f,-18000.0f,-116000.0f };

	// 制限エリアかの判定
	isRestrictedArea_ = true;

	// 共通部分は基底クラスで初期化
	ActorBase::Init(pos);

	// モデル制御の基本情報を初期化
	CollisionInit(transform_);

}

void Area5Collision::Draw(const float deltaTime)
{

	// 衝突の壁の描画
	CollisionDraw(deltaTime);

}
