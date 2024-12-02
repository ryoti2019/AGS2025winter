#pragma once
#include <functional>
#include <map>
#include "Component.h"
#include "../Object/Common/InputController.h"

class InputComponent : public Component
{

public:

	enum class ACTION
	{
		NONE,
		RUN,
		ATTACK,
	};

	InputComponent();

	virtual void Update()override;

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

};

