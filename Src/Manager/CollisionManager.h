#pragma once
#include "../Object/ActorBase.h"


class CollisionManager
{

public:

	CollisionManager() = default;
	~CollisionManager() = default;

	void Init();
	void Update();

	void Register(const std::shared_ptr<ActorBase>& actor);

private:

	struct collisionChannnelInfo
	{
		ActorType type1;
		ActorType type2;
	};

	const std::vector<collisionChannnelInfo> collisionChannelList_ =
	{
		{{ActorType::PLAYER,ActorType::ENEMY}}
	};

	// すべてのアクターの衝突判定をまとめたデータ
	std::unordered_map<ActorType, std::vector<std::shared_ptr<ActorBase>>> collisionActorData_;

	void CollisionCheck();

};

