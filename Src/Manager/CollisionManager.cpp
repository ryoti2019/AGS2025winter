#define NOMINMAX
#include <map>
#include <algorithm>
#include <cmath>
#include "CollisionManager.h"

void CollisionManager::Init()
{
}

void CollisionManager::Update(const float deltaTime)
{

	// プレイヤーと敵の攻撃の当たり判定をチェック
	CheckAttackCollision(deltaTime);

	// プレイヤーと敵のステージの当たり判定をチェック
	CheckStageCollision();

	// プレイヤーと敵同士が重ならないようにする当たり判定をチェック
	CheckResolveCollision();

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

void CollisionManager::CheckAttackCollision(const float deltaTime)
{

	// 無敵時間の計算
	for (auto& data : invincibleData_)
	{
		for (auto& state : data.second)
		{
			// 無敵時間を減算していく
			state.second -= deltaTime;
		}
	}

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

				// 中身があるか確認
				auto hitData = invincibleData_.find(target);

				// 生成されていない場合は、新しくvector配列の箱を作りその中に要素を入れていく
				if (hitData == invincibleData_.end())
				{

					// 当たったもののデータを作るattacker->GetToatlAttackTypes()
					std::map<int, float> data;
					for (int type = 0; type < attacker->GetToatlAttackTypes().size(); type++)
					{
						data.emplace(type, 0.0f);
					}

					// 当たったものを格納
					invincibleData_.emplace(target, data);

					// 上で作ったデータを格納
					hitData = invincibleData_.find(target);

				}

				// 今攻撃している側のアニメーションが要素にあるか確認する
				auto a = hitData->second.find(attacker->GetState() - 3);

				// 中身が無ければ処理しない
				if (a == hitData->second.end())return;

				// このアニメーション中の無敵時間が消えていなければ処理しない
				if (a->second > 0.0f)return;

				// 右手の判定
				if (HitCheck_Capsule_Capsule(attacker->GetCollisionData().rightHandCapsuleUpPos, attacker->GetCollisionData().rightHandCapsuleDownPos,
					attacker->GetCollisionData().handAndFootCollisionRadius,
					target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius)
					&& attacker->GetCollisionData().isRightHandAttack)
				{
					// 当たった時の処理
					OnAttackCollision(attacker, target);
				}
				// 左手の判定
				if (HitCheck_Capsule_Capsule(attacker->GetCollisionData().leftHandCapsuleUpPos, attacker->GetCollisionData().leftHandCapsuleDownPos,
					attacker->GetCollisionData().handAndFootCollisionRadius,
					target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius)
					&& attacker->GetCollisionData().isLeftHandAttack)
				{
					// 当たった時の処理
					OnAttackCollision(attacker, target);
				}
				// 右足の判定
				if (HitCheck_Capsule_Capsule(attacker->GetCollisionData().rightFootCapsuleUpPos, attacker->GetCollisionData().rightFootCapsuleDownPos,
					attacker->GetCollisionData().handAndFootCollisionRadius,
					target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius)
					&& attacker->GetCollisionData().isRightFootAttack)
				{
					// 当たった時の処理
					OnAttackCollision(attacker, target);
				}
				// 左足の判定
				if (HitCheck_Capsule_Capsule(attacker->GetCollisionData().leftFootCapsuleUpPos, attacker->GetCollisionData().leftFootCapsuleDownPos,
					attacker->GetCollisionData().handAndFootCollisionRadius,
					target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius)
					&& attacker->GetCollisionData().isLeftFootAttack)
				{
					// 当たった時の処理
					OnAttackCollision(attacker, target);
				}
			}
		}
	}

}

