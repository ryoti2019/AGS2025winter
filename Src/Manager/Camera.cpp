#include <EffekseerForDXLib.h>
#include "../Lib/ImGui/imgui.h"
#include "../Utility/Utility.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "SceneManager.h" 
#include "../Application.h"
#include "../Object/Common/Transform.h"
#include "Camera.h"

Camera::Camera()
{

	mode_ = MODE::FIXED_POINT;
	pos_ = { 0.0f,0.0f,0.0f };
	targetPos_ = { 0.0f, 0.0f, 0.0f };
	movePow_ = { 0.0f,0.0f,0.0f };
	angle_ = { 0.0f, Utility::Deg2RadF(90.0f), 0.0f};
	lockOn_ = false;
	specialMoveCnt_ = 0.0f;

	// �J�����̏����ݒ�
	SetDefault();

}

Camera::~Camera()
{
}

void Camera::Init()
{
}

void Camera::Update()
{

	// ImGui�̃f�o�b�O�`��̍X�V
	UpdateDebugImGui();

}

void Camera::SetBeforeDraw(const float deltaTime)
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
	case Camera::MODE::SPECIAL:
		SetBeforeDrawSpecial(deltaTime);
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

void Camera::SetBeforeDrawFixedPoint()
{
	pos_ = { 0.0f,5000.0f,-5000.0f };
}

void Camera::SetBeforeDrawFree()
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

void Camera::SetBeforeDrawFollow()
{

	auto& ins = InputManager::GetInstance();

	auto& sce = SceneManager::GetInstance();

	// �}�E�X�ł̑���
	if (!SceneManager::GetInstance().GetGamePad())
	{
		KeyboardController();
	}

	// �Q�[���p�b�h�ł̑���
	if (SceneManager::GetInstance().GetGamePad())
	{
		GamePadController();
	}

	//// Q�L�[����������true�ɂȂ�
	//if (isLazy_)
	//{
	//	LazyRotation();
	//}

	//// �X�e�[�W�̏Փ˔���
	//CollisionStage();

}

void Camera::SetBeforeDrawLockOn()
{

	// ������̈ʒu
	VECTOR playerPos = playerTransform_->pos;

	// �G�̈ʒu
	VECTOR enemyPos = { 0.0f,1000.0f,5000.0f };

	if (lockOn_)
	{
		enemyPos = enemyTransform_->pos;
		enemyPos = VAdd(enemyPos, { 0.0f,1000.0f,0.0f });
	}

	// �v���C���[�ƓG�̒��Ԓn�_��ݒ�
	VECTOR centerPos = VAdd(playerPos, enemyPos);

	// �v���C���[�ƃ��b�N�I���Ώۂ̋������v�Z
	float distance = VSize(VSub(playerPos, enemyPos));

	centerPos = VScale(centerPos, 0.5f);

	VECTOR pos = rotY_.PosAxis(LOCAL_P2C_POS);
	
	// �J�����̈ʒu
	pos_ = VAdd(playerPos, pos);

	// �����_��ݒ�
	targetPos_ = centerPos;

	// �J�����̏����
	cameraUp_ = Utility::DIR_U;

}

void Camera::SetBeforeDrawSpecial(const float deltaTime)
{

	auto& ins = InputManager::GetInstance();

	// ��]
	//-------------------------------------

	// �K�E�Z���̃J�����̈ړ����鎞�Ԃ��v�Z
	specialMoveCnt_ += deltaTime;

	// �J�����̃I�t�Z�b�g
	VECTOR pow = { 20.0f,0.02f,20.0f };

	// �J�����������鎞�Ԃ܂ňړ�������
	if (SPECIAL_MOVE_MAX_TIME >= specialMoveCnt_)
	{

		// �v���C���[���猩���J�����̃��[�J�����W
		VECTOR localRotPos = playerTransform_->quaRot.PosAxis({ 500.0f,2000.0f,500.0f });

		// �J�����̍��W��ݒ�
		pos_ = VAdd(playerTransform_->pos, localRotPos);

		// �J�����̈ړ��ʂ�ݒ�
		movePow_ = VAdd(movePow_,playerTransform_->quaRot.PosAxis(pow));

		// �J�������W���ړ�������
		pos_ = VAdd(pos_, movePow_);

	}

	// �J�����̏����
	cameraUp_ = Utility::DIR_U;

}


void Camera::Draw()
{
}

void Camera::Release()
{
}

void Camera::SetLazyAngles(const VECTOR angles)
{

	lazyGoalRotY_ = Quaternion::Euler(0.0f, angles.y, 0.0f);
	isLazy_ = true;

}

void Camera::SetPlayer(const std::shared_ptr<Transform>& follow)
{
	playerTransform_ = follow;
}

void Camera::SetEnemy(const std::shared_ptr<Transform>& follow)
{
	enemyTransform_ = follow;
}

