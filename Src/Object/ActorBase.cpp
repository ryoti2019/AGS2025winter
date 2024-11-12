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

#pragma region �I�u�W�F�N�g�̏��

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

#pragma region �E��

	// �w��̃t���[���̃��[�J�����W���烏�[���h���W�ɕϊ�����s��𓾂�
	MATRIX matRightHandPos = MV1GetFrameLocalWorldMatrix(transform_.modelId, collisionData_.rightHand);

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
	MATRIX matLeftHandPos = MV1GetFrameLocalWorldMatrix(transform_.modelId, collisionData_.leftHand);

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
	MATRIX matRightFootPos = MV1GetFrameLocalWorldMatrix(transform_.modelId, collisionData_.rightFoot);

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
	MATRIX matLeftFootPos = MV1GetFrameLocalWorldMatrix(transform_.modelId, collisionData_.leftFoot);

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

	// �w��̃t���[���̃��[�J�����W���烏�[���h���W�ɕϊ�����s��𓾂�
	MATRIX matBodyPos = MV1GetFrameLocalWorldMatrix(transform_.modelId, collisionData_.body);

	// �s��̕��s�ړ��������擾����
	VECTOR bodyPos = MGetTranslateElem(matBodyPos);

	// �s��̉�]�������擾����
	MATRIX matBodyRot = MGetRotElem(matBodyPos);

	// �s�񂩂�N�H�[�^�j�I���ɕϊ�
	Quaternion bodyQua = Quaternion::GetRotation(matBodyRot);

	// �����蔻��̍��W���X�V
	collisionData_.bodyPos = bodyPos;

	// ��]���X�V
	collisionData_.bodyRot = bodyQua;

	// �J�v�Z���̏�̑��΍��W
	VECTOR bodyUpPos = collisionData_.bodyRot.PosAxis(BODY_RELATIVE_UP_POS);

	// �J�v�Z���̉��̑��΍��W
	VECTOR bodyDownPos = collisionData_.bodyRot.PosAxis(BODY_RELATIVE_DOWN_POS);

	// �����蔻��Ŏg���J�v�Z���̏�̍��W���X�V
	collisionData_.bodyCapsuleUpPos = VAdd(collisionData_.bodyPos, bodyUpPos);

	// �����蔻��Ŏg���J�v�Z���̉��̍��W���X�V
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

	// ���x�ɉ����x�i�d�́j��������
	velocity_ = VAdd(velocity_, acceleration_);

	// �ʒu�ɑ��x��������
	transform_.pos = VAdd(transform_.pos, velocity_);

}

void ActorBase::Draw()
{
	
	// ���f���`��
	MV1DrawModel(transform_.modelId);

	// �f�o�b�O�`��
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

	// �E��̓����蔻��̕`��
	DrawCapsule3D(collisionData_.rightHandCapsuleUpPos, collisionData_.rightHandCapsuleDownPos, HAND_AND_FOOT_COLLISION_RADIUS, 10, 0xff0000, 0xff0000, false);

	// ����̓����蔻��̕`��
	DrawCapsule3D(collisionData_.leftHandCapsuleUpPos, collisionData_.leftHandCapsuleDownPos, HAND_AND_FOOT_COLLISION_RADIUS, 10, 0xff0000, 0xff0000, false);

	// �E���̓����蔻��̕`��
	DrawCapsule3D(collisionData_.rightFootCapsuleUpPos, collisionData_.rightFootCapsuleDownPos, HAND_AND_FOOT_COLLISION_RADIUS, 10, 0xff0000, 0xff0000, false);

	// �����̓����蔻��̕`��
	DrawCapsule3D(collisionData_.leftFootCapsuleUpPos, collisionData_.leftFootCapsuleDownPos, HAND_AND_FOOT_COLLISION_RADIUS, 10, 0xff0000, 0xff0000, false);

	// �̂̓����蔻��̕`��
	DrawCapsule3D(collisionData_.bodyCapsuleUpPos, collisionData_.bodyCapsuleDownPos, BODY_COLLISION_RADIUS, 10, 0xff0000, 0xff0000, false);

}
