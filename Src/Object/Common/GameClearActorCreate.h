#pragma once
#include "CreateBase.h"
class GameClearActorCreate : public CreateBase
{

public:

	GameClearActorCreate();

	~GameClearActorCreate()override = default;

private:

	// �v���C���[�̍��W
	static constexpr VECTOR PLAYER_POS = { -8700.0f,-19500.0f,-140000.0f };

};

