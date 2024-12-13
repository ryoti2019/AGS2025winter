#pragma once
#include "ActorBase.h"

class ModelMaterial;
class Renderer;

class Stage : public ActorBase
{

public:

	// �Փ˔���f�[�^
	struct CollisionModel
	{

		// ���f��ID
		int modelId;

		// �Փ˔��肷�邩�̃t���O
		bool isActive;

	};

	Stage(const VECTOR& pos, const json& data);

	~Stage() = default;

	void Init(const VECTOR& pos)override;

	void Update(const float deltaTime)override;

	void Draw(const float deltaTime)override;

	// �Փ˃��f�����X�g���擾
	const std::vector<CollisionModel>& GetCollisionModels()const { return collisionModels_; }

	// �G���A1�̓����蔻���ݒ�
	void SetIsArea1Collision(const bool isCollision)
	{
		isArea1Collision_ = isCollision;
		collisionModels_.at(1) = { area1CollisionModelId_ ,isArea1Collision_ };
	}

	// �G���A2�̓����蔻���ݒ�
	void SetIsArea2Collision(const bool isCollision)
	{
		isArea2Collision_ = isCollision;
		collisionModels_.at(2) = { area2CollisionModelId_ ,isArea2Collision_ };
	}

	// �G���A3�̓����蔻���ݒ�
	void SetIsArea3Collision(const bool isCollision) 
	{
		isArea3Collision_ = isCollision;
		collisionModels_.at(3) = { area3CollisionModelId_ ,isArea3Collision_ };

	}

	// �G���A4�̓����蔻���ݒ�
	void SetIsArea4Collision(const bool isCollision) 
	{
		isArea4Collision_ = isCollision;
		collisionModels_.at(4) = { area4CollisionModelId_ ,isArea4Collision_ };

	}

	// �G���A5�̓����蔻���ݒ�
	void SetIsArea5Collision(const bool isCollision)
	{
		isArea5Collision_ = isCollision;
		collisionModels_.at(5) = { area5CollisionModelId_ ,isArea5Collision_ };

	}

	// �G���A1�̓����蔻����擾
	const bool GetIsArea1Collision() { return isArea1Collision_; }

	// �G���A2�̓����蔻����擾
	const bool GetIsArea2Collision() { return isArea2Collision_; }

	// �G���A3�̓����蔻����擾
	const bool GetIsArea3Collision() { return isArea3Collision_; }

	// �G���A4�̓����蔻����擾
	const bool GetIsArea4Collision() { return isArea4Collision_; }

	// �G���A5�̓����蔻����擾
	const bool GetIsArea5Collision() { return isArea5Collision_; }

private:

	// �X�e�[�W�̃R���W�������f��
	std::unique_ptr<Transform> stageCollision_;
	std::unique_ptr<Transform> area1Collision_;
	std::unique_ptr<Transform> area2Collision_;
	std::unique_ptr<Transform> area3Collision_;
	std::unique_ptr<Transform> area4Collision_;
	std::unique_ptr<Transform> area5Collision_;

	// ���f���}�e���A��
	std::shared_ptr<ModelMaterial> modelMaterial_;

	// �����_���[
	std::shared_ptr<Renderer> renderer_;

	// �X�e�[�W���f��ID
	int stageCollisionModelId_;
	int area1CollisionModelId_;
	int area2CollisionModelId_;
	int area3CollisionModelId_;
	int area4CollisionModelId_;
	int area5CollisionModelId_;

	// �X�e�[�W�̏Փ˔��肵�Ă��邩�ǂ���
	bool isStageCollision_;
	bool isArea1Collision_;
	bool isArea2Collision_;
	bool isArea3Collision_;
	bool isArea4Collision_;
	bool isArea5Collision_;

	// �e�N�X�`��
	int texId_;

	// �f���^�^�C��
	float deltaTime_;

	// ���ߗp�̃^�C��
	float alphaTime_;

	// �f�B�]���u�����邩�ǂ���
	bool isDissolve_;


	float time_;

	// �Փ˃��f�����X�g
	std::vector<CollisionModel> collisionModels_;

	// ���f������̊�{����������
	void CollisionInit(std::unique_ptr<Transform>& transform, const int modelId, const bool isCollision);

};

