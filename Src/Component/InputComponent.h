#pragma once
#include "Component.h"
#include "../Object/Common/InputController.h"

class InputComponent : public Component
{

public:

	InputComponent();

	virtual void Update()override;

private:

	// �ړ�����
	void Move();

	// �U������
	void Attack(const float deltaTime);

	// ���͗p�R���g���[���[
	std::unique_ptr<InputController> inputController_;

	// ���߃p���`�̃J�E���^
	float chargeCnt_;

};

