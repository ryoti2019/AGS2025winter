#pragma once
#include "StageBase.h"
class Area4Collision : public StageBase
{

public:

	Area4Collision(const VECTOR& pos, const json& data);

	~Area4Collision() = default;

	void Draw(const float deltaTime)override;

private:

};

