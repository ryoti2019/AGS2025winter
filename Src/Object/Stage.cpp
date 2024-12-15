#include "Stage.h"

Stage::Stage(const VECTOR& pos, const json& data)
	:
	StageBase(pos, data)
{

	// ƒ‚ƒfƒ‹ID
	modelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_MODEL)]);

	// ‹¤’Ê•”•ª‚ÍŠî’êƒNƒ‰ƒX‚Å‰Šú‰»
	ActorBase::Init(pos);

}
