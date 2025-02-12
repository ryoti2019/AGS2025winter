#pragma once
#include <functional>
#include <map>
#include "Component.h"
#include "../Object/Common/InputController.h"

class Player;

class InputComponent : public Component
{

public:

	// �A�b�p�[�̃N�[���^�C��
	static constexpr float UPPER_COOL_TIME = 2.0f;

	InputComponent(std::shared_ptr<Player> player);

	void Update(const float deltaTime);

	void Dir();

private:

	// �v���C���[�̃|�C���^�[
	std::shared_ptr<Player> player_;

	// ���͗p�R���g���[���[
	std::unique_ptr<InputController> inputController_;

	// ���߃p���`�̃J�E���^
	float attackChargePunchCnt_;

	// �A�b�p�[�̃N�[���^�C��
	float upperCoolTimeCnt_;

	// �ړ�����
	void Move();

	// �U������
	void Attack(const float deltaTime);

	// �������
	void Evasion();

};

