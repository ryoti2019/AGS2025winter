#include "../Manager/SceneManager.h"
#include "Area5Collision.h"

Area5Collision::Area5Collision(const VECTOR& pos, const json& data)
	:
	StageBase(pos, data)
{

	// ���f��ID
	modelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA5_COLLISION_MODEL)]);

	// �X�e�[�W�̏Փ˔��肵�Ă��邩�ǂ���
	isStageCollision_ = false;

	// �����G���A�Ƃ̓����蔻����o�����W
	restrictedAreaPos_ = { -4000.0f,-18000.0f,-116000.0f };

	// �����G���A���̔���
	isRestrictedArea_ = true;

	// ���ʕ����͊��N���X�ŏ�����
	ActorBase::Init(pos);

	// ���f������̊�{����������
	CollisionInit(transform_);

}

void Area5Collision::Draw(const float deltaTime)
{

	// �{�X�o�g���̂ݕ`�悷��
	if (SceneManager::GetInstance().GetSceneID() == SCENE_ID::BOSS_BATTLE)
	{
		// �Փ˂̕ǂ̕`��
		CollisionDraw(deltaTime);
	}

	// �����G���A�Ƃ̓����蔻����o�����W
	//DrawSphere3D(restrictedAreaPos_, restrictedAreaRadius_, 10, 0xff0000, 0xff0000, false);

}
