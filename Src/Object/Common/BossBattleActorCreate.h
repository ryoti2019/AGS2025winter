#pragma once
#include "CreateBase.h"

class BossBattleActorCreate : public CreateBase
{

public:

	BossBattleActorCreate();

	~BossBattleActorCreate()override = default;

	void Update()override;

	void Draw()override;

private:

};

