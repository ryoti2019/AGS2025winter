#pragma once
#include "Component.h"
#include "../Object/Common/Transform.h"

class Player;

class MoveComponent : public Component
{

public:

	MoveComponent(std::shared_ptr<ActorBase> actor);

	// 走る移動
	void Move();

	// ゆっくり移動
	void Lerp();

private:

};

