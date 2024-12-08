#include "../Lib/ImGui/imgui.h"
#include "../Common/Quaternion.h"
#include "../Object/Common/Transform.h"
#include "Stage.h"

Stage::Stage(const VECTOR& pos, const json& data)
	:
	ActorBase(pos,data)
{

	// ���f��ID
	modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::STAGE_MODEL);

	// �Փ˔���̃��f��ID
	collisionModelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::STAGE_COLLISION_MODEL);

	// ���f������̊�{���𐶐�
	collision_ = std::make_shared<Transform>();

	collision_->SetModel(collisionModelId_);
	collision_->pos = { 0.0f, -20000.0f, 0.0f };
	collision_->scl = { 10.0f,10.0f,10.0f };
	collision_->quaRot = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(0.0f),Utility::Deg2RadF(0.0f) });
	collision_->quaRotLocal = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(180.0f),Utility::Deg2RadF(0.0f) });
	collision_->Update();

	// ���ʕ����͊��N���X�ŏ�����
	ActorBase::Init(pos);

}

void Stage::Init(const VECTOR& pos)
{

	// ���ʕ����͊��N���X�ŏ�����
	ActorBase::Init(pos);

	// �`��p�̃R���|�[�l���g
	drawComponent_ = std::make_unique<DrawComponent>(std::static_pointer_cast<Stage>(GetThis()));

}