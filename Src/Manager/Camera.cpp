#include <EffekseerForDXLib.h>
#include "../Utility/Utility.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "SceneManager.h" 
#include "../Application.h"
#include "../Object/Common/Transform.h"
#include "Camera.h"

Camera::Camera(void)
{
	mode_ = MODE::FREE;
	pos_ = { 0.0f, 0.0f, 0.0f };
	targetPos_ = { 0.0f, 0.0f, 0.0f };
	angle_ = { 0.0f, 0.0f, 0.0f };

	// �J�����̏����ݒ�
	SetDefault();

}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{

	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;
	case Camera::MODE::LOCKON:
		SetBeforeDrawLockOn();
		break;
	}

	// �J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);

	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
	Effekseer_Sync3DSetting();

}

void Camera::SetBeforeDrawFixedPoint(void)
{
	pos_ = { 0.0f,100.0f,-5000.0f };
}

void Camera::SetBeforeDrawFree(void)
{

	auto& ins = InputManager::GetInstance();

#pragma region ��]

	VECTOR axisDeg = Utility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_UP)) { axisDeg.x += -1.0f; }
	if (ins.IsNew(KEY_INPUT_DOWN)) { axisDeg.x += 1.0f; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { axisDeg.y += -1.0f; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { axisDeg.y += 1.0f; }


	if (!Utility::EqualsVZero(axisDeg))
	{
		// �J��������]������
		angle_.x += Utility::Deg2RadF(axisDeg.x);
		angle_.y += Utility::Deg2RadF(axisDeg.y);
	}


	rotY_ = Quaternion::AngleAxis(angle_.y, Utility::AXIS_Y);
	rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, Utility::AXIS_X));

	// �����_(�ʏ�d�͂ł����Ƃ����Y�l��Ǐ]�ΏۂƓ����ɂ���)
	VECTOR localPos = rotXY_.PosAxis(LOCAL_P2T_POS);
	targetPos_ = VAdd(pos_, localPos);

	//// �J�����ʒu
	//localPos = rotXY_.PosAxis(LOCAL_P2C_POS);
	//pos_ = VAdd(playerTransform_->pos, localPos);

	// �J�����̏����
	cameraUp_ = rotXY_.GetUp();

#pragma endregion

#pragma region �ړ�

	// �ړ�
	//--------------------------------------
	VECTOR moveDir = Utility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_Y)) { moveDir = Utility::DIR_F; }
	if (ins.IsNew(KEY_INPUT_G)) { moveDir = Utility::DIR_L; }
	if (ins.IsNew(KEY_INPUT_H)) { moveDir = Utility::DIR_B; }
	if (ins.IsNew(KEY_INPUT_J)) { moveDir = Utility::DIR_R; }
	if (ins.IsNew(KEY_INPUT_T)) { moveDir = Utility::DIR_U; }
	if (ins.IsNew(KEY_INPUT_U)) { moveDir = Utility::DIR_D; }
	//---------------------------------------

	if (!Utility::EqualsVZero(moveDir))
	{

		// �������ꂽ�ړ��{�^���̕����Ɉړ�
		VECTOR direction = VNorm(rotY_.PosAxis(moveDir)); // ��]������K�v������

		// �ړ���(���� * �X�s�[�h)
		VECTOR movePow = VScale(direction, SPEED);

		// �ړ�(���W + �ړ���)
		// �J�����ʒu�ƃJ���������_
		pos_ = VAdd(pos_, movePow);
		targetPos_ = VAdd(targetPos_, movePow);

	}

#pragma endregion

}

void Camera::SetBeforeDrawFollow(void)
{

	auto& ins = InputManager::GetInstance();

	auto& sce = SceneManager::GetInstance();

	VECTOR playerPos = playerTransform_->pos;

	// Q�L�[����������true�ɂȂ�
	if (isLazy_)
	{
		LazyRotation();
	}

	// �X�e�[�W�̏Փ˔���
	CollisionStage();

}

