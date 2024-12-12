#pragma once
class ActorCreate
{

public:

	// エリア1のすべての敵の数
	const int AREA1_TOTAL_ENEMYS;

	// エリア2のすべての敵の数
	const int AREA2_TOTAL_ENEMYS;

	// エリア3のすべての敵の数
	const int AREA3_TOTAL_ENEMYS;

	// エリア4のすべての敵の数
	const int AREA4_TOTAL_ENEMYS;

	// エリア5のすべての敵の数
	const int AREA5_TOTAL_ENEMYS;

	ActorCreate();

	~ActorCreate() = default;

	void Update();

	void Draw();

private:

	// エリア1と衝突しているか
	bool isCollisionArea1_;

	// エリア2と衝突しているか
	bool isCollisionArea2_;

	// 死んだ敵の数をカウント
	int deathEnemyCnt_;

	// エリアの衝突判定の処理
	void AreaCollision();

	// エリアの衝突判定をなくす処理
	void DeactiveAreaCollision();

	// 敵を何体倒したかカウントする処理
	void CountDeathEnemys();

};

