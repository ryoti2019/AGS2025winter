#pragma once
#include "StageBase.h"
class Area2Collision : public StageBase
{

public:

	Area2Collision(const VECTOR& pos, const json& data);

	~Area2Collision()override = default;

	void Draw(const float deltaTime)override;

private:

};

