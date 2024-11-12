#include "ActorBase.h"

ActorBase::ActorBase(const VECTOR& pos, const json& data)
	:
	transform_(Transform()),
	RIGHT_HAND_RELATIVE_UP_POS({ 0.0f,100.0f,0.0f }),
	RIGHT_HAND_RELATIVE_DOWN_POS({ 0.0f,-100.0f,0.0f }),
	LEFT_HAND_RELATIVE_UP_POS({ 0.0f,100.0f,0.0f }),
	LEFT_HAND_RELATIVE_DOWN_POS({ 0.0f,-100.0f,0.0f }),
	RIGHT_FOOT_RELATIVE_UP_POS({ 0.0f,100.0f,0.0f }),
	RIGHT_FOOT_RELATIVE_DOWN_POS({ 0.0f,-100.0f,0.0f }),
	LEFT_FOOT_RELATIVE_UP_POS({ 0.0f,100.0f,0.0f }),
	LEFT_FOOT_RELATIVE_DOWN_POS({ 0.0f,-100.0f,0.0f }),
	BODY_RELATIVE_UP_POS({ 0.0f,500.0f,0.0f }),
	BODY_RELATIVE_DOWN_POS({ 0.0f,-500.0f,0.0f }),
	ATTACK_MOVE_POW(data["ATTACK_MOVE_POW"]),
	GRAVITY(9.8f),
	ROTATION_POW(0.2f),
	HAND_AND_FOOT_COLLISION_RADIUS(100.0f),
	BODY_COLLISION_RADIUS(300.0f),
	HP_MAX(data["HP"]),
	resMng_(ResourceManager::GetInstance()),
	dir_({ 0.0f,0.0f,0.0f }),
	moveDir_({ 0.0f,0.0f,0.0f }),
	modelId_(-1),
	damage_(0),
	scl_(data["SCALE"]),
	stepRotTime_(0.0f),
	speed_(0.0f),
	velocity_({ 0.0f,0.0f,0.0f }),
	acceleration_({ 0.0f,-GRAVITY,0.0f }),
	hp_(HP_MAX),
	actorType_(static_cast<ActorType>(data["ACTOR_TYPE"])),
	isAlive_(false),
	isActive_(false),
	isLockOn_(false),
	isAttackHit_(false),
	jsonData_(data)
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

void ActorBase::InitFunction()
{
}

void ActorBase::InitParameter()
{
}

void ActorBase::InitAnimation()
{
}

void ActorBase::InitFunctionPointer()
{
}

