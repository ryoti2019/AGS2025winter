#define NOMINMAX
#include <map>
#include <algorithm>
#include <cmath>
#include <vector>
#include <DxLib.h>
#include "CollisionManager.h"
#include "../Object/Stage.h"
#include "../Object/StageCollision.h"
#include "../Object/Area1Collision.h"
#include "../Object/Area2Collision.h"
#include "../Object/Area3Collision.h"
#include "../Object/Area4Collision.h"
#include "../Object/Area5Collision.h"

CollisionManager::CollisionManager()
	:
	ATTACK_START_NUM(3),
	STAGE_COLLISION_COUNT(10),
	ENEMY_PUSH_FORCE(10.0f),
	OBJECT_COLLISION_PUSH_FORCE(50.0f),
	CAMERA_COLLISION_PUSH_FORCE(1000.0f),
	DOWN_DIR(0.9f),
	LEN_CAMERACOLLISION_LINE(500.0f),
	MIN_LEN_CAMERA_COLLISION_LINE(1.0f),
	collisionLineStageCamera_(0.0f)
{

}

void CollisionManager::Init()
{
}

void CollisionManager::Update(const float deltaTime)
{

	// プレイヤーと敵の攻撃の当たり判定をチェック
	CheckAttackCollision(deltaTime);

	// プレイヤーと敵の飛び道具の当たり判定
	CheckProjectileCollision(deltaTime);

	// ステージとの当たり判定をチェック
	CheckStageCollision();

	// プレイヤーと敵同士が重ならないようにする当たり判定をチェック
	CheckResolveCollision();

	// 制限エリアが出てくる場所との当たり判定
	CheckRestrictedAreasCollision();

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

void CollisionManager::StageErasure(std::shared_ptr<StageBase>& actor)
{

	// ステージデータを探す
	auto stageData = collisionActorData_.find(ActorType::STAGE);

	// ステージデータの中身があるかチェック
	if (stageData == collisionActorData_.end()) return;

	// ステージのデータを取り出す
	auto& stages = collisionActorData_.at(ActorType::STAGE);

	// わたってきた引数と同じポインタがあれば配列から消す
	std::erase_if(stages, [&actor](std::shared_ptr<ActorBase> actVal)
		{
			return (actor == std::dynamic_pointer_cast<StageBase>(actVal));
		});

}

void CollisionManager::SetCamera(const std::weak_ptr<Camera>& camera)
{
	camera_ = camera;
}

void CollisionManager::CheckAttackCollision(const float deltaTime)
{

	// 無敵時間の計算
	for (auto& data : isCloseRangeAttackHitData_)
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

				// ポインタが入っているか確認
				if (!attacker)return;
				if (!target)return;

				// 攻撃状態に入っていなかったら当たり判定を通らない
				if (!attacker->GetCloseRangeAttackState())continue;

				// 中身があるか確認
				auto hitData = isCloseRangeAttackHitData_.find(target);

				// 生成されていない場合は、新しくvector配列の箱を作りその中に要素を入れていく
				if (hitData == isCloseRangeAttackHitData_.end())
				{

					// データを作る
					std::map<int, float> data;
					for (int type = 0; type < attacker->GetTotalAttackTypes().size(); type++)
					{
						data.emplace(type, 0.0f);
					}

					// 当たったものを格納
					isCloseRangeAttackHitData_.emplace(target, data);

					// 上で作ったデータを格納
					hitData = isCloseRangeAttackHitData_.find(target);

				}

				// 今攻撃している側のアニメーションが要素にあるか確認する
				auto a = hitData->second.find(attacker->GetState() - ATTACK_START_NUM);

				// 中身が無ければ処理しない
				if (a == hitData->second.end())continue;

				// このアニメーション中の無敵時間が消えていなければ処理しない
				if (a->second > 0.0f)continue;

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

	// 攻撃が当たった処理
	target->AttackHit(attacker->GetDamage(), attacker->GetState());

	// 相手の座標を設定
	target->SetTargetPos(attacker->GetTransform()->pos);

	// 攻撃が当たったフラグをtrueにする
	attacker->SetIsHitAttack(true);

	// 当たったターゲットの情報を取得
	auto& data = isCloseRangeAttackHitData_[target];

	// ターゲットに今攻撃された攻撃状態の無敵時間を設定する
	data[attacker->GetState() - ATTACK_START_NUM] = 1.0f;

}

void CollisionManager::CheckProjectileCollision(const float deltaTime)
{

	// 無敵時間の計算
	for (auto& data : isLongRangeAttackHitData_)
	{
		// 無敵時間を減算していく
		data.second -= deltaTime;
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

				// ポインタが入っているか確認
				if (!attacker)return;
				if (!target)return;

				// 中身があるか確認
				auto hitData = isLongRangeAttackHitData_.find(target);

				// 生成されていない場合は、新しくvector配列の箱を作りその中に要素を入れていく
				if (hitData == isLongRangeAttackHitData_.end())
				{

					// 無敵時間を作る
					float invincibleTime = 0.0f;

					// 当たったものを格納
					isLongRangeAttackHitData_.emplace(target, invincibleTime);

					// 上で作ったデータを格納
					hitData = isLongRangeAttackHitData_.find(target);

				}

				// このアニメーション中の無敵時間が消えていなければ処理しない
				if (hitData->second > 0.0f)continue;

				// 飛び道具の判定
				if (HitCheck_Sphere_Capsule(attacker->GetCollisionData().projectilePos, attacker->GetCollisionData().projectileCollisionRadius,
					target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius)
					&& attacker->GetCollisionData().isProjectileAttack)
				{
					// 当たった時の処理
					OnProjectileCollision(attacker, target);
				}
			}
		}
	}

}

