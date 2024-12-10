#pragma once
#include "Component.h"

class Boss;

class BossAIComponent : public Component
{
public:

	BossAIComponent(const std::shared_ptr<Boss> boss);

	void Update(const float deltaTime);

private:

	// �G�̃|�C���^�[
	std::shared_ptr<Boss> boss_;

	// �A�N�V������I��
	void SelectAction(const float deltaTime);

	// �ړ�����
	void Move(const float deltaTime);

	// �ߋ����U������
	void CloseRangeAttack(const float deltaTime);

	// �������U������
	void LongRangeAttack(const float deltaTime);

};

