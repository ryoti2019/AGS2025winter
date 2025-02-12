#include "ActorBase.h"
#include "../Component/Component.h"

ActorBase::ActorBase(const VECTOR& pos, const json& data)
	:
	transform_(nullptr),
	RIGHT_HAND_RELATIVE_UP_POS({ 0.0f,100.0f,0.0f }),
	RIGHT_HAND_RELATIVE_DOWN_POS({ 0.0f,-100.0f,0.0f }),
	LEFT_HAND_RELATIVE_UP_POS({ 0.0f,100.0f,0.0f }),
	LEFT_HAND_RELATIVE_DOWN_POS({ 0.0f,-100.0f,0.0f }),
	RIGHT_FOOT_RELATIVE_UP_POS({ 0.0f,100.0f,0.0f }),
	RIGHT_FOOT_RELATIVE_DOWN_POS({ 0.0f,-100.0f,0.0f }),
	LEFT_FOOT_RELATIVE_UP_POS({ 0.0f,100.0f,0.0f }),
	LEFT_FOOT_RELATIVE_DOWN_POS({ 0.0f,-100.0f,0.0f }),
	BODY_RELATIVE_CENTER_POS({ 0.0f,750.0f,0.0f }),
	BODY_RELATIVE_UP_POS({ 0.0f,750.0f,0.0f }),
	BODY_RELATIVE_DOWN_POS({ 0.0f,-750.0f,0.0f }),
	ATTACK_MOVE_POW(data["ATTACK_MOVE_POW"]),
	GRAVITY(9.8f),
	ROTATION_POW(0.3f),
	HAND_AND_FOOT_COLLISION_RADIUS(1000.0f),
	BODY_COLLISION_RADIUS(300.0f),
	resMng_(ResourceManager::GetInstance()),
	dir_({ 0.0f,0.0f,0.0f }),
	moveDir_({ 0.0f,0.0f,0.0f }),
	modelId_(-1),
	damage_(0),
	scl_(data["SCALE"]),
	INIT_ANGLE(data["INIT_ANGLE"]),
	FLOOR_HEIGHT(-19500.0f),
	stepRotTime_(0.0f),
	speed_(0.0f),
	velocity_({ 0.0f,-1.0f,0.0f }),
	gravityScale_(1.0f),
	acceleration_({ 0.0f,-GRAVITY,0.0f }),
	actorType_(static_cast<ActorType>(data["ACTOR_TYPE"])),
	isAlive_(false),
	isActive_(false),
	isLockOn_(false),
	isOnGround_(false),
	isHitAttack_(false),
	jsonData_(data)
{

	// ���f������̊�{���𐶐�
	transform_ = std::make_shared<Transform>();

}

void ActorBase::Init(const VECTOR& pos)
{

#pragma region �I�u�W�F�N�g�̏��

	transform_->SetModel(modelId_);
	SetPos(pos);
	transform_->scl = { scl_,scl_,scl_ };
	transform_->quaRot = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(INIT_ANGLE),Utility::Deg2RadF(0.0f) });
	transform_->quaRotLocal = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(180.0f),Utility::Deg2RadF(0.0f) });
	transform_->Update();

#pragma endregion

}

void ActorBase::Create(const VECTOR& pos)
{

#pragma region �I�u�W�F�N�g�̏��

	transform_->SetModel(modelId_);
	SetPos(pos);
	transform_->scl = { scl_,scl_,scl_ };
	transform_->quaRot = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(INIT_ANGLE),Utility::Deg2RadF(0.0f) });
	transform_->quaRotLocal = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(180.0f),Utility::Deg2RadF(0.0f) });
	transform_->Update();

#pragma endregion

}

void ActorBase::Update(const float deltaTime)
{
}

void ActorBase::LazyRotation(float goalRot)
{
	auto goal = Quaternion::Euler(0.0f, goalRot, 0.0f);
	transform_->quaRot = Quaternion::Slerp(transform_->quaRot, goal, ROTATION_POW);
}

void ActorBase::InitFunction()
{
}

void ActorBase::InitParameter()
{
}

void ActorBase::InitImage()
{
}

void ActorBase::InitAnimation()
{
}

void ActorBase::InitEffect()
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

