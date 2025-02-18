#pragma once
#include "Component.h"

class MoveComponent : public Component
{

public:

	MoveComponent(std::weak_ptr<ActorBase> actor);

	// ����ړ�
	void Move();

	// �������ړ�
	void Lerp();

	// ������ɔ��ł����ړ�
	void HitMove();

private:

	// �ړ���
	static constexpr float MOVE_SMOOTHNESS = 0.1f;

};

