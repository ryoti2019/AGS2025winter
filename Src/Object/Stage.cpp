#include "Stage.h"

Stage::Stage(const VECTOR& pos, const json& data)
	:
	ActorBase(pos,data)
{

	// モデルID
	modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::STAGE);

	// 共通部分は基底クラスで初期化
	ActorBase::Init(pos);

}
