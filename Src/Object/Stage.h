#pragma once
#include "ActorBase.h"

class Stage : public ActorBase
{

public:

	Stage(const VECTOR& pos, const json& data);

	~Stage() = default;

	void Init(const VECTOR& pos)override;

private:

	// �X�e�[�W�̃R���W�������f��
	std::shared_ptr<Transform> collision_;

};

