#pragma once
#include "ActorBase.h"

class Stage : public ActorBase
{

public:

	Stage(const VECTOR& pos, const json& data);

	~Stage() = default;

	void Init(const VECTOR& pos) override;

private:


};