void Camera::SetBeforeDrawLockOn(void)
{

	// ������̈ʒu
	VECTOR playerPos = playerTransform_->pos;

	// �J�����̑��΍��W
	VECTOR localRotPos;

	// +�����_�����b�N�I���ΏۂƂ���
	auto goalPos = VAdd(enemyTransform_->pos, { 0.0f,200.0f,0.0f });

	// �G�ƃv���C���[�̋������Ƃ�
	float dis = Utility::Distance(goalPos, playerPos);
	
	// �����_�̍��W��ڕW�̍��W�ɋ߂Â���(�������ڕW�̍��W�ɋ߂Â���)
	targetPos_ = Utility::Lerp(targetPos_, goalPos, 0.1f);

	// +�L�����N�^�[���猩�������_�̕���
	auto followToTargetDir = VSub(targetPos_, playerPos);
	followToTargetDir.y = 0.0f;

	// ���̕����Ɍ������]�����
	lockOnLook_ = Quaternion::LookRotation(VNorm(followToTargetDir));

	// �J�����ʒu
	lockOnLook_ = lockOnLook_.Mult(Quaternion::AngleAxis(lockOnAngles_.y, Utility::AXIS_Y));
	lockOnLook_ = lockOnLook_.Mult(Quaternion::AngleAxis(-angle_.x + lockOnAngles_.x, Utility::AXIS_X));
	localRotPos = lockOnLook_.PosAxis(LOCAL_LOCK_ON_F2C_POS);
	auto goalCameraPos = VAdd(playerPos, localRotPos);
	dis = Utility::Distance(goalCameraPos, targetPos_);

	// �����_�ƃJ�����̍Œ዗��
	float min = 500.0f;
	isNearLockOnTarget_ = false;
	if (dis < min)
	{
		float minmin = 400.0f;
		if (dis < minmin)
		{
			isNearLockOnTarget_ = true;
		}

		// �����_�̋������߂�����ꍇ�A���̋�����ۂ�
		auto dir = lockOnLook_.GetBack();
		goalCameraPos = VAdd(targetPos_, VScale(dir, min));

		// �ǂ����邩
		goalCameraPos_ = goalCameraPos;
		pos_ = Utility::Lerp(pos_, goalCameraPos, 0.05f);

	}
	else
	{
		goalCameraPos_ = goalCameraPos;
		pos_ = Utility::Lerp(pos_, goalCameraPos, 0.1f);
	}

	rotXY_ = Quaternion::LookRotation(VSub(goalPos, goalCameraPos_));

	// �N�H�[�^�j�I������I�C���[�p�ɒ���(Y��������)
	auto targetPosXZ = goalPos;
	targetPosXZ.y = 0.0f;
	auto posXZ = goalCameraPos_;
	posXZ.y = 0.0f;
	auto cameraDir = VSub(targetPosXZ, posXZ);
	rotY_ = Quaternion::LookRotation(cameraDir);

	angle_ = rotY_.ToEuler();

	// �J�����̏����
	cameraUp_ = { 0.0f,1.0f,0.0f };

	// �X�e�[�W�̏Փ˔���
	CollisionStage();

}


void Camera::Draw(void)
{
}

void Camera::Release(void)
{
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

VECTOR Camera::GetAngles(void) const
{
	return angle_;
}

void Camera::SetLazyAngles(const VECTOR angles)
{

	lazyGoalRotY_ = Quaternion::Euler(0.0f, angles.y, 0.0f);
	isLazy_ = true;

}

VECTOR Camera::GetTargetPos(void) const
{
	return targetPos_;
}

void Camera::SetPlayer(const Transform* follow)
{
	playerTransform_ = follow;
}

void Camera::SetEnemy(const Transform* follow)
{
	enemyTransform_ = follow;
}

void Camera::ChangeLockOnFlag(void)
{

	// ���b�N�I������
	if(!lockOn_ && mode_ == Camera::MODE::FOLLOW)
	{
		ChangeMode(Camera::MODE::LOCKON);
		lockOn_ = true;
		return;
	}
	
	// ���b�N�I������������
	if (lockOn_ && mode_ == Camera::MODE::LOCKON)
	{
		ChangeMode(Camera::MODE::FOLLOW);
		lockOn_ = false;
		return;
	}

}

void Camera::ChangeMode(MODE mode)
{

	// �J�������[�h�̕ύX
	mode_ = mode;

	// �ύX���̏���������
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		// �J�����̏����ݒ�
		SetDefault();
		pos_ = { 0.0f,200.0f,-500.0f };
		targetPos_ = { 0.0f,150.0f,0.0f };
		break;
	case Camera::MODE::FREE:
		break;
	case Camera::MODE::FOLLOW:
		angle_ = { 0.0f,0.0f,0.0f };
		lockOnAngles_ = { 0.0f, 0.0f, 0.0f };
		break;
	case Camera::MODE::LOCKON:
		angle_ = { 0.0f,0.0f,0.0f };
		lockOnAngles_ = { 0.0f, 0.0f, 0.0f };
		break;
	}

}

Quaternion Camera::GetRotY(void) const
{
	return rotY_;
}

