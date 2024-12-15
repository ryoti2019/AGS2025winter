#include "Stage.h"

Stage::Stage(const VECTOR& pos, const json& data)
	:
	StageBase(pos, data)
{

	// ���f��ID
	modelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_MODEL)]);

	// ���ʕ����͊��N���X�ŏ�����
	ActorBase::Init(pos);

}
