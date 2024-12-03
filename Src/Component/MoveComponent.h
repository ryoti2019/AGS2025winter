#pragma once
#include "Component.h"
#include "../Object/Common/Transform.h"

class Player;

class MoveComponent : public Component
{

public:

	MoveComponent(std::shared_ptr<ActorBase> actor);

	// ����ړ�
	void Move();

	// �������ړ�
	void Lerp();

	// ������ɔ��ł����ړ�
	void HitMove();

private:

};