bool Camera::GetLockOn(void)
{
	return lockOn_;
}

Camera::MODE Camera::GetMode(void)
{
	return mode_;
}

void Camera::AddLockOnAnglesY(float rad)
{
	lockOnAngles_.y += rad;
}

void Camera::SetStageID(const int modelId)
{
	stageId_ = modelId;
}

void Camera::SetDefault(void)
{

	// �J�����̏����ݒ�
	pos_ = DEFAULT_CAMERA_POS;

	// �����_
	targetPos_ = VAdd(pos_, LOCAL_P2T_POS);

	// �J�����̏����
	cameraUp_ = { 0.0f, 1.0f, 0.0f };

	// �J������X���ɌX���Ă��邪�A
	// ���̌X������Ԃ��p�x�[���A�X�������Ƃ���
	rotY_ = Quaternion::Identity();
	rotXY_ = Quaternion::Identity();

	// ���b�N�I��
	lockOn_ = false;

}

void Camera::SetTargetPosFollowForward(void)
{

	// �J�����ʒu(�v���C���[����̑��΍��W�Ő���)
	VECTOR localCameraPos = { 0.0f, HEIGHT, -DIS_FOLLOW2CAMERA };
	VECTOR localTargetPos = { 0.0f, 0.0f, DIS_FOLLOW2TARGET };

	// �����_�̈ړ�
	VECTOR followPos = { 0.0f,0.0f,0.0f };

	// �J�����̉�]�s����쐬
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotX(rotXY_.x));
	mat = MMult(mat, MGetRotY(rotXY_.y));

	VECTOR localRotPos;

	// �����_�̈ړ�
	localRotPos = VTransform(localTargetPos, mat);
	targetPos_ = VAdd(followPos, localRotPos);

	// �J�����̈ړ�
	localRotPos = VTransform(localCameraPos, mat);
	movedPos_ = VAdd(followPos, localRotPos);

}

void Camera::KeybordContoroller(void)
{

	auto& ins = InputManager::GetInstance();

	// �}�E�X�J�[�\�����\���ɂ���
	SetMouseDispFlag(false);

	// ��]
	//-------------------------------------
	VECTOR axisDeg = Utility::VECTOR_ZERO;

	// �}�E�X��]��
	float rotPow = 4.0f;
	//float rotPow = 0.0f;

	// �}�E�X�ʒu
	Vector2 mousePos;

	// ��ʂ̒��S�ʒu
	Vector2 center = { Application::SCREEN_SIZE_X / 2,Application::SCREEN_SIZE_Y / 2 };

	// �}�E�X�ʒu�̎擾
	GetMousePoint(&mousePos.x, &mousePos.y);

	// �J������]�̌v�Z(�}�E�X�J�[�\���ʒu�Ɖ�ʂ̒��S�̍������v�Z���A��]��/FPS����Z����)
	// ���ꂪ��]��
	rotPowY_ = float(std::clamp(mousePos.x - center.x, -120, 120)) * rotPow / GetFPS();	// �}�E�X���ړ�
	rotPowX_ = float(std::clamp(mousePos.y - center.y, -120, 120)) * rotPow / GetFPS();	// �}�E�X�c�ړ�

	// �J�����ʒu�𒆐S�ɃZ�b�g
	SetMousePoint(center.x, center.y);

	if (center.x <= mousePos.x) { axisDeg.y += rotPowY_; }
	if (center.x >= mousePos.x) { axisDeg.y += rotPowY_; }

	if (center.y >= mousePos.y && Utility::Rad2DegF(angle_.x) >= -30.0f)
	{
		axisDeg.x += rotPowX_;
	}
	if (center.y <= mousePos.y && Utility::Rad2DegF(angle_.x) <= 10.0f)
	{
		axisDeg.x += rotPowX_;
	}

	if (!Utility::EqualsVZero(axisDeg))
	{

		// �J��������]������
		// X���̃J�����̈ړ�����
		angle_.x += Utility::Deg2RadF(axisDeg.x);
		angle_.y += Utility::Deg2RadF(axisDeg.y);

		rotY_ = Quaternion::AngleAxis(angle_.y, Utility::AXIS_Y);

		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, Utility::AXIS_X));

	}

	// �Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = playerTransform_->pos;

	// �Ǐ]�Ώۂ��璍���_�܂ł̑��΍��W����]
	VECTOR relativeTPos = rotY_.PosAxis(LOCAL_P2T_POS);

	// �����_�̍X�V
	//targetPos_ = VAdd(followPos, relativeTPos);

	// �Ǐ]�Ώۂ���J�����܂ł̑��΍��W
	VECTOR relativeCPos = rotXY_.PosAxis(LOCAL_P2C_POS);

	// �J�����ʒu�̍X�V
	//pos_ = VAdd(followPos, relativeCPos);

	// �J�������W���������ړ�������
	pos_ = Utility::Lerp(pos_, VAdd(followPos, relativeCPos), 0.1f);

	// �����_���������ړ�������
	targetPos_ = Utility::Lerp(targetPos_, VAdd(followPos, relativeTPos), 0.1f);

	// �J�����̏����
	cameraUp_ = Utility::DIR_U;

}

