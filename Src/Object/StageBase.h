#pragma once
#include "ActorBase.h"

class ModelMaterial;
class Renderer;

class StageBase : public ActorBase
{

public:

	StageBase(const VECTOR& pos, const json& data);

	//virtual ~StageBase() = default;
	virtual ~StageBase() override = default;

	void Init(const VECTOR& pos)override;

	void Update(const float deltaTime)override;

	virtual void Draw(const float deltaTime)override;

	// �Փ˂̕ǂ̕`��
	void CollisionDraw(const float deltaTime);

	// �����蔻��̐ݒ�
	void SetIsCollision(const bool isCollision) { isStageCollision_ = isCollision; }

	// �����蔻����擾
	const bool GetIsCollision() { return isStageCollision_; }

	// �f�B�]���u�����邩�ݒ�
	const bool GetIsDissolve()const { return isDissolve_; }

	// �f�B�]���u�����邩�ݒ�
	void SetIsDissolve(const bool isDissolve) { isDissolve_ = isDissolve; }

	// �ǂ̂��炢���߂��Ă��邩�擾
	const float GetAlphaTime() const { return alphaTime_; }

	// �����G���A�Ƃ̓����蔻����o�����W���擾
	const VECTOR GetRestrictedAreaPos() const { return restrictedAreaPos_; }

	// �����G���A�Ƃ̓����蔻����o�����a���擾
	const float GetRestrictedAreaRadius()const { return restrictedAreaRadius_; }

	// �����G���A���̔�����擾
	const bool GetIsRestrictedArea()const { return isRestrictedArea_; }

	// �����G���A���̔�����擾
	void SetIsRestrictedArea(const bool isRestrictedArea) { isRestrictedArea_ = isRestrictedArea; }

protected:

	// �X�e�[�W�̃R���W�������f��
	std::unique_ptr<Transform> stageCollision_;

	// ���f���}�e���A��
	std::shared_ptr<ModelMaterial> modelMaterial_;

	// �����_���[
	std::shared_ptr<Renderer> renderer_;

	// �X�e�[�W���f��ID
	int stageCollisionModelId_;

	// �X�e�[�W�̏Փ˔��肵�Ă��邩�ǂ���
	bool isStageCollision_;

	// �e�N�X�`��
	int texId_;

	// �f���^�^�C��
	float deltaTime_;

	// ���ߗp�̃^�C��
	float alphaTime_;

	// �f�B�]���u�����邩�ǂ���
	bool isDissolve_;

	// �����G���A�Ƃ̓����蔻����o�����W
	VECTOR restrictedAreaPos_;

	// �����G���A�Ƃ̓����蔻����o�����a
	float restrictedAreaRadius_;
	
	// �����G���A���̔���
	bool isRestrictedArea_;

	// ���f������̊�{����������
	void CollisionInit(std::shared_ptr<Transform>& transform);

private:

};