void CollisionManager::OnAttackCollision(const std::shared_ptr<ActorBase>& attacker, const std::shared_ptr<ActorBase>& target)
{

	// 攻撃がずっと当たらないようにする
	attacker->SetIsAttackHit(true);

	// 攻撃が当たった処理
	target->AttackHit(attacker->GetDamage(), attacker->GetState());

	// 相手の座標を設定
	target->SetTargetPos(attacker->GetTransform().pos);

	// 当たったターゲットの情報を取得
	auto& data = invincibleData_[target];

	// ターゲットに今攻撃された攻撃状態の無敵時間を設定する
	data[attacker->GetState() - 3] = 1.0f;

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
							
							// y方向には処理しない
							if (hit.Normal.y == 1.0f)continue;

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

void CollisionManager::CheckResolveCollision()
{

	// 衝突しているか判定する
	for (const collisionChannnelInfo& info : resolveCollisionChannelList_)
	{

		// アクター1
		const auto& actors1 = collisionActorData_.find(info.type1);

		// アクター2
		const auto& actors2 = collisionActorData_.find(info.type2);

		// 中身が入っているか確認
		if (actors1 == collisionActorData_.end())continue;
		if (actors2 == collisionActorData_.end())continue;

		for (const std::shared_ptr<ActorBase>& actor1 : actors1->second)
		{
			for (const std::shared_ptr<ActorBase>& actor2 : actors2->second)
			{

				// ポインタが入っている確認
				if (!actor1)return;
				if (!actor2)return;

				// 直方体の当たり判定の図形を作る
				// アクター1
				// 最小点
				VECTOR actorPos1Min = actor1->GetCollisionData().minPos;

				//最大点
				VECTOR actorPos1Max = actor1->GetCollisionData().maxPos;

				// アクター2
				// 最小値
				VECTOR actorPos2Min = actor2->GetCollisionData().minPos;

				//最大点
				VECTOR actorPos2Max = actor2->GetCollisionData().maxPos;

				// 各軸方向の重なり量を計算
				float overlapX = std::min(actorPos1Max.x, actorPos2Max.x) - std::max(actorPos1Min.x, actorPos2Min.x);
				float overlapY = std::min(actorPos1Max.y, actorPos2Max.y) - std::max(actorPos1Min.y, actorPos2Min.y);
				float overlapZ = std::min(actorPos1Max.z, actorPos2Max.z) - std::max(actorPos1Min.z, actorPos2Min.z);

				// 重なっていなければ通らない
				if (overlapX <= 0 || overlapY <= 0 || overlapZ <= 0)continue;

				// 重なってる分ずらす座標
				VECTOR pos = Utility::VECTOR_ZERO;

				// もし重なりがあれば解消する
				if (overlapX < overlapY && overlapX < overlapZ)
				{
					// 1.0fにして符号だけ取得する
					float x = std::copysignf(1.0f, overlapX);
					x = x * -10.0f;
					// aのX軸方向に移動
					if (actorPos1Max.x > actorPos2Max.x)
					{
						// 重なってる分を代入
						pos = { x,0.0f,0.0f };
						actor2->AddPos(pos);
					}
					else
					{
						// 重なってる分を代入
						pos = { x,0.0f,0.0f };
						actor2->SubPos(pos);
					}
				}
				// Y軸方向
				else if (overlapY < overlapZ)
				{
					// 1.0fにして符号だけ取得する
					float y = std::copysignf(1.0f, overlapY);
					y = y * -10.0f;
					// aのY軸方向に移動
					if (actorPos1Max.y > actorPos2Max.y)
					{
						// 重なってる分を代入
						pos = { 0.0f,y,0.0f };
						actor2->AddPos(pos);
					}
					else
					{
						// 重なってる分を代入
						pos = { 0.0f,y,0.0f };
						actor2->SubPos(pos);
					}
				}
				// Z軸方向
				else
				{
					// 1.0fにして符号だけ取得する
					float z = std::copysignf(1.0f, overlapZ);
					z = z * -10.0f;
					// aのZ軸方向に移動
					if (actorPos1Max.z > actorPos2Max.z)
					{
						// 重なってる分を代入
						pos = { 0.0f,0.0f,z };
						actor2->AddPos(pos);
					}
					else
					{
						// 重なってる分を代入
						pos = { 0.0f,0.0f,z };
						actor2->SubPos(pos);
					}
				}
			}
		}
	}

}