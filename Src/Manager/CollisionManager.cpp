#include "CollisionManager.h"

void CollisionManager::Init()
{
}

void CollisionManager::Update()
{
	CollisionCheck();
}

void CollisionManager::Register(const std::shared_ptr<ActorBase>& actor)
{

	// collisionActorData_の中にすでに同じ型が生成されているかチェックする
	auto collisionElem = collisionActorData_.find(actor->GetActorType());

	// 生成されていない場合は、新しくvector配列の箱を作りその中に要素を入れていく
	if (collisionElem == collisionActorData_.end())
	{
		std::vector<std::shared_ptr<ActorBase>> data;
		data.emplace_back(actor);
		collisionActorData_.emplace(actor->GetActorType(), data);
	}
	// 生成されている場合はすでにある箱の中に要素を入れていく
	else
	{
		collisionElem->second.emplace_back(actor);
	}

}

void CollisionManager::CollisionCheck()
{

	// 衝突しているか判定する
	for (const collisionChannnelInfo& info : collisionChannelList_)
	{

		// 攻撃している側
		const auto& data1 = collisionActorData_.find(info.type1);

		// 攻撃を受ける側
		const auto& data2 = collisionActorData_.find(info.type2);

		// 中身が入っているか確認
		if (data1 == collisionActorData_.end())continue;
		if (data2 == collisionActorData_.end())continue;

		for (const std::shared_ptr<ActorBase>& actor1 : data1->second)
		{
			for (const std::shared_ptr<ActorBase>& actor2 : data2->second)
			{

				// ポインタが入っている確認
				if (!actor1)return;
				if (!actor2)return;

				if (!actor1->GetAttackState())return;

				// 右手の判定
				if (HitCheck_Capsule_Capsule(actor1->GetCollisionData().rightHandCapsuleUpPos, actor1->GetCollisionData().rightHandCapsuleDownPos,
					actor1->GetCollisionData().handAndFootCollisionRadius,
					actor2->GetCollisionData().bodyCapsuleUpPos, actor2->GetCollisionData().bodyCapsuleDownPos, actor2->GetCollisionData().bodyCollisionRadius))
				{
					// 攻撃が当たった処理
					actor2->AttackHit();
				}
				// 左手の判定
				if (HitCheck_Capsule_Capsule(actor1->GetCollisionData().leftHandCapsuleUpPos, actor1->GetCollisionData().leftHandCapsuleDownPos,
					actor1->GetCollisionData().handAndFootCollisionRadius,
					actor2->GetCollisionData().bodyCapsuleUpPos, actor2->GetCollisionData().bodyCapsuleDownPos, actor2->GetCollisionData().bodyCollisionRadius))
				{
					// 攻撃が当たった処理
					actor2->AttackHit();
				}
				// 右足の判定
				if (HitCheck_Capsule_Capsule(actor1->GetCollisionData().rightFootCapsuleUpPos, actor1->GetCollisionData().rightFootCapsuleDownPos,
					actor1->GetCollisionData().handAndFootCollisionRadius,
					actor2->GetCollisionData().bodyCapsuleUpPos, actor2->GetCollisionData().bodyCapsuleDownPos, actor2->GetCollisionData().bodyCollisionRadius))
				{
					// 攻撃が当たった処理
					actor2->AttackHitFly();
				}
				// 左足の判定
				if (HitCheck_Capsule_Capsule(actor1->GetCollisionData().leftFootCapsuleUpPos, actor1->GetCollisionData().leftFootCapsuleDownPos,
					actor1->GetCollisionData().handAndFootCollisionRadius,
					actor2->GetCollisionData().bodyCapsuleUpPos, actor2->GetCollisionData().bodyCapsuleDownPos, actor2->GetCollisionData().bodyCollisionRadius))
				{
					// 攻撃が当たった処理
					actor2->AttackHit();
				}
			}
		}
	}
}