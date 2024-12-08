#pragma once
#include "Component.h"

class Enemy;

class EnemyAIComponent : public Component
{

public:

	EnemyAIComponent(const std::shared_ptr<Enemy> enemy);

	void Update(const float deltaTime);

private:

	// �G�̃|�C���^�[
	std::shared_ptr<Enemy> enemy_;

	// �A�N�V������I��
	void SelectAction(const float deltaTime);

	// �ړ�����
	void Move(const float deltaTime);

	// �U������
	void Attack(const float deltaTime);

};

