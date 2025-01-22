#pragma once
#include "StageBase.h"

class StageCollision : public StageBase
{

public:

	StageCollision(const VECTOR& pos, const json& data);

	~StageCollision()override = default;

	void Draw(const float deltaTime)override;

private:

};

