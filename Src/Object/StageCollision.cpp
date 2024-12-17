#include "StageCollision.h"

StageCollision::StageCollision(const VECTOR& pos, const json& data)
	:
	StageBase(pos, data)
{

	// ���f��ID
	modelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_COLLISION_MODEL)]);

	// �X�e�[�W�̏Փ˔��肵�Ă��邩�ǂ���
	isStageCollision_ = true;

	// �����G���A���̔���
	isRestrictedArea_ = false;

	// ���ʕ����͊��N���X�ŏ�����
	ActorBase::Init(pos);

}

void StageCollision::Draw(const float deltaTime)
{
}