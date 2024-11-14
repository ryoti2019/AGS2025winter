#include <map>
#include "CollisionManager.h"

void CollisionManager::Init()
{
}

void CollisionManager::Update()
{

	// プレイヤーと敵の攻撃の当たり判定をチェック
	CheckAttackCollision();

	// プレイヤーと敵のステージの当たり判定をチェック
	CheckStageCollision();

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

void CollisionManager::CheckAttackCollision()
{

	// 衝突しているか判定する
	for (const collisionChannnelInfo& info : attackCollisionChannelList_)
	{

		// 攻撃している側
		const auto& attackers = collisionActorData_.find(info.type1);

		// 攻撃を受ける側
		const auto& targets = collisionActorData_.find(info.type2);

		// 中身が入っているか確認
		if (attackers == collisionActorData_.end())continue;
		if (targets == collisionActorData_.end())continue;

		for (const std::shared_ptr<ActorBase>& attacker : attackers->second)
		{
			for (const std::shared_ptr<ActorBase>& target : targets->second)
			{

				// ポインタが入っている確認
				if (!attacker)return;
				if (!target)return;

				// 攻撃状態に入っていなかったら当たり判定を通らない
				if (!attacker->GetAttackState())continue;

				//// 中身があるか確認
				//auto hitData = attackCollisionData_.find(target->GetActorType());

				//// 中身があれば処理する
				//if (hitData != attackCollisionData_.end())
				//{
				//	// すでに当たっていたら処理しない
				//	if (std::find(hitData->second.begin(), hitData->second.end(), target) != hitData->second.end())continue;
				//}

				// 攻撃がすでに当たっていたら当たり判定を通らない
				//if (attacker->GetIsAttackHit())continue;

				// 攻撃を受けている状態だったら当たり判定を通らない
				//if (target->GetHitState())continue;

				// 右手の判定
				if (HitCheck_Capsule_Capsule(attacker->GetCollisionData().rightHandCapsuleUpPos, attacker->GetCollisionData().rightHandCapsuleDownPos,
					attacker->GetCollisionData().handAndFootCollisionRadius,
					target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius)
					&& attacker->GetCollisionData().isRightHandAttack)
				{

					// 攻撃がずっと当たらないようにする
					attacker->SetIsAttackHit(true);

					// 攻撃が当たった処理
					target->AttackHit(attacker->GetDamage(), attacker->GetState());

					// 相手の座標を設定
					target->SetTargetPos(attacker->GetPos());

					//// 当たったもののデータを作る
					//std::vector<std::shared_ptr<ActorBase>> data;
					//data.emplace_back(target);

					//// 当たったものを格納
					//attackCollisionData_.emplace(target->GetActorType(), data);

				}
				// 左手の判定
				if (HitCheck_Capsule_Capsule(attacker->GetCollisionData().leftHandCapsuleUpPos, attacker->GetCollisionData().leftHandCapsuleDownPos,
					attacker->GetCollisionData().handAndFootCollisionRadius,
					target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius)
					&& attacker->GetCollisionData().isLeftHandAttack)
				{

					// 攻撃がずっと当たらないようにする
					attacker->SetIsAttackHit(true);

					// 攻撃が当たった処理
					target->AttackHit(attacker->GetDamage(), attacker->GetState());

					// 相手の座標を設定
					target->SetTargetPos(attacker->GetPos());

					//// 当たったもののデータを作る
					//std::vector<std::shared_ptr<ActorBase>> data;
					//data.emplace_back(target);

					//// 当たったものを格納
					//attackCollisionData_.emplace(target->GetActorType(), data);

				}
				// 右足の判定
				if (HitCheck_Capsule_Capsule(attacker->GetCollisionData().rightFootCapsuleUpPos, attacker->GetCollisionData().rightFootCapsuleDownPos,
					attacker->GetCollisionData().handAndFootCollisionRadius,
					target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius)
					&& attacker->GetCollisionData().isRightFootAttack)
				{

					// 攻撃がずっと当たらないようにする
					attacker->SetIsAttackHit(true);

					// 攻撃が当たった処理
					target->AttackHit(attacker->GetDamage(), attacker->GetState());

					// 相手の座標を設定
					target->SetTargetPos(attacker->GetPos());

					//// 当たったもののデータを作る
					//std::vector<std::shared_ptr<ActorBase>> data;
					//data.emplace_back(target);

					//// 当たったものを格納
					//attackCollisionData_.emplace(target->GetActorType(), data);

				}
				// 左足の判定
				if (HitCheck_Capsule_Capsule(attacker->GetCollisionData().leftFootCapsuleUpPos, attacker->GetCollisionData().leftFootCapsuleDownPos,
					attacker->GetCollisionData().handAndFootCollisionRadius,
					target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius)
					&& attacker->GetCollisionData().isLeftFootAttack)
				{

					// 攻撃がずっと当たらないようにする
					attacker->SetIsAttackHit(true);

					// 攻撃が当たった処理
					target->AttackHit(attacker->GetDamage(), attacker->GetState());

					// 相手の座標を設定
					target->SetTargetPos(attacker->GetPos());

					//// 当たったもののデータを作る
					//std::vector<std::shared_ptr<ActorBase>> data;
					//data.emplace_back(target);

					//// 当たったものを格納
					//attackCollisionData_.emplace(target->GetActorType(), data);

				}
			}
		}
	}

}

void CollisionManager::CheckStageCollision()
{

	// 衝突しているか判定する
	for (const collisionChannnelInfo& info : stageCollisionChannelList_)
	{

		// ステージと当たっているか判定したいもの
		const auto& targets = collisionActorData_.find(info.type1);

		// ステージ
		const auto& stages = collisionActorData_.find(info.type2);

		// 中身が入っているか確認
		if (targets == collisionActorData_.end())continue;
		if (stages == collisionActorData_.end())continue;

		for (const std::shared_ptr<ActorBase>& target : targets->second)
		{
			for (const std::shared_ptr<ActorBase>& stage : stages->second)
			{

				// ポインタが入っている確認
				if (!target)return;
				if (!stage)return;

				// カプセルとの衝突判定
				auto hits = MV1CollCheck_Capsule(
					stage->GetTransform().modelId, -1,
					target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius);

				// 衝突した複数のポリゴンと衝突回避するまで、
				// プレイヤーの位置を移動させる
				for (int i = 0; i < hits.HitNum; i++)
				{

					auto hit = hits.Dim[i];

					// 地面と異なり、衝突回避位置が不明なため、何度か移動させる
					// この時、移動させる方向は、移動前座標に向いた方向であったり、
					// 衝突したポリゴンの法線方向だったりする
					for (int tryCnt = 0; tryCnt < 10; tryCnt++)
					{

						// 再度、モデル全体と衝突検出するには、効率が悪過ぎるので、
						// 最初の衝突判定で検出した衝突ポリゴン1枚と衝突判定を取る
						int pHit = HitCheck_Capsule_Triangle(
							target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius,
							hit.Position[0], hit.Position[1], hit.Position[2]);

						if (pHit)
						{

							// 当たっているか判定したいActorの座標
							VECTOR pos = target->GetTransform().pos;

							// 法線の方向にちょっとだけ移動させる
							pos = VAdd(pos, VScale(hit.Normal, 30.0f));

							// カプセルも一緒に移動させる
							target->SetPos(pos);
							continue;

						}
						break;
					}
				}

				// 検出した地面ポリゴン情報の後始末
				MV1CollResultPolyDimTerminate(hits);
				
				// 地面との衝突
				auto hit = MV1CollCheck_Line(
					stage->GetTransform().modelId, -1,
					VAdd(target->GetCollisionData().bodyCapsuleUpPos, VECTOR(0.0f,target->GetCollisionData().bodyCollisionRadius,0.0f)),
					VAdd(target->GetCollisionData().bodyCapsuleDownPos, VECTOR(0.0f, -target->GetCollisionData().bodyCollisionRadius, 0.0f)));
				if (hit.HitFlag > 0 && VDot({0.0f,-1.0f,0.0f}, target->GetVelocity()) > 0.9f)
				{

					// 衝突地点から、少し上に移動
					target->SetPos(hit.HitPosition);

					// ジャンプリセット
					target->SetVelocity(Utility::VECTOR_ZERO);

				}
			}
		}
	}

}