void Camera::KeybordLockOnContoroller(void)
{

	auto& ins = InputManager::GetInstance();

	// �}�E�X�J�[�\�����\���ɂ���
	SetMouseDispFlag(false);

	// ��]
	//-------------------------------------
	VECTOR axisDeg = Utility::VECTOR_ZERO;

	// �}�E�X��]��
	float rotPow = 3.0f;

	// �}�E�X�ʒu
	Vector2 mousePos;

	// ��ʂ̒��S�ʒu
	Vector2 center = { Application::SCREEN_SIZE_X / 2,Application::SCREEN_SIZE_Y / 2 };

	// �}�E�X�ʒu�̎擾
	GetMousePoint(&mousePos.x, &mousePos.y);

	// �J������]�̌v�Z(�}�E�X�J�[�\���ʒu�Ɖ�ʂ̒��S�̍������v�Z���A��]��/FPS����Z����)
	// ���ꂪ��]��
	rotPowY_ = float(std::clamp(mousePos.x - center.x, -120, 120)) * rotPow / GetFPS();	// �}�E�X���ړ�
	rotPowX_ = float(std::clamp(mousePos.y - center.y, -120, 120)) * rotPow / GetFPS();	// �}�E�X�c�ړ�

	// �J�����ʒu�𒆐S�ɃZ�b�g
	SetMousePoint(center.x, center.y);

	if (center.x <= mousePos.x) { axisDeg.y += rotPowY_; }
	if (center.x >= mousePos.x) { axisDeg.y += rotPowY_; }

	if (center.y >= mousePos.y && Utility::Rad2DegF(lockOnAngles_.x) >= -20.0f)
	{
		axisDeg.x += rotPowX_;
	}
	if (center.y <= mousePos.y && Utility::Rad2DegF(lockOnAngles_.x) <= 10.0f)
	{
		axisDeg.x += rotPowX_;
	}

	if (!Utility::EqualsVZero(axisDeg))
	{

		// �J��������]������
		// X���̃J�����̈ړ�����
		lockOnAngles_.x += Utility::Deg2RadF(axisDeg.x);
		lockOnAngles_.y += Utility::Deg2RadF(axisDeg.y);

		rotY_ = Quaternion::AngleAxis(lockOnAngles_.y, Utility::AXIS_Y);

		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(lockOnAngles_.x, Utility::AXIS_X));

	}

}

void Camera::GamePadController(void)
{

	auto& ins = InputManager::GetInstance();

	// ��]
	//-------------------------------------
	VECTOR axisDeg = Utility::VECTOR_ZERO;

	// �Q�[���p�b�h�̔ԍ����擾
	auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	// �p�b�h�̕�����dir�ɒ���
	// �E����
	if (pad.AKeyRX > 0)
	{
		axisDeg.y = pad.AKeyRX;
		// �����𐳋K��
		axisDeg = VNorm(axisDeg);
		axisDeg = VScale(axisDeg, 3.0f);
	}
	// ������
	if (pad.AKeyRX < 0)
	{
		axisDeg.y = pad.AKeyRX;
		// �����𐳋K��
		axisDeg = VNorm(axisDeg);
		axisDeg = VScale(axisDeg, 3.0f);
	}
	// �����
	if (pad.AKeyRZ < 0 && Utility::Rad2DegF(angle_.x) <= 30.0f)
	{
		axisDeg.x = -pad.AKeyRZ;
		// �����𐳋K��
		axisDeg = VNorm(axisDeg);
		axisDeg = VScale(axisDeg, 3.0f);
	}
	// ������
	if (pad.AKeyRZ > 0 && Utility::Rad2DegF(angle_.x) >= -30.0f)
	{
		axisDeg.x = -pad.AKeyRZ;
		// �����𐳋K��
		axisDeg = VNorm(axisDeg);
		axisDeg = VScale(axisDeg, 3.0f);
	}


	if (axisDeg.x != 0.0f || axisDeg.y != 0.0f)
	{
		// �J��������]������
		// X���̃J�����̈ړ�����
		angle_.x += Utility::Deg2RadF(axisDeg.x);
		angle_.y += Utility::Deg2RadF(axisDeg.y);

		rotY_ = Quaternion::AngleAxis(angle_.y, Utility::AXIS_Y);

		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, Utility::AXIS_X));
	}

	// �Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = playerTransform_->pos;

	// �Ǐ]�Ώۂ��璍���_�܂ł̑��΍��W����]
	VECTOR relativeTPos = rotY_.PosAxis(LOCAL_P2T_POS);

	// �Ǐ]�Ώۂ���J�����܂ł̑��΍��W
	VECTOR relativeCPos = rotXY_.PosAxis(LOCAL_P2C_POS);

	// �J�������W���������ړ�������
	pos_ = Utility::Lerp(pos_, VAdd(followPos, relativeCPos), 0.1f);

	// �����_���������ړ�������
	targetPos_ = Utility::Lerp(targetPos_, VAdd(followPos, relativeTPos), 0.1f);

	// �J�����̏����
	cameraUp_ = Utility::DIR_U;

}

