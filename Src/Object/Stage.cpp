#include "../Lib/ImGui/imgui.h"
#include "../Common/Quaternion.h"
#include "../Object/Common/Transform.h"
#include "Stage.h"

Stage::Stage(const VECTOR& pos, const json& data)
	:
	ActorBase(pos,data)
{

	// ���f��ID
	modelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_MODEL)]);

	// ���f������̊�{���𐶐�
	stageCollision_ = std::make_unique<Transform>();
	area1Collision_ = std::make_unique<Transform>();
	area2Collision_ = std::make_unique<Transform>();
	area3Collision_ = std::make_unique<Transform>();
	area4Collision_ = std::make_unique<Transform>();
	area5Collision_ = std::make_unique<Transform>();

	// �Փ˔���̃��f��ID
	stageCollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_COLLISION_MODEL)]);
	area1CollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA1_COLLISION_MODEL)]);
	area2CollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA2_COLLISION_MODEL)]);
	area3CollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA3_COLLISION_MODEL)]);
	area4CollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA4_COLLISION_MODEL)]);
	area5CollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA5_COLLISION_MODEL)]);

	// �X�e�[�W�̏Փ˔��肵�Ă��邩�ǂ���
	isStageCollision_ = true;
	isArea1Collision_ = false;
	isArea2Collision_ = false;
	isArea3Collision_ = false;
	isArea4Collision_ = false;
	isArea5Collision_ = false;

	// ���f������̊�{����������
	CollisionInit(stageCollision_,stageCollisionModelId_, isStageCollision_);
	CollisionInit(area1Collision_, area1CollisionModelId_, isArea1Collision_);
	CollisionInit(area2Collision_, area2CollisionModelId_, isArea2Collision_);
	CollisionInit(area3Collision_, area3CollisionModelId_, isArea3Collision_);
	CollisionInit(area4Collision_, area4CollisionModelId_, isArea4Collision_);
	CollisionInit(area5Collision_, area5CollisionModelId_, isArea5Collision_);

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

void Stage::CollisionInit(std::unique_ptr<Transform>& transform, const int modelId, const bool isCollision)
{

	// ������
	transform->SetModel(modelId);
	transform->pos = { 0.0f, -20000.0f, 0.0f };
	transform->scl = { 10.0f,10.0f,10.0f };
	transform->quaRot = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(0.0f),Utility::Deg2RadF(0.0f) });
	transform->quaRotLocal = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(180.0f),Utility::Deg2RadF(0.0f) });
	transform->Update();

	// �Փ˔���f�[�^
	CollisionModel data;

	data.isActive = isCollision;
	data.modelId = transform->modelId;

	// �Փ˃��f�����X�g�ɒǉ�
	collisionModels_.emplace_back(data);

}