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

void Stage::Init(const VECTOR& pos)
{

	// ���ʕ����͊��N���X�ŏ�����
	ActorBase::Init(pos);

	// �`��p�̃R���|�[�l���g
	drawComponent_ = std::make_shared<DrawComponent>(std::dynamic_pointer_cast<Stage>(GetThis()));

}
