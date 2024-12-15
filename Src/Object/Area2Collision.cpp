#include "../Object/Common/ModelMaterial.h"
#include "../Object/Common/Renderer.h"
#include "Area2Collision.h"

Area2Collision::Area2Collision(const VECTOR& pos, const json& data)
	:
	StageBase(pos, data)
{

	// モデルID
	modelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA1_COLLISION_MODEL)]);

	// ステージの衝突判定しているかどうか
	isStageCollision_ = false;

	// 共通部分は基底クラスで初期化
	ActorBase::Init(pos);

	// モデル制御の基本情報を初期化
	CollisionInit(transform_, stageCollisionModelId_, isStageCollision_);

}

void Area2Collision::Draw(const float deltaTime)
{

	if (isStageCollision_ || isDissolve_)
	{

		// ゲートミストの描画
		deltaTime_ += deltaTime;
		if (isDissolve_)
		{
			alphaTime_ += 0.005f;
		}
		modelMaterial_->SetConstBufsPS({ 0.0f,0.0f,0.0f,deltaTime_ }, 2);
		modelMaterial_->SetConstBufsPS({ alphaTime_,0.0f,0.0f,0.0f }, 3);
		modelMaterial_->SetTextureAddress(ModelMaterial::TEXADDRESS::MIRROR);
		renderer_->Draw();
	}

}
