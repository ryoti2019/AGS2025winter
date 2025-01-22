#pragma once
#include "Stage.h"
class Area1Collision : public StageBase
{
public:

	Area1Collision(const VECTOR& pos,const json& data);

	~Area1Collision()override = default;

	void Draw(const float deltaTime)override;

private:

};

