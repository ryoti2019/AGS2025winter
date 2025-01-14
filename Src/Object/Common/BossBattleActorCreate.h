#pragma once
#include "CreateBase.h"

class BossBattleActorCreate : public CreateBase
{

public:

	BossBattleActorCreate();

	~BossBattleActorCreate() = default;

	void Update()override;

	void Draw()override;

private:

};