void Camera::SetLockOn(const bool lockOn)
{
	lockOn_ = lockOn;
}

void Camera::ChangeMode(const MODE& mode)
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
		targetPos_ = VAdd(playerTransform_->pos, playerTransform_->quaRot.PosAxis(LOCAL_P2T_POS));
		angle_.y = playerTransform_->quaRot.ToEuler().y;
		rotY_ = playerTransform_->quaRot;
		rotXY_ = playerTransform_->quaRot;
		lockOnAngles_ = { 0.0f, 0.0f, 0.0f };
		break;
	case Camera::MODE::LOCKON:
		angle_ = { 0.0f,0.0f,0.0f };
		lockOnAngles_ = { 0.0f, 0.0f, 0.0f };
		break;
	case Camera::MODE::SPECIAL:

		movePow_ = { 0.0f,0.0f,0.0f };

		pos_ = VAdd(playerTransform_->pos, { 0.0f,1500.0f,0.0f });

		// ��]���ĂȂ�
		targetPos_ = VAdd(playerTransform_->pos,{0.0f,1000.0f,0.0f});

		// ��]���Ă���
		targetPos_ = VAdd(playerTransform_->pos, playerTransform_->quaRot.PosAxis({ 0.0f,1000.0f,0.0f }));

		specialMoveCnt_ = 0.0f;

		break;
	}

}

void Camera::AddLockOnAnglesY(float rad)
{
	lockOnAngles_.y += rad;
}

void Camera::SetStageID(const int modelId)
{
	stageId_ = modelId;
}

void Camera::SetDefault()
{

	// �J�����̏����ݒ�
	pos_ = DEFAULT_CAMERA_POS;

	// �����_
	targetPos_ = VAdd(pos_, LOCAL_P2T_POS);

	// �J�����̏����
	cameraUp_ = { 0.0f, 1.0f, 0.0f };

	// �J������X���ɌX���Ă��邪�A
	// ���̌X������Ԃ��p�x�[���A�X�������Ƃ���
	rotY_ = Quaternion::AngleAxis(angle_.y, Utility::AXIS_Y);
	rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, Utility::AXIS_X));

	// ���b�N�I��
	lockOn_ = false;

}

void Camera::SetTargetPosFollowForward()
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

