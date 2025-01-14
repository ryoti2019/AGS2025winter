#include "../Object/Common/ModelMaterial.h"
#include "../Object/Common/Renderer.h"
#include "Area4Collision.h"

Area4Collision::Area4Collision(const VECTOR& pos, const json& data)
	:
	StageBase(pos, data)
{

	// ���f��ID
	modelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA4_COLLISION_MODEL)]);

	// �X�e�[�W�̏Փ˔��肵�Ă��邩�ǂ���
	isStageCollision_ = false;

	// �����G���A�Ƃ̓����蔻����o�����W
	restrictedAreaPos_ = { -60000.0f,-18000.0f,-68000.0f };

	// �����G���A���̔���
	isRestrictedArea_ = true;

	// ���ʕ����͊��N���X�ŏ�����
	ActorBase::Init(pos);

	// ���f������̊�{����������
	CollisionInit(transform_);

}

void Area4Collision::Draw(const float deltaTime)
{

	// �Փ˂̕ǂ̕`��
	CollisionDraw(deltaTime);

	// �����G���A�Ƃ̓����蔻����o�����W
	DrawSphere3D(restrictedAreaPos_, restrictedAreaRadius_, 10, 0xff0000, 0xff0000, false);

}
