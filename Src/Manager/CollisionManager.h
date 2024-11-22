#pragma once
#include "../Object/ActorBase.h"

class CollisionManager
{

public:

	// 攻撃の最初の状態の番号
	const int ATTACK_START_NUM;

	// ステージと衝突しているか判定する回数
	const int STAGE_COLLISION_COUNT;

	// 敵同士の押し合う力
	const float ENEMY_PUSH_FORCE;

	// ステージとの当たり判定で押し返す力
	const float STAGE_PUSH_FORCE;

	// ほぼ真下への向き
	const float DOWN_DIR;

	CollisionManager();
	~CollisionManager() = default;

	void Init();
	void Update(const float deltaTime);

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
			{ActorType::ENEMY,ActorType::PLAYER}
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

	// プレイヤーと敵同士が当たっているかの組み合わせを登録する
	const std::vector<collisionChannnelInfo> resolveCollisionChannelList_ =
	{
		{
			{ActorType::PLAYER,ActorType::ENEMY},
			{ActorType::ENEMY,ActorType::ENEMY}
		}
	};

	// どれだけ重なっているか
	VECTOR overlap_;

	// すべてのアクターの衝突判定をまとめたデータ
	std::unordered_map<ActorType, std::vector<std::shared_ptr<ActorBase>>> collisionActorData_;

	// 攻撃が当たっているかどうかのデータ
	std::map<std::shared_ptr<ActorBase>, std::map<int, float>> invincibleData_;

	// プレイヤーと敵の攻撃の当たり判定
	void CheckAttackCollision(const float deltaTime);

	// 当たった時の処理
	void OnAttackCollision(const std::shared_ptr<ActorBase>& attacker, const std::shared_ptr<ActorBase>& target);

	// プレイヤーと敵のステージとの当たり判定
	void CheckStageCollision();

	// プレイヤーや敵同士が重なってた時の当たり判定
	void CheckResolveCollision();

	// プレイヤーと敵が重なってた時の処理
	void ResolvePlayerEnemyCollision(const std::shared_ptr<ActorBase>& actor1, const std::shared_ptr<ActorBase>& actor2);

	// 敵同士が重なってた時の処理
	void ResolveEnemysCollision(const std::shared_ptr<ActorBase>& actor1, const std::shared_ptr<ActorBase>& actor2);

};

