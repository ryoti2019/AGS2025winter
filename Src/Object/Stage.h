#pragma once
#include "ActorBase.h"

class Stage : public ActorBase
{

public:

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

	// �Փ˃��f�����X�g���擾
	const std::vector<CollisionModel>& GetCollisionModels()const { return collisionModels_; }

private:

	// �X�e�[�W�̃R���W�������f��
	std::shared_ptr<Transform> collision_;

	// �G���A1�̃R���W�������f��
	std::shared_ptr<Transform> area1Collision_;

	// �Փ˃��f�����X�g
	std::vector<CollisionModel> collisionModels_;

};