void Camera::KeyboardController()
{

	auto& ins = InputManager::GetInstance();

	// ��]
	//-------------------------------------
	VECTOR axisDeg = Utility::VECTOR_ZERO;

	// ��ʂ̒��S�ʒu
	Vector2 center = { Application::SCREEN_SIZE_X / 2,Application::SCREEN_SIZE_Y / 2 };

	// �L�[�{�[�h����
	if (ins.IsNew(KEY_INPUT_UP)) { axisDeg.x += 1.0f; }
	if (ins.IsNew(KEY_INPUT_DOWN)) { axisDeg.x += -1.0f; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { axisDeg.y += -1.0f; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { axisDeg.y += 1.0f; }

	if (ins.IsNew(KEY_INPUT_RIGHT))
	{
		angle_.y += Utility::Deg2RadF(axisDeg.y);
	}
	if (ins.IsNew(KEY_INPUT_LEFT))
	{
		angle_.y += Utility::Deg2RadF(axisDeg.y);
	}
	if (ins.IsNew(KEY_INPUT_UP) && Utility::Rad2DegF(angle_.x) <= 40.0f)
	{
		angle_.x += Utility::Deg2RadF(axisDeg.x);
	}
	if (ins.IsNew(KEY_INPUT_DOWN) && Utility::Rad2DegF(angle_.x) >= -15.0f)
	{
		angle_.x += Utility::Deg2RadF(axisDeg.x);
	}

	//// �J��������]������
	//if (!Utility::EqualsVZero(axisDeg))
	//{

	//	// X���̃J�����̈ړ�����
	//	angle_.x += Utility::Deg2RadF(axisDeg.x);
	//	angle_.y += Utility::Deg2RadF(axisDeg.y);

	//	rotY_ = Quaternion::AngleAxis(angle_.y, Utility::AXIS_Y);

	//	rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, Utility::AXIS_X));

	//}

	//// �Ǐ]�Ώۂ̈ʒu
	//VECTOR followPos = playerTransform_->pos;

	//// �Ǐ]�Ώۂ��璍���_�܂ł̑��΍��W����]
	//VECTOR relativeTPos = rotY_.PosAxis(LOCAL_P2T_POS);

	//// �Ǐ]�Ώۂ���J�����܂ł̑��΍��W
	//VECTOR relativeCPos = rotXY_.PosAxis(LOCAL_P2C_POS);

	//// �J�������W���������ړ�������
	//pos_ = Utility::Lerp(pos_, VAdd(followPos, relativeCPos), 0.1f);

	//// �����_���������ړ�������
	//targetPos_ = Utility::Lerp(targetPos_, VAdd(followPos, relativeTPos), 0.1f);

	//// �J�����̏����
	//cameraUp_ = Utility::DIR_U;

	//// �}�E�X�J�[�\�����\���ɂ���
	//SetMouseDispFlag(false);

	//// ��]
	////-------------------------------------
	//VECTOR axisDeg = Utility::VECTOR_ZERO;

	//// �}�E�X��]��
	//float rotPow = 4.0f;
	////float rotPow = 0.0f;

	//// �}�E�X�ʒu
	//Vector2 mousePos;

	//// ��ʂ̒��S�ʒu
	//Vector2 center = { Application::SCREEN_SIZE_X / 2,Application::SCREEN_SIZE_Y / 2 };

	//// �}�E�X�ʒu�̎擾
	//GetMousePoint(&mousePos.x, &mousePos.y);

	//// �J������]�̌v�Z(�}�E�X�J�[�\���ʒu�Ɖ�ʂ̒��S�̍������v�Z���A��]��/FPS����Z����)
	//// ���ꂪ��]��
	//rotPowY_ = float(std::clamp(mousePos.x - center.x, -120, 120)) * rotPow / GetFPS();	// �}�E�X���ړ�
	//rotPowX_ = float(std::clamp(mousePos.y - center.y, -120, 120)) * rotPow / GetFPS();	// �}�E�X�c�ړ�

	//// �J�����ʒu�𒆐S�ɃZ�b�g
	//SetMousePoint(center.x, center.y);

	//if (center.x <= mousePos.x) { axisDeg.y += rotPowY_; }
	//if (center.x >= mousePos.x) { axisDeg.y += rotPowY_; }

	//if (center.y >= mousePos.y && Utility::Rad2DegF(angle_.x) >= -30.0f)
	//{
	//	axisDeg.x += rotPowX_;
	//}
	//if (center.y <= mousePos.y && Utility::Rad2DegF(angle_.x) <= 10.0f)
	//{
	//	axisDeg.x += rotPowX_;
	//}

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

	// �Ǐ]�Ώۂ���J�����܂ł̑��΍��W
	VECTOR relativeCPos = rotXY_.PosAxis(LOCAL_P2C_POS);

	// �J�������W���������ړ�������
	pos_ = Utility::Lerp(pos_, VAdd(followPos, relativeCPos), 0.1f);

	// �����_���������ړ�������
	targetPos_ = Utility::Lerp(targetPos_, VAdd(followPos, relativeTPos), 0.1f);

	// �J�����̏����
	cameraUp_ = Utility::DIR_U;

}

void Camera::KeyboardLockOnController()
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

void Camera::GamePadController()
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
	if (pad.AKeyRZ < 0 && Utility::Rad2DegF(angle_.x) >= -30.0f)
	{
		axisDeg.x = pad.AKeyRZ;
		// �����𐳋K��
		axisDeg = VNorm(axisDeg);
		axisDeg = VScale(axisDeg, 3.0f);
	}
	// ������
	if (pad.AKeyRZ > 0 && Utility::Rad2DegF(angle_.x) <= 30.0f)
	{
		axisDeg.x = pad.AKeyRZ;
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
	pos_ = Utility::Lerp(pos_, VAdd(followPos, relativeCPos), 1.0f);

	// �����_���������ړ�������
	targetPos_ = Utility::Lerp(targetPos_, VAdd(followPos, relativeTPos), 1.0f);

	// �J�����̏����
	cameraUp_ = Utility::DIR_U;

}

void Camera::GamePadLockOnController()
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

void Camera::UpdateDebugImGui()
{

	// �E�B���h�E�^�C�g��&�J�n����
	ImGui::Begin("Camera");

	// �傫��
	//ImGui::Text("scale");
	//ImGui::InputFloat("Scl", &scl_);

	// �p�x
	VECTOR rotDeg = VECTOR();
	rotDeg.x = Utility::Rad2DegF(angle_.x);
	rotDeg.y = Utility::Rad2DegF(angle_.y);
	rotDeg.z = Utility::Rad2DegF(angle_.z);
	ImGui::Text("angle(deg)");
	ImGui::SliderFloat("RotX", &rotDeg.x, -90.0f, 90.0f);
	ImGui::SliderFloat("RotY", &rotDeg.y, -90.0f, 90.0f);
	ImGui::SliderFloat("RotZ", &rotDeg.z, -90.0f, 90.0f);
	angle_.x = Utility::Deg2RadF(rotDeg.x);
	angle_.y = Utility::Deg2RadF(rotDeg.y);
	angle_.z = Utility::Deg2RadF(rotDeg.z);

	// �ʒu
	ImGui::Text("position");
	// �\���̂̐擪�|�C���^��n���Axyz�ƘA�������������z�u�փA�N�Z�X
	ImGui::InputFloat3("Pos", &pos_.x);
	// �I������
	ImGui::End();

}

void Camera::LazyRotation()
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