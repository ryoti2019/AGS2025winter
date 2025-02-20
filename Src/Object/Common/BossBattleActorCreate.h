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

	// プレイヤーの初期座標
	static constexpr VECTOR PLAYER_INIT_POS = { -10800.0f,-19500.0f,-120000.0f };

	// 敵を生成する数
	static constexpr int CREATE_ENEMYS = 50;

	// ボスの初期座標
	static constexpr VECTOR BOSS_INIT_POS = { -10800.0f,-19500.0f,-140000.0f };

	// 一度に生成する敵の数
	static constexpr int ENEMY_CREATE_NUM = 5;

	// 敵が出現する座標
	static constexpr VECTOR ENEMY_POS = { -9000.0f, 1000.0f, -140000.0f };

};

