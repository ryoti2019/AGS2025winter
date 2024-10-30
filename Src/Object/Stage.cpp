#include "Stage.h"

Stage::Stage(const VECTOR& pos, const json& data)
	:
	ActorBase(pos,data)
{

	// ƒ‚ƒfƒ‹ID
	modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::STAGE);

	// ‹¤’Ê•”•ª‚ÍŠî’êƒNƒ‰ƒX‚Å‰Šú‰»
	ActorBase::Init(pos);

}
