#pragma once
#include "Component.h"
#include "../Object/Common/InputController.h"

class Player;

class InputComponent : public Component
{

public:

	InputComponent(std::shared_ptr<Player>& player);

	void Update(const float deltaTime);

private:

	// �ړ�����
	void Move();

	// �U������
	void Attack(const float deltaTime);

	// �v���C���[�̃|�C���^�[
	std::shared_ptr<Player> player_;

	// ���͗p�R���g���[���[
	std::unique_ptr<InputController> inputController_;

};

