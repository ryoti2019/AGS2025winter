#include "ActorBase.h"

ActorBase::ActorBase(const VECTOR& pos, const json& data)
	:
	transform_(Transform()),
	RIGHT_HAND_RELATIVE_UP_POS({0.0f,100.0f,0.0f}),
	RIGHT_HAND_RELATIVE_DOWN_POS({0.0f,-100.0f,0.0f}),
	LEFT_HAND_RELATIVE_UP_POS({ 0.0f,100.0f,0.0f }),
	LEFT_HAND_RELATIVE_DOWN_POS({ 0.0f,-100.0f,0.0f }),
	RIGHT_FOOT_RELATIVE_UP_POS({ 0.0f,100.0f,0.0f }),
	RIGHT_FOOT_RELATIVE_DOWN_POS({ 0.0f,-100.0f,0.0f }),
	LEFT_FOOT_RELATIVE_UP_POS({ 0.0f,100.0f,0.0f }),
	LEFT_FOOT_RELATIVE_DOWN_POS({ 0.0f,-100.0f,0.0f }),
	ATTACK_MOVE_POW(data["ATTACK_MOVE_POW"]),
	ROTATION_POW(0.1f),
	COLLISION_RADIUS(100.0f),
	resMng_(ResourceManager::GetInstance()),
	dir_({0.0f,0.0f,0.0f}),
	modelId_(-1),
	scl_(data["SCALE"]),
	stepRotTime_(0.0f),
	speed_(0.0f),
	hp_(data["HP"]),
	actorType_(static_cast<ActorType>(data["ACTOR_TYPE"])),
	isAlive_(false),
	isActive_(false),
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

bool ActorBase::AttackState()
{
	return false;
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

	// �Ǐ]�Ώۂ���̑��΍��W
	VECTOR leftHandRotPos = collisionData_.leftHandRot.PosAxis({ 0.0f,0.0f,0.0f });

	// ��]�������������蔻��̍��W���X�V
	collisionData_.leftHandPos = VAdd(collisionData_.leftHandPos, leftHandRotPos);

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

	// �Ǐ]�Ώۂ���̑��΍��W
	VECTOR rightFootRotPos = collisionData_.rightFootRot.PosAxis({ 0.0f,0.0f,0.0f });

	// ��]�������������蔻��̍��W���X�V
	collisionData_.rightFootPos = VAdd(collisionData_.rightFootPos, rightFootRotPos);

#pragma endregion

#pragma region ����

	// �w��̃t���[���̃��[�J�����W���烏�[���h���W�ɕϊ�����s��𓾂�
	MATRIX matLeftFootPos = MV1GetFrameLocalWorldMatrix(transform_.modelId, collisionData_.leftFoot);

	// �s��̕��s�ړ��������擾����
	VECTOR leftFootpos = MGetTranslateElem(matLeftFootPos);

	// �s��̉�]�������擾����
	MATRIX matLeftFootRot = MGetRotElem(matLeftFootPos);

	// �s�񂩂�N�H�[�^�j�I���ɕϊ�
	Quaternion leftFootQua = Quaternion::GetRotation(matLeftFootRot);

	// �����蔻��̍��W���X�V
	collisionData_.leftFootPos = leftFootpos;

	// ��]���X�V
	collisionData_.leftFootRot = leftFootQua;

	// �Ǐ]�Ώۂ���̑��΍��W
	VECTOR leftFootRotPos = collisionData_.leftFootRot.PosAxis({ 0.0f,0.0f,0.0f });

	// ��]�������������蔻��̍��W���X�V
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

	// �E��̓����蔻��̕`��
	DrawCapsule3D(collisionData_.rightHandCapsuleUpPos, collisionData_.rightHandCapsuleDownPos, COLLISION_RADIUS, 10, 0xff0000, 0xff0000, true);

	// ����̓����蔻��̕`��
	DrawCapsule3D(collisionData_.leftHandCapsuleUpPos, collisionData_.leftHandCapsuleDownPos, COLLISION_RADIUS, 10, 0xff0000, 0xff0000, true);

	// �E���̓����蔻��̕`��
	DrawCapsule3D(collisionData_.rightFootCapsuleUpPos, collisionData_.rightFootCapsuleDownPos, COLLISION_RADIUS, 10, 0xff0000, 0xff0000, true);

	// �����̓����蔻��̕`��
	DrawCapsule3D(collisionData_.leftFootCapsuleUpPos, collisionData_.leftFootCapsuleDownPos, COLLISION_RADIUS, 10, 0xff0000, 0xff0000, true);

}
