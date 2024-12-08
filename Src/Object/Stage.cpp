#include "../Lib/ImGui/imgui.h"
#include "../Common/Quaternion.h"
#include "../Object/Common/Transform.h"
#include "Stage.h"

Stage::Stage(const VECTOR& pos, const json& data)
	:
	ActorBase(pos,data)
{

	// モデルID
	modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::STAGE_MODEL);

	// 衝突判定のモデルID
	collisionModelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::STAGE_COLLISION_MODEL);

	// モデル制御の基本情報を生成
	collision_ = std::make_shared<Transform>();

	collision_->SetModel(collisionModelId_);
	collision_->pos = { 0.0f, -20000.0f, 0.0f };
	collision_->scl = { 10.0f,10.0f,10.0f };
	collision_->quaRot = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(0.0f),Utility::Deg2RadF(0.0f) });
	collision_->quaRotLocal = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(180.0f),Utility::Deg2RadF(0.0f) });
	collision_->Update();

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