#pragma region �E��

	// �w��̃t���[���̃��[�J�����W���烏�[���h���W�ɕϊ�����s��𓾂�
	MATRIX matRightHandPos = MV1GetFrameLocalWorldMatrix(transform_->modelId, collisionData_.rightHand);

	// �s��̕��s�ړ��������擾����
	VECTOR rightHandPos = MGetTranslateElem(matRightHandPos);

	// �s��̉�]�������擾����
	MATRIX matRightHandRot = MGetRotElem(matRightHandPos);

	// �s�񂩂�N�H�[�^�j�I���ɕϊ�
	Quaternion rightHandQua = Quaternion::GetRotation(matRightHandRot);

	// �����蔻��̍��W���X�V
	collisionData_.rightHandPos = rightHandPos;

	// ��]���X�V
	collisionData_.rightHandRot = rightHandQua;

	// �J�v�Z���̏�̑��΍��W
	VECTOR rightHandUpPos = collisionData_.rightHandRot.PosAxis(RIGHT_HAND_RELATIVE_UP_POS);

	// �J�v�Z���̉��̑��΍��W
	VECTOR rightHandDownPos = collisionData_.rightHandRot.PosAxis(RIGHT_HAND_RELATIVE_DOWN_POS);

	// �����蔻��Ŏg���J�v�Z���̏�̍��W���X�V
	collisionData_.rightHandCapsuleUpPos = VAdd(collisionData_.rightHandPos, rightHandUpPos);

	// �����蔻��Ŏg���J�v�Z���̉��̍��W���X�V
	collisionData_.rightHandCapsuleDownPos = VAdd(collisionData_.rightHandPos, rightHandDownPos);

#pragma endregion

#pragma region ����

	// �w��̃t���[���̃��[�J�����W���烏�[���h���W�ɕϊ�����s��𓾂�
	MATRIX matLeftHandPos = MV1GetFrameLocalWorldMatrix(transform_->modelId, collisionData_.leftHand);

	// �s��̕��s�ړ��������擾����
	VECTOR leftHandPos = MGetTranslateElem(matLeftHandPos);

	// �s��̉�]�������擾����
	MATRIX matLeftHandRot = MGetRotElem(matLeftHandPos);

	// �s�񂩂�N�H�[�^�j�I���ɕϊ�
	Quaternion leftHandQua = Quaternion::GetRotation(matLeftHandRot);

	// �����蔻��̍��W���X�V
	collisionData_.leftHandPos = leftHandPos;

	// ��]���X�V
	collisionData_.leftHandRot = leftHandQua;

	// �J�v�Z���̏�̑��΍��W
	VECTOR leftHandUpPos = collisionData_.leftHandRot.PosAxis(LEFT_HAND_RELATIVE_UP_POS);

	// �J�v�Z���̉��̑��΍��W
	VECTOR leftHandDownPos = collisionData_.leftHandRot.PosAxis(LEFT_HAND_RELATIVE_DOWN_POS);

	// �����蔻��Ŏg���J�v�Z���̏�̍��W���X�V
	collisionData_.leftHandCapsuleUpPos = VAdd(collisionData_.leftHandPos, leftHandUpPos);

	// �����蔻��Ŏg���J�v�Z���̉��̍��W���X�V
	collisionData_.leftHandCapsuleDownPos = VAdd(collisionData_.leftHandPos, leftHandDownPos);

#pragma endregion

#pragma region �E��

	// �w��̃t���[���̃��[�J�����W���烏�[���h���W�ɕϊ�����s��𓾂�
	MATRIX matRightFootPos = MV1GetFrameLocalWorldMatrix(transform_->modelId, collisionData_.rightFoot);

	// �s��̕��s�ړ��������擾����
	VECTOR rightFootPos = MGetTranslateElem(matRightFootPos);

	// �s��̉�]�������擾����
	MATRIX matRightFootRot = MGetRotElem(matRightFootPos);

	// �s�񂩂�N�H�[�^�j�I���ɕϊ�
	Quaternion rightFootQua = Quaternion::GetRotation(matRightFootRot);

	// �����蔻��̍��W���X�V
	collisionData_.rightFootPos = rightFootPos;

	// ��]���X�V
	collisionData_.rightFootRot = rightFootQua;

	// �J�v�Z���̏�̑��΍��W
	VECTOR rightFootUpPos = collisionData_.rightFootRot.PosAxis(RIGHT_FOOT_RELATIVE_UP_POS);

	// �J�v�Z���̉��̑��΍��W
	VECTOR rightFootDownPos = collisionData_.rightFootRot.PosAxis(RIGHT_FOOT_RELATIVE_DOWN_POS);

	// �����蔻��Ŏg���J�v�Z���̏�̍��W���X�V
	collisionData_.rightFootCapsuleUpPos = VAdd(collisionData_.rightFootPos, rightFootUpPos);

	// �����蔻��Ŏg���J�v�Z���̉��̍��W���X�V
	collisionData_.rightFootCapsuleDownPos = VAdd(collisionData_.rightFootPos, rightFootDownPos);

