#include "Stage.h"

Stage::Stage(const VECTOR& pos, const json& data)
	:
	ActorBase(pos,data)
{

	// ���f��ID
	modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::STAGE);

	// ���ʕ����͊��N���X�ŏ�����
	ActorBase::Init(pos);

}
