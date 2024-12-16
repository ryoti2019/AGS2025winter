#pragma once
#include "StageBase.h"
class Area3Collision : public StageBase
{

public:

	Area3Collision(const VECTOR& pos, const json& data);

	~Area3Collision() = default;

	void Draw(const float deltaTime)override;

private:

};

