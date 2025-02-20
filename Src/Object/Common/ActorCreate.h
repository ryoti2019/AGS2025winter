#pragma once
#include "CreateBase.h"

class ActorCreate : public CreateBase
{

public:

	ActorCreate();

	~ActorCreate()override = default;

	void Update()override;

	void Draw()override;

private:

	// エリア1のすべての敵の数
	static constexpr int AREA1_TOTAL_ENEMYS = 10;

	// エリア2のすべての敵の数
	static constexpr int AREA2_TOTAL_ENEMYS = 10;

	// エリア3のすべての敵の数
	static constexpr int AREA3_TOTAL_ENEMYS = 10;

	// エリア4のすべての敵の数
	static constexpr int AREA4_TOTAL_ENEMYS = 10;

	// 敵を生成する数
	static constexpr int CREATE_ENEMYS = 50;

	// エリア1の敵が出現する座標
	static constexpr VECTOR CREATE_ENEMY_POS1 = { 25000.0f, -18000.0f, 26000.0f };

	// エリア2の敵が出現する座標
	static constexpr VECTOR CREATE_ENEMY_POS2 = { -30000.0f, -18000.0f, -21000.0f };

	// エリア3の敵が出現する座標
	static constexpr VECTOR CREATE_ENEMY_POS3 = { -82000.0f, -18000.0f, -62000.0f };

	// エリア4の敵が出現する座標
	static constexpr VECTOR CREATE_ENEMY_POS4 = { -26000.0f, -18000.0f, -68000.0f };

	// プレイヤーの初期値
	static constexpr VECTOR PLAYER_INIT_POS = { -80000.0f, -19500.0f, 25900.0f };

	// 透明度の最大値
	static constexpr float ALPHA_TIME = 1.0f;

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

