#include "ActorBase.h"

ActorBase::ActorBase(const VECTOR& pos)
	:
	resMng_(ResourceManager::GetInstance()),
	transform_(Transform()),
	ROTATION_POW(0.1f),
	COLLISION_RADIUS(100.0f),
	speed_(0.0f),
	hp_(0.0f),
	actorType_(ActorType::NONE),
	isAlive_(false),
	isActive_(false)
{

}

void ActorBase::Init(const VECTOR& pos)
{

#pragma region オブジェクトの情報

	transform_.SetModel(modelId_);
	SetPos(pos);
	transform_.scl = { scl_,scl_,scl_ };
	transform_.quaRot = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(0.0f),Utility::Deg2RadF(0.0f) });
	transform_.quaRotLocal = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(180.0f),Utility::Deg2RadF(0.0f) });
	transform_.Update();

	speed_ = 0.0f;
	hp_ = 0;
	actorType_ = ActorType::NONE;
	isAlive_ = true;
	isActive_ = false;

#pragma endregion

}

void ActorBase::Create(const VECTOR& pos)
{
}

void ActorBase::Update(const float deltaTime)
{
}

void ActorBase::LazyRotation(float goalRot)
{
	auto goal = Quaternion::Euler(0.0f, goalRot, 0.0f);
	transform_.quaRot = Quaternion::Slerp(transform_.quaRot, goal, ROTATION_POW);
}

bool ActorBase::AttackState()
{
	return false;
}

void ActorBase::CollisionUpdate()
{

#pragma region 右手

	// 指定のフレームのローカル座標からワールド座標に変換する行列を得る
	MATRIX matRightHandPos = MV1GetFrameLocalWorldMatrix(transform_.modelId, collisionData_.rightHand);

	// 行列の平行移動成分を取得する
	VECTOR rightHandPos = MGetTranslateElem(matRightHandPos);

	// 行列の回転成分を取得する
	MATRIX matRightHandRot = MGetRotElem(matRightHandPos);

	// 行列からクォータニオンに変換
	Quaternion rightHandQua = Quaternion::GetRotation(matRightHandRot);

	// 当たり判定の座標を更新
	collisionData_.rightHandPos = rightHandPos;

	// 回転を更新
	collisionData_.rightHandRot = rightHandQua;

	// 追従対象からの相対座標
	VECTOR rightHandRotPos = collisionData_.rightHandRot.PosAxis({ 0.0f,0.0f,0.0f });

	// 回転を加えた当たり判定の座標を更新
	collisionData_.rightHandPos = VAdd(collisionData_.rightHandPos, rightHandRotPos);

#pragma endregion

#pragma region 左手

	// 指定のフレームのローカル座標からワールド座標に変換する行列を得る
	MATRIX matLeftHandPos = MV1GetFrameLocalWorldMatrix(transform_.modelId, collisionData_.leftHand);

	// 行列の平行移動成分を取得する
	VECTOR leftHandPos = MGetTranslateElem(matLeftHandPos);

	// 行列の回転成分を取得する
	MATRIX matLeftHandRot = MGetRotElem(matLeftHandPos);

	// 行列からクォータニオンに変換
	Quaternion leftHandQua = Quaternion::GetRotation(matLeftHandRot);

	// 当たり判定の座標を更新
	collisionData_.leftHandPos = leftHandPos;

	// 回転を更新
	collisionData_.leftHandRot = leftHandQua;

	// 追従対象からの相対座標
	VECTOR leftHandRotPos = collisionData_.leftHandRot.PosAxis({ 0.0f,0.0f,0.0f });

	// 回転を加えた当たり判定の座標を更新
	collisionData_.leftHandPos = VAdd(collisionData_.leftHandPos, leftHandRotPos);

#pragma endregion

#pragma region 右足

	// 指定のフレームのローカル座標からワールド座標に変換する行列を得る
	MATRIX matRightFootPos = MV1GetFrameLocalWorldMatrix(transform_.modelId, collisionData_.rightFoot);

	// 行列の平行移動成分を取得する
	VECTOR rightFootPos = MGetTranslateElem(matRightFootPos);

	// 行列の回転成分を取得する
	MATRIX matRightFootRot = MGetRotElem(matRightFootPos);

	// 行列からクォータニオンに変換
	Quaternion rightFootQua = Quaternion::GetRotation(matRightFootRot);

	// 当たり判定の座標を更新
	collisionData_.rightFootPos = rightFootPos;

	// 回転を更新
	collisionData_.rightFootRot = rightFootQua;

	// 追従対象からの相対座標
	VECTOR rightFootRotPos = collisionData_.rightFootRot.PosAxis({ 0.0f,0.0f,0.0f });

	// 回転を加えた当たり判定の座標を更新
	collisionData_.rightFootPos = VAdd(collisionData_.rightFootPos, rightFootRotPos);

#pragma endregion

#pragma region 左足

	// 指定のフレームのローカル座標からワールド座標に変換する行列を得る
	MATRIX matLeftFootPos = MV1GetFrameLocalWorldMatrix(transform_.modelId, collisionData_.leftFoot);

	// 行列の平行移動成分を取得する
	VECTOR leftFootpos = MGetTranslateElem(matLeftFootPos);

	// 行列の回転成分を取得する
	MATRIX matLeftFootRot = MGetRotElem(matLeftFootPos);

	// 行列からクォータニオンに変換
	Quaternion leftFootQua = Quaternion::GetRotation(matLeftFootRot);

	// 当たり判定の座標を更新
	collisionData_.leftFootPos = leftFootpos;

	// 回転を更新
	collisionData_.leftFootRot = leftFootQua;

	// 追従対象からの相対座標
	VECTOR leftFootRotPos = collisionData_.leftFootRot.PosAxis({ 0.0f,0.0f,0.0f });

	// 回転を加えた当たり判定の座標を更新
	collisionData_.leftFootPos = VAdd(collisionData_.leftFootPos, leftFootRotPos);

#pragma endregion

}

void ActorBase::CollisionRegister()
{
}

void ActorBase::Draw()
{
	MV1DrawModel(transform_.modelId);

	DrawDebug();

}

void ActorBase::DrawDebug()
{

	// 右手の当たり判定の描画
	DrawSphere3D(collisionData_.rightHandPos, COLLISION_RADIUS, 10, 0xff0000, 0xff0000, true);

	// 左手の当たり判定の描画
	DrawSphere3D(collisionData_.leftHandPos, COLLISION_RADIUS, 10, 0xff0000, 0xff0000, true);

	// 右足の当たり判定の描画
	DrawSphere3D(collisionData_.rightFootPos, COLLISION_RADIUS, 10, 0xff0000, 0xff0000, true);

	// 左足の当たり判定の描画
	DrawSphere3D(collisionData_.leftFootPos, COLLISION_RADIUS, 10, 0xff0000, 0xff0000, true);

}
