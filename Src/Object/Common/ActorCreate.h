#pragma once
#include "CreateBase.h"

class ActorCreate : public CreateBase
{

public:

	// エリア1のすべての敵の数
	static constexpr int AREA1_TOTAL_ENEMYS = 10;

	// エリア2のすべての敵の数
	static constexpr int AREA2_TOTAL_ENEMYS = 10;

	// エリア3のすべての敵の数
	static constexpr int AREA3_TOTAL_ENEMYS = 10;

	// エリア4のすべての敵の数
	static constexpr int AREA4_TOTAL_ENEMYS = 10;

	// プレイヤーの初期値
	static constexpr VECTOR PLAYER_INIT_POS = { -80000.0f, -19500.0f, 25900.0f };

	ActorCreate();

	~ActorCreate()override = default;

	void Update()override;

	void Draw()override;

private:

	// エリア1と衝突しているか
	bool isCollisionArea1_;

	// エリア2と衝突しているか
	bool isCollisionArea2_;

	// エリア3と衝突しているか
	bool isCollisionArea3_;

	// エリア4と衝突しているか
	bool isCollisionArea4_;

	// エリア5と衝突しているか
	bool isCollisionArea5_;

	// 死んだ敵の数をカウント
	int deathEnemyCnt_;

	// エリアの衝突判定の処理
	void AreaCollision();

	// エリアの衝突判定をなくす処理
	void DeactiveAreaCollision();

	// 敵を何体倒したかカウントする処理
	void CountDeathEnemys();

};