void Camera::GamePadLockOnContoroller(void)
{

	auto& ins = InputManager::GetInstance();

	// ��]
	//-------------------------------------
	VECTOR axisDeg = Utility::VECTOR_ZERO;

	// �Q�[���p�b�h�̔ԍ����擾
	auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	// �p�b�h�̕�����dir�ɒ���
	// �E����
	if (pad.AKeyRX > 0)
	{
		axisDeg.y = pad.AKeyRX;
		// �����𐳋K��
		axisDeg = VNorm(axisDeg);
		axisDeg = VScale(axisDeg, 3.0f);
	}
	// ������
	if (pad.AKeyRX < 0)
	{
		axisDeg.y = pad.AKeyRX;
		// �����𐳋K��
		axisDeg = VNorm(axisDeg);
		axisDeg = VScale(axisDeg, 3.0f);
	}
	// �����
	if (pad.AKeyRZ < 0 && Utility::Rad2DegF(lockOnAngles_.x) <= 10.0f)
	{
		axisDeg.x = -pad.AKeyRZ;
		// �����𐳋K��
		axisDeg = VNorm(axisDeg);
		axisDeg = VScale(axisDeg, 3.0f);
	}
	// ������
	if (pad.AKeyRZ > 0 && Utility::Rad2DegF(lockOnAngles_.x) >= -20.0f)
	{
		axisDeg.x = -pad.AKeyRZ;
		// �����𐳋K��
		axisDeg = VNorm(axisDeg);
		axisDeg = VScale(axisDeg, 3.0f);
	}


	if (axisDeg.x != 0.0f || axisDeg.y != 0.0f)
	{
		// �J��������]������
		// X���̃J�����̈ړ�����
		lockOnAngles_.x += Utility::Deg2RadF(axisDeg.x);
		lockOnAngles_.y += Utility::Deg2RadF(axisDeg.y);

		rotY_ = Quaternion::AngleAxis(lockOnAngles_.y, Utility::AXIS_Y);

		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(lockOnAngles_.x, Utility::AXIS_X));
	}

}

void Camera::CollisionStage(void)
{

	// ���̂Ƃ̏Փ˔���
	auto hits = MV1CollCheck_Sphere(
		stageId_, -1, pos_, 20.0f);

	if (hits.HitNum > 0)
	{
		pHit_ = true;
	}
	else
	{
		pHit_ = false;
	}

	// ���o�����n�ʃ|���S�����̌�n��
	MV1CollResultPolyDimTerminate(hits);

}

void Camera::LazyRotation(void)
{

	// �v���C���[�������Ă�������ɃJ��������]������
	// ��̃N�H�[�^�j�I���̊p�x��
	// ��]���Ƀ}�E�X���삪�����������]����߂�
	if (Quaternion::Angle(rotY_, lazyGoalRotY_) > abs(2.0f) && abs(rotPowY_) >= 5.0f && abs(rotPowX_) >= 5.0f)
	{
		rotY_ = Quaternion::Slerp(rotY_, lazyGoalRotY_, 0.1f);
		angle_.y = rotY_.ToEuler().y;
		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, Utility::AXIS_X));
	}
	// ��]���Ȃ��ꍇ
	else
	{
		rotY_ = lazyGoalRotY_;
		isLazy_ = false;
		angle_.y = rotY_.ToEuler().y;
		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, Utility::AXIS_X));
	}

}