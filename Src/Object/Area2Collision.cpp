#include "../Object/Common/ModelMaterial.h"
#include "../Object/Common/Renderer.h"
#include "Area2Collision.h"

Area2Collision::Area2Collision(const VECTOR& pos, const json& data)
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
	CollisionInit(transform_, stageCollisionModelId_, isStageCollision_);

}

void Area2Collision::Draw(const float deltaTime)
{

	if (isStageCollision_ || isDissolve_)
	{

		// �Q�[�g�~�X�g�̕`��
		deltaTime_ += deltaTime;
		if (isDissolve_)
		{
			alphaTime_ += 0.005f;
		}
		modelMaterial_->SetConstBufsPS({ 0.0f,0.0f,0.0f,deltaTime_ }, 2);
		modelMaterial_->SetConstBufsPS({ alphaTime_,0.0f,0.0f,0.0f }, 3);
		modelMaterial_->SetTextureAddress(ModelMaterial::TEXADDRESS::MIRROR);
		renderer_->Draw();
	}

}
