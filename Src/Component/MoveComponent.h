#pragma once
#include "Component.h"
#include "TransformComponent.h"
class MoveComponent : public Component
{

public:

	MoveComponent();

	~MoveComponent();

	void Update(ActorBase& actor, const VECTOR& moveDir, float speed);

private:

	std::shared_ptr<TransformComponent> transform_;

};

