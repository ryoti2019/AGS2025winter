#pragma once
#include "../Object/ActorBase.h"

class CollisionManager
{

public:

	CollisionManager() = default;
	~CollisionManager() = default;

	void Init();
	void Update();

	// 当たり判定を見たいものを登録する
	void Register(const std::shared_ptr<ActorBase>& actor);

private:

	// ２つのActorの当たり判定を見るために組み合わせを登録する型
	struct collisionChannnelInfo
	{
		ActorType type1;
		ActorType type2;
	};

	// 敵とプレイヤーの攻撃が当たっているかの組み合わせを登録する
	const std::vector<collisionChannnelInfo> attackCollisionChannelList_ =
	{
		{
			{ActorType::PLAYER,ActorType::ENEMY},
			//{ActorType::ENEMY,ActorType::PLAYER}
		}
	};

	// 敵とプレイヤーがステージに当たっているかの組み合わせを登録する
	const std::vector<collisionChannnelInfo> stageCollisionChannelList_ =
	{
		{
			{ActorType::PLAYER,ActorType::STAGE},
			{ActorType::ENEMY,ActorType::STAGE}
		}
	};

	// すべてのアクターの衝突判定をまとめたデータ
	std::unordered_map<ActorType, std::vector<std::shared_ptr<ActorBase>>> collisionActorData_;

	// 攻撃が当たった敵をまとめたデータ
	std::unordered_map<ActorType, std::vector<std::shared_ptr<ActorBase>>> attackCollisionData_;

	// アクターの状態をまとめたデータ


	// プレイヤーと敵の攻撃の当たり判定
	void CheckAttackCollision();

	// プレイヤーと敵のステージとの当たり判定
	void CheckStageCollision();

};

