#pragma once
#include <functional>
#include <map>
#include "Component.h"
#include "../Object/Common/InputController.h"

class Player;

class InputComponent : public Component
{

public:

	InputComponent(std::shared_ptr<Player>& player);

	void Update(const float deltaTime);

	// �A�N�V������ǉ�����
	void AddAction(ACTION action, std::function<void(void)> func);

private:

	// ���͗p�R���g���[���[
	std::unique_ptr<InputController> inputController_;

	// ���߃p���`�̃J�E���^
	float chargeCnt_;

	std::map<ACTION, std::function<void(void)>> funcMap_;

	// �ړ�����
	void Move();

	// �U������
	void Attack(const float deltaTime);

	// �v���C���[�̃|�C���^�[
	std::shared_ptr<Player> player_;

	// ���͗p�R���g���[���[
	std::unique_ptr<InputController> inputController_;

};

