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

#pragma region �I�u�W�F�N�g�̏��

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

	// �Ǐ]�Ώۂ���̑��΍��W
	VECTOR rightHandRotPos = collisionData_.rightHandRot.PosAxis({ 0.0f,0.0f,0.0f });

	// ��]�������������蔻��̍��W���X�V
	collisionData_.rightHandPos = VAdd(collisionData_.rightHandPos, rightHandRotPos);

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
	DrawSphere3D(collisionData_.rightHandPos, COLLISION_RADIUS, 10, 0xff0000, 0xff0000, true);

	// ����̓����蔻��̕`��
	DrawSphere3D(collisionData_.leftHandPos, COLLISION_RADIUS, 10, 0xff0000, 0xff0000, true);

	// �E���̓����蔻��̕`��
	DrawSphere3D(collisionData_.rightFootPos, COLLISION_RADIUS, 10, 0xff0000, 0xff0000, true);

	// �����̓����蔻��̕`��
	DrawSphere3D(collisionData_.leftFootPos, COLLISION_RADIUS, 10, 0xff0000, 0xff0000, true);

}
