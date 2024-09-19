#pragma once
#include <DxLib.h>
#include "ActorBase.h"

class Player : public ActorBase
{

public:

	using ActorBase::ActorBase;

	Player(const VECTOR& pos) : movedPos_(0.0f) : ActorBase(pos),
		
		
	{
	};

	~Player() = default;

	void Init(const VECTOR& pos) override;
	void Update(const float deltaTime) override;

private:

};

