#pragma once
#include "CreateBase.h"
class TitleActorCreate : public CreateBase
{

public:

	TitleActorCreate();

	~TitleActorCreate()override = default;

private:

	// プレイヤーの座標
	static constexpr VECTOR PLAYER_POS = { -80000.0f,-19500.0f,25900.0f };

};

