#include "../Object/Common/ModelMaterial.h"
#include "../Object/Common/Renderer.h"
#include "Area1Collision.h"

Area1Collision::Area1Collision(const VECTOR& pos, const json& data)
	:
	StageBase(pos, data)
{

	// モデルID
	modelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA1_COLLISION_MODEL)]);

	// ステージの衝突判定しているかどうか
	isStageCollision_ = false;

	// 制限エリアとの当たり判定を出す座標
	restrictedAreaPos_ = { 14000.0f,-18000.0f,30000.0f };

	// 制限エリアかの判定
	isRestrictedArea_ = true;

	// 共通部分は基底クラスで初期化
	ActorBase::Init(pos);

	// モデル制御の基本情報を初期化
	CollisionInit(transform_);

}

void Area1Collision::Draw(const float deltaTime)
{

	// 衝突の壁の描画
	CollisionDraw(deltaTime);

	// 制限エリアとの当たり判定を出す座標
	//DrawSphere3D(restrictedAreaPos_, restrictedAreaRadius_, 10, 0xff0000, 0xff0000, false);

}