#pragma endregion

#pragma region ����

	// �w��̃t���[���̃��[�J�����W���烏�[���h���W�ɕϊ�����s��𓾂�
	MATRIX matLeftFootPos = MV1GetFrameLocalWorldMatrix(transform_->modelId, collisionData_.leftFoot);

	// �s��̕��s�ړ��������擾����
	VECTOR leftFootPos = MGetTranslateElem(matLeftFootPos);

	// �s��̉�]�������擾����
	MATRIX matLeftFootRot = MGetRotElem(matLeftFootPos);

	// �s�񂩂�N�H�[�^�j�I���ɕϊ�
	Quaternion leftFootQua = Quaternion::GetRotation(matLeftFootRot);

	// �����蔻��̍��W���X�V
	collisionData_.leftFootPos = leftFootPos;

	// ��]���X�V
	collisionData_.leftFootRot = leftFootQua;

	// �J�v�Z���̏�̑��΍��W
	VECTOR leftFootUpPos = collisionData_.leftFootRot.PosAxis(LEFT_FOOT_RELATIVE_UP_POS);

	// �J�v�Z���̉��̑��΍��W
	VECTOR leftFootDownPos = collisionData_.leftFootRot.PosAxis(LEFT_FOOT_RELATIVE_DOWN_POS);

	// �����蔻��Ŏg���J�v�Z���̏�̍��W���X�V
	collisionData_.leftFootCapsuleUpPos = VAdd(collisionData_.leftFootPos, leftFootUpPos);

	// �����蔻��Ŏg���J�v�Z���̉��̍��W���X�V
	collisionData_.leftFootCapsuleDownPos = VAdd(collisionData_.leftFootPos, leftFootDownPos);

#pragma endregion

#pragma region ��

	// �����蔻��̍��W���X�V
	collisionData_.bodyPos = VAdd(transform_->pos, BODY_RELATIVE_CENTER_POS);

	// �J�v�Z���̏�̑��΍��W
	VECTOR bodyUpPos = BODY_RELATIVE_UP_POS;

	// �J�v�Z���̉��̑��΍��W
	VECTOR bodyDownPos = BODY_RELATIVE_DOWN_POS;

	// �����蔻��Ŏg���J�v�Z���̏�̍��W���X�V
	collisionData_.bodyCapsuleUpPos = VAdd(collisionData_.bodyPos, bodyUpPos);

	// �����蔻��Ŏg���J�v�Z���̉��̍��W���X�V
	collisionData_.bodyCapsuleDownPos = VAdd(collisionData_.bodyPos, bodyDownPos);

#pragma endregion

}

void ActorBase::Move()
{
}

void ActorBase::Attack(const float deltaTime)
{
}

void ActorBase::CollisionRegister()
{
}

void ActorBase::AnimationFrame()
{

}

void ActorBase::AttackHitCheck(const int state)
{
}

void ActorBase::Gravity(const float scale)
{

	// �d�͂̋�����ύX����
	const VECTOR acceleration = { acceleration_.x, acceleration_.y / scale, acceleration_.z };

	// ���x�ɉ����x�i�d�́j��������
	velocity_ = VAdd(velocity_, acceleration);

	// ���W���X�V
	transform_->pos = VAdd(transform_->pos, velocity_);

}

void ActorBase::DeathAnim(int state)
{
}

void ActorBase::InitBGMAndSE()
{
}

void ActorBase::Draw(const float deltaTime)
{
	// �`��̍X�V
	drawComponent_->Update();
}

void ActorBase::AddPos(const VECTOR& pos)
{
	transform_->pos = VAdd(transform_->pos, pos);
}

void ActorBase::SetIsLockOn(const bool isLockOn)
{
	isLockOn_ = isLockOn;
}

const bool ActorBase::GetAttackState()const
{
	return false;
}

const bool ActorBase::GetCloseRangeAttackState() const
{
	return false;
}

const bool ActorBase::GetHitState()const
{
	return false;
}

const bool ActorBase::GetIsSuperArmor() const
{
	return false;
}

const bool ActorBase::GetIsLockOn()const
{
	return isLockOn_;
}

void ActorBase::AttackHit(const int damage, const int state)
{
}

void ActorBase::ProjectileHit(const int damage)
{
}