void CollisionManager::OnProjectileCollision(const std::shared_ptr<ActorBase>& attacker, const std::shared_ptr<ActorBase>& target)
{

	// 飛び道具が当たった処理
	target->ProjectileHit(attacker->GetDamage());

	// 相手の座標を設定
	target->SetTargetPos(attacker->GetTransform()->pos);

	// 攻撃が当たったフラグをtrueにする
	attacker->SetIsHitAttack(true);

	// 敵の飛び道具だけ当たり判定を消す
	if (attacker->GetActorType() == ActorType::BOSS)
	{
		attacker->SetIsProjectileCollision(false);
	}

	// 当たったターゲットの情報を取得
	auto& data = isLongRangeAttackHitData_[target];

	// ターゲットに今攻撃された攻撃状態の無敵時間を設定する
	data = 1.0f;

}

void CollisionManager::CheckStageCollision()
{

	// アクターとステージとの当たり判定
	CheckActorsAndStageCollision();

	// カメラとステージとの当たり判定
	CheckCameraAndStageCollision();

}

void CollisionManager::CheckActorsAndStageCollision()
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

				// ポインタが入っているか確認
				if (!target)return;
				if (!stage)return;

				// ステージに情報を変更
				auto stageData = std::dynamic_pointer_cast<StageBase>(stage);

				// ステージに当たり判定があるかチェック
				if (!stageData->GetIsCollision()) continue;

				// カプセルとの衝突判定
				MV1_COLL_RESULT_POLY_DIM hits = MV1CollCheck_Capsule(
					stage->GetTransform()->modelId, -1,
					target->GetCollisionData().bodyCapsuleUpPos, target->GetCollisionData().bodyCapsuleDownPos, target->GetCollisionData().bodyCollisionRadius);

				// 衝突した複数のポリゴンと衝突回避するまで、
				// プレイヤーの位置を移動させる
				for (int i = 0; i < hits.HitNum; i++)
				{

					auto hit = hits.Dim[i];

					// 地面と異なり、衝突回避位置が不明なため、何度か移動させる
					// この時、移動させる方向は、移動前座標に向いた方向であったり、
					// 衝突したポリゴンの法線方向だったりする
					for (int tryCnt = 0; tryCnt < STAGE_COLLISION_COUNT; tryCnt++)
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
							VECTOR pos = target->GetTransform()->pos;

							// 法線の方向にちょっとだけ移動させる
							pos = VAdd(pos, VScale(hit.Normal, OBJECT_COLLISION_PUSH_FORCE));

							// カプセルも一緒に移動させる
							target->SetPos(pos);

						}
					}
				}

				// 検出した地面ポリゴン情報の後始末
				MV1CollResultPolyDimTerminate(hits);

				// StgaeCollisionにキャスト
				auto stageCollision = std::dynamic_pointer_cast<StageCollision>(stage);

				// StageCollision出なければ処理しない
				if (stageCollision)
				{

					// 地面との衝突
					MV1_COLL_RESULT_POLY hit = MV1CollCheck_Line(
						stage->GetTransform()->modelId, -1,
						VAdd(target->GetCollisionData().bodyCapsuleUpPos, VECTOR(0.0f, target->GetCollisionData().bodyCollisionRadius, 0.0f)),
						VAdd(target->GetCollisionData().bodyCapsuleDownPos, VECTOR(0.0f, -target->GetCollisionData().bodyCollisionRadius, 0.0f)));

					// 地面に当たっている時と下方向に動いている時のみ判定する
					if (hit.HitFlag > 0 && VDot({ 0.0f,-1.0f,0.0f }, target->GetVelocity()) > DOWN_DIR)
					{

						// 衝突地点から、少し上に移動
						target->SetPos(hit.HitPosition);

						// 衝突しているに変更
						target->SetIsCollisionFloor(true);

						// ジャンプリセット
						target->SetVelocity(Utility::VECTOR_ZERO);

					}
					else
					{
						// 衝突していないに変更
						target->SetIsCollisionFloor(false);
					}
				}
			}

			// もし地面を貫通して下に行ってしまったとき
			//if (target->GetTransform()->pos.y < -19500.0f)
			//{
			//	target->SetPos({ target->GetTransform()->pos.x, -19500.0f,target->GetTransform()->pos.z });
			//}

		}
	}

}

