#pragma once
#include "CreateBase.h"

class BossAppearanceCreate : public CreateBase
{

public:

	BossAppearanceCreate();

	~BossAppearanceCreate()override = default;

	void Update()override;

	void Draw()override;

private:

	// プレイヤーの座標
	static constexpr VECTOR PLAYER_POS = { -10800.0f,-19500.0f,-120000.0f };

	// ボスの座標
	static constexpr VECTOR BOSS_POS = { -10800.0f,10380.0f,-156000.0f };

};

