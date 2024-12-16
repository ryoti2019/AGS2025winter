#include "../Object/Common/ModelMaterial.h"
#include "../Object/Common/Renderer.h"
#include "Area1Collision.h"

Area1Collision::Area1Collision(const VECTOR& pos, const json& data)
	:
	StageBase(pos, data)
{

	// ���f��ID
	modelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA1_COLLISION_MODEL)]);

	// �X�e�[�W�̏Փ˔��肵�Ă��邩�ǂ���
	isStageCollision_ = false;

	// ���ʕ����͊��N���X�ŏ�����
	ActorBase::Init(pos);

	// ���f������̊�{����������
	CollisionInit(transform_);

}

void Area1Collision::Draw(const float deltaTime)
{

	// �Փ˂̕ǂ̕`��
	CollisionDraw(deltaTime);

}