void ActorBase::UpdateDebugImGui()
{
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

	// カプセルの上の相対座標
	VECTOR rightHandUpPos = collisionData_.rightHandRot.PosAxis(RIGHT_HAND_RELATIVE_UP_POS);

	// カプセルの下の相対座標
	VECTOR rightHandDownPos = collisionData_.rightHandRot.PosAxis(RIGHT_HAND_RELATIVE_DOWN_POS);

	// 当たり判定で使うカプセルの上の座標を更新
	collisionData_.rightHandCapsuleUpPos = VAdd(collisionData_.rightHandPos, rightHandUpPos);

	// 当たり判定で使うカプセルの下の座標を更新
	collisionData_.rightHandCapsuleDownPos = VAdd(collisionData_.rightHandPos, rightHandDownPos);

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

	// カプセルの上の相対座標
	VECTOR leftHandUpPos = collisionData_.leftHandRot.PosAxis(LEFT_HAND_RELATIVE_UP_POS);

	// カプセルの下の相対座標
	VECTOR leftHandDownPos = collisionData_.leftHandRot.PosAxis(LEFT_HAND_RELATIVE_DOWN_POS);

	// 当たり判定で使うカプセルの上の座標を更新
	collisionData_.leftHandCapsuleUpPos = VAdd(collisionData_.leftHandPos, leftHandUpPos);

	// 当たり判定で使うカプセルの下の座標を更新
	collisionData_.leftHandCapsuleDownPos = VAdd(collisionData_.leftHandPos, leftHandDownPos);

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

	// カプセルの上の相対座標
	VECTOR rightFootUpPos = collisionData_.rightFootRot.PosAxis(RIGHT_FOOT_RELATIVE_UP_POS);

	// カプセルの下の相対座標
	VECTOR rightFootDownPos = collisionData_.rightFootRot.PosAxis(RIGHT_FOOT_RELATIVE_DOWN_POS);

	// 当たり判定で使うカプセルの上の座標を更新
	collisionData_.rightFootCapsuleUpPos = VAdd(collisionData_.rightFootPos, rightFootUpPos);

	// 当たり判定で使うカプセルの下の座標を更新
	collisionData_.rightFootCapsuleDownPos = VAdd(collisionData_.rightFootPos, rightFootDownPos);

#pragma endregion

#pragma region 左足

	// 指定のフレームのローカル座標からワールド座標に変換する行列を得る
	MATRIX matLeftFootPos = MV1GetFrameLocalWorldMatrix(transform_.modelId, collisionData_.leftFoot);

	// 行列の平行移動成分を取得する
	VECTOR leftFootPos = MGetTranslateElem(matLeftFootPos);

	// 行列の回転成分を取得する
	MATRIX matLeftFootRot = MGetRotElem(matLeftFootPos);

	// 行列からクォータニオンに変換
	Quaternion leftFootQua = Quaternion::GetRotation(matLeftFootRot);

	// 当たり判定の座標を更新
	collisionData_.leftFootPos = leftFootPos;

	// 回転を更新
	collisionData_.leftFootRot = leftFootQua;

	// カプセルの上の相対座標
	VECTOR leftFootUpPos = collisionData_.leftFootRot.PosAxis(LEFT_FOOT_RELATIVE_UP_POS);

	// カプセルの下の相対座標
	VECTOR leftFootDownPos = collisionData_.leftFootRot.PosAxis(LEFT_FOOT_RELATIVE_DOWN_POS);

	// 当たり判定で使うカプセルの上の座標を更新
	collisionData_.leftFootCapsuleUpPos = VAdd(collisionData_.leftFootPos, leftFootUpPos);

	// 当たり判定で使うカプセルの下の座標を更新
	collisionData_.leftFootCapsuleDownPos = VAdd(collisionData_.leftFootPos, leftFootDownPos);

#pragma endregion

#pragma region 体

	// 指定のフレームのローカル座標からワールド座標に変換する行列を得る
	MATRIX matBodyPos = MV1GetFrameLocalWorldMatrix(transform_.modelId, collisionData_.body);

	// 行列の平行移動成分を取得する
	VECTOR bodyPos = MGetTranslateElem(matBodyPos);

	// 行列の回転成分を取得する
	MATRIX matBodyRot = MGetRotElem(matBodyPos);

	// 行列からクォータニオンに変換
	Quaternion bodyQua = Quaternion::GetRotation(matBodyRot);

	// 当たり判定の座標を更新
	collisionData_.bodyPos = bodyPos;

	// 回転を更新
	collisionData_.bodyRot = bodyQua;

	// カプセルの上の相対座標
	VECTOR bodyUpPos = collisionData_.bodyRot.PosAxis(BODY_RELATIVE_UP_POS);

	// カプセルの下の相対座標
	VECTOR bodyDownPos = collisionData_.bodyRot.PosAxis(BODY_RELATIVE_DOWN_POS);

	// 当たり判定で使うカプセルの上の座標を更新
	collisionData_.bodyCapsuleUpPos = VAdd(collisionData_.bodyPos, bodyUpPos);

	// 当たり判定で使うカプセルの下の座標を更新
	collisionData_.bodyCapsuleDownPos = VAdd(collisionData_.bodyPos, bodyDownPos);

#pragma endregion

}

void ActorBase::Move()
{
}

void ActorBase::Attack()
{
}

void ActorBase::CollisionRegister()
{
}

void ActorBase::AnimationFrame()
{

}

void ActorBase::Gravity()
{

	// 速度に加速度（重力）を加える
	velocity_ = VAdd(velocity_, acceleration_);

	// 位置に速度を加える
	transform_.pos = VAdd(transform_.pos, velocity_);

}

void ActorBase::Draw()
{
	
	// モデル描画
	MV1DrawModel(transform_.modelId);

	// デバッグ描画
	DrawDebug();

}

void ActorBase::SetIsLockOn(const bool isLockOn)
{
	isLockOn_ = isLockOn;
}

bool ActorBase::GetAttackState()
{
	return false;
}

bool ActorBase::GetHitState()
{
	return false;
}

bool ActorBase::GetIsLockOn()
{
	return isLockOn_;
}

void ActorBase::AttackHit(const int damage, const int state)
{
}

void ActorBase::DrawDebug()
{

	// 右手の当たり判定の描画
	DrawCapsule3D(collisionData_.rightHandCapsuleUpPos, collisionData_.rightHandCapsuleDownPos, HAND_AND_FOOT_COLLISION_RADIUS, 10, 0xff0000, 0xff0000, false);

	// 左手の当たり判定の描画
	DrawCapsule3D(collisionData_.leftHandCapsuleUpPos, collisionData_.leftHandCapsuleDownPos, HAND_AND_FOOT_COLLISION_RADIUS, 10, 0xff0000, 0xff0000, false);

	// 右足の当たり判定の描画
	DrawCapsule3D(collisionData_.rightFootCapsuleUpPos, collisionData_.rightFootCapsuleDownPos, HAND_AND_FOOT_COLLISION_RADIUS, 10, 0xff0000, 0xff0000, false);

	// 左足の当たり判定の描画
	DrawCapsule3D(collisionData_.leftFootCapsuleUpPos, collisionData_.leftFootCapsuleDownPos, HAND_AND_FOOT_COLLISION_RADIUS, 10, 0xff0000, 0xff0000, false);

	// 体の当たり判定の描画
	DrawCapsule3D(collisionData_.bodyCapsuleUpPos, collisionData_.bodyCapsuleDownPos, BODY_COLLISION_RADIUS, 10, 0xff0000, 0xff0000, false);

}
