#pragma once
#include "StageBase.h"
class Area5Collision : public StageBase
{

public:

	Area5Collision(const VECTOR& pos, const json& data);

	~Area5Collision()override = default;

	void Draw(const float deltaTime)override;

private:

};