void CollisionManager::CheckCameraAndStageCollision()
{

	// ステージ
	const auto& stages = collisionActorData_.find(ActorType::STAGE);

	// 中身が入っているか確認
	if (stages == collisionActorData_.end())return;

	// カメラ座標
	auto cPos = camera_.lock()->GetPos();

	// 注視点
	auto tPos = camera_.lock()->GetTargetPos();

	// カメラから注視点に向けてのベクトル
	auto cDir = VNorm(VSub(tPos, cPos));

	for (const std::shared_ptr<ActorBase>& stage : stages->second)
	{

		// ステージに情報を変更
		auto stageData = std::dynamic_pointer_cast<StageBase>(stage);

		// ステージに当たり判定があるかチェック
		if (!stageData->GetIsCollision()) continue;

		// ステージと衝突しているか判定
		MV1_COLL_RESULT_POLY hit = MV1CollCheck_Line(stage->GetTransform()->modelId, -1, camera_.lock()->GetTargetPos(), camera_.lock()->GetPos());

		if (hit.HitFlag)
		{

			// 当たった地点に座標を設定
			camera_.lock()->SetPos(VAdd(hit.HitPosition, VScale(cDir, 10.0f)));

			// カメラ座標
			auto tmpCPos = camera_.lock()->GetPos();

			// 注視点
			auto tmpTPos = camera_.lock()->GetTargetPos();

			// ワールドの上方向と地面の法線が近しい方向だったらカメラの上昇補正を処理しない
			auto a = VDot({ 0.0f,1.0f,0.0f }, hit.Normal);
			if (a > 0.9f)return;

			// カメラと注視点の距離を計算
			auto disPow2 = Utility::SqrMagnitude(tmpCPos, tmpTPos);

			// 距離
			float len = 5000.0f;
			if (disPow2 < len * len)
			{
				auto l = Utility::Lerp(tmpCPos, VAdd(tmpCPos, { 0.0f,len ,0.0f }), 0.1f);
				camera_.lock()->SetPos(l);
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

				// ポインタが入っているか確認
				if (!actor1)return;
				if (!actor2)return;

				// プレイヤーと敵が重なってたときの処理
				if (actor1->GetActorType() == ActorType::PLAYER)
				{
					ResolvePlayerEnemyCollision(actor1, actor2);
				}
				// プレイヤーと敵が重なってたときの処理
				else if (actor1->GetActorType() == ActorType::ENEMY)
				{
					ResolveEnemysCollision(actor1, actor2);
				}

			}
		}
	}

}

void CollisionManager::ResolvePlayerEnemyCollision(const std::shared_ptr<ActorBase>& actor1, const std::shared_ptr<ActorBase>& actor2)
{

	// カプセル同士の当たり判定
	if (HitCheck_Capsule_Capsule(actor1->GetCollisionData().bodyCapsuleUpPos, actor1->GetCollisionData().bodyCapsuleDownPos, actor1->GetCollisionData().bodyCollisionRadius,
								 actor2->GetCollisionData().bodyCapsuleUpPos, actor2->GetCollisionData().bodyCapsuleDownPos, actor2->GetCollisionData().bodyCollisionRadius))
	{

		// 重なっていたら互いに押し返す
		VECTOR overlap = VSub(actor2->GetCollisionData().bodyPos, actor1->GetCollisionData().bodyPos);

		// 正規化する
		VECTOR normOverlap = VNorm(overlap);

		// 敵側を移動させる
		actor2->AddPos(VScale(normOverlap, ENEMY_PUSH_FORCE));

	}

}

void CollisionManager::ResolveEnemysCollision(const std::shared_ptr<ActorBase>& actor1, const std::shared_ptr<ActorBase>& actor2)
{

	// カプセル同士の当たり判定
	if (HitCheck_Capsule_Capsule(actor1->GetCollisionData().bodyCapsuleUpPos, actor1->GetCollisionData().bodyCapsuleDownPos, actor1->GetCollisionData().bodyCollisionRadius,
								 actor2->GetCollisionData().bodyCapsuleUpPos, actor2->GetCollisionData().bodyCapsuleDownPos, actor2->GetCollisionData().bodyCollisionRadius))
	{

		// 重なっていたら互いに押し返す
		VECTOR overlap = VSub(actor2->GetCollisionData().bodyPos, actor1->GetCollisionData().bodyPos);

		// 正規化する
		VECTOR normOverlap = VNorm(overlap);

		// 高さをなくす
		normOverlap.y = 0.0f;

		// お互いに移動させる
		actor1->AddPos(VScale(normOverlap, -ENEMY_PUSH_FORCE / 2));
		actor2->AddPos(VScale(normOverlap, ENEMY_PUSH_FORCE / 2));

	}

}

void CollisionManager::CheckRestrictedAreasCollision()
{

	// ステージと当たっているか判定したいもの
	const auto& players = collisionActorData_.find(ActorType::PLAYER);

	// ステージ
	const auto& stages = collisionActorData_.find(ActorType::STAGE);

	// 中身が入っているか確認
	if (players == collisionActorData_.end())return;
	if (stages == collisionActorData_.end())return;

	for (const std::shared_ptr<ActorBase>& player : players->second)
	{
		for (const std::shared_ptr<ActorBase>& stage : stages->second)
		{

			// ポインタが入っているか確認
			if (!player)return;
			if (!stage)return;

			// ステージに情報を変更
			auto stageData = std::dynamic_pointer_cast<StageBase>(stage);

			// ステージに当たり判定があるかチェック
			if ((!stageData->GetIsRestrictedArea() || stageData->GetIsCollision())) continue;

			// 衝突しているか判定
			if (HitCheck_Sphere_Sphere(
				player->GetTransform()->pos,
				player->GetCollisionData().bodyCollisionRadius,
				stageData->GetRestrictedAreaPos(),stageData->GetRestrictedAreaRadius()))
			{
				// 制限エリアの当たり判定を付ける
				stageData->SetIsCollision(true);
			}

		}
	}

}