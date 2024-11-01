#include <memory>
#include "../Lib/ImGui/imgui.h"
#include "../Utility/Utility.h"
#include "../Scene/GameScene.h"
#include "../Manager/SceneManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/Camera.h"
#include "../Object/Common/InputController.h"
#include "../Manager/ActorManager.h"
#include "Player.h"

Player::Player(const VECTOR& pos, const json& data)
	:
	ActorBase(pos, data)
{

	// �@�\�̏�����
	InitFunction();

	// ���f��ID
	modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER);

	// �֐��|�C���^�̏�����
	InitFunctionPointer();

	// ���ʕ����͊��N���X�ŏ�����
	ActorBase::Init(pos);

	// �p�����[�^�̏�����
	InitParameter();

	// �A�j���[�V�����̏�����
	InitAnimation();

}

void Player::Init(const VECTOR& pos)
{
}

void Player::InitFunction()
{

	// �C���v�b�g�R���g���[���[�̐���
	inputController_ = std::make_unique<InputController>(this);

	// �J�����𐶐�
	std::weak_ptr<Camera> camera = SceneManager::GetInstance().GetCamera();

	// �J�����̃^�[�Q�b�g���v���C���[�ɐݒ�
	camera.lock()->SetPlayer(&transform_);

}

void Player::InitFunctionPointer()
{
	//�֐��|�C���^�̏�����
	stateChange_.emplace(STATE::IDLE, std::bind(&Player::ChangeIdle, this));
	stateChange_.emplace(STATE::RUN, std::bind(&Player::ChangeRun, this));
	stateChange_.emplace(STATE::JAB, std::bind(&Player::ChangeJab, this));
	stateChange_.emplace(STATE::STRAIGHT, std::bind(&Player::ChangeStraight, this));
	stateChange_.emplace(STATE::HOOK, std::bind(&Player::ChangeHook, this));
	stateChange_.emplace(STATE::LEFT_KICK, std::bind(&Player::ChangeLeftKick, this));
	stateChange_.emplace(STATE::RIGHT_KICK, std::bind(&Player::ChangeRightKick, this));
	stateChange_.emplace(STATE::UPPER, std::bind(&Player::ChangeUpper, this));
}

void Player::InitParameter()
{

	// �W���u
	jab_ = false;

	// �X�g���[�g
	straight_ = false;

	// �t�b�N
	hook_ = false;

	// ���L�b�N
	leftKick_ = false;

	// �E�L�b�N
	rightKick_ = false;

	// �A�b�p�[
	upper_ = false;

	// �E��̃t���[����
	RIGHT_HAND_FRAME = jsonData_["RIGHT_HAND_FRAME_NAME"];

	// ����̃t���[����
	LEFT_HAND_FRAME = jsonData_["LEFT_HAND_FRAME_NAME"];

	// �E���̃t���[����
	RIGHT_FOOT_FRAME = jsonData_["RIGHT_FOOT_FRAME_NAME"];

	// �����̃t���[����
	LEFT_FOOT_FRAME = jsonData_["LEFT_FOOT_FRAME_NAME"];

	// �̂̃t���[����
	BODY_FRAME = jsonData_["BODY_FRAME_NAME"];

	// �E��̃t���[���ԍ����擾
	collisionData_.rightHand = MV1SearchFrame(transform_.modelId, RIGHT_HAND_FRAME.c_str());

	// ����̃t���[���ԍ����擾
	collisionData_.leftHand = MV1SearchFrame(transform_.modelId, LEFT_HAND_FRAME.c_str());

	// �E���̃t���[���ԍ����擾
	collisionData_.rightFoot = MV1SearchFrame(transform_.modelId, RIGHT_FOOT_FRAME.c_str());

	// �����̃t���[���ԍ����擾
	collisionData_.leftFoot = MV1SearchFrame(transform_.modelId, LEFT_FOOT_FRAME.c_str());

	// �̂̃t���[���ԍ����擾
	collisionData_.body = MV1SearchFrame(transform_.modelId, BODY_FRAME.c_str());

	// �葫�̏Փ˔���̔��a
	collisionData_.handAndFootCollisionRadius = HAND_AND_FOOT_COLLISION_RADIUS;

	// �̂̏Փ˔���̔��a
	collisionData_.bodyCollisionRadius = BODY_COLLISION_RADIUS;

	ANIM_ACCEPT_TIME data;
	for (int i = static_cast<int>(STATE::JAB); i < static_cast<int>(STATE::MAX); ++i)
	{

		// �A�j���[�V�����̏��
		data.key = static_cast<STATE>(i);

		// �A�j���[�V�����̎�t�J�n����
		data.animAcceptStartTime = jsonData_["ANIM"][i-1]["ACCEPT_START"];

		// �A�j���[�V�����̎�t�I������
		data.animAcceptEndTime = jsonData_["ANIM"][i - 1]["ACCEPT_END"];

		// �A�j���[�V�����̎�t���Ԃ�ǉ�
		animAcceptTime_.emplace_back(data);

	}

	// ���̓J�E���^�̏�����
	acceptCnt_ = 0.0f;

}

void Player::InitAnimation()
{

	// �A�j���[�V�����R���g���[���̐���
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);

	// �A�j���[�V�����R���g���[���[�ɃA�j���[�V������ǉ�
	for (int i = static_cast<int>(STATE::IDLE); i < static_cast<int>(STATE::MAX); ++i)
	{

		// ���[�v�Đ�����A�j���[�V��������isLoop��true�ɂ���
		bool isLoop = i == static_cast<int>(STATE::IDLE) || i == static_cast<int>(STATE::RUN);
			animationController_->Add(

			// �A�j���[�V�����̖��O
			jsonData_["ANIM"][i - 1]["NAME"],

			// �A�j���[�V�����̃p�X
			jsonData_["ANIM"][i - 1]["PATH"],

			// �A�j���[�V�������n�܂鎞��
			0.0f,
			
			// �A�j���[�V�����X�s�[�h
			jsonData_["ANIM"][i - 1]["SPEED"],

			// �A�j���[�V�����n���h��
			resMng_.LoadModelDuplicate(static_cast<ResourceManager::SRC>(i)),

			// �A�j���[�V�����̃��[�v�Đ�
			isLoop,

			// �A�j���[�V�����ԍ�
			0,

			// �A�j���[�V�����̋t�Đ�
			false
			);
	}

	// �A�j���[�V�����Đ�����L�[
	key_ = "IDLE";

	// 1�O�̃A�j���[�V����
	preKey_ = key_;

	// �������
	ChangeState(STATE::IDLE);

}

void Player::Update(const float deltaTime)
{

	// ImGui�̃f�o�b�O�`��̍X�V
	UpdateDebugImGui();

	// �ړ�����
	Move();

	// �U������
	ComboAttack(deltaTime);

	// ���͎�t���Ԃ��J�E���g
	acceptCnt_++;

	// �R���{���Ɏ��̍U�����Ȃ�������ҋ@��Ԃɖ߂�
	for (const auto& stateData : animAcceptTime_)
	{
		if (state_ != stateData.key)continue;
		if (acceptCnt_ >= stateData.animAcceptEndTime)
		{
			ChangeState(STATE::IDLE);
			acceptCnt_ = 0.0f;
		}
	}

	// �Փ˔���̍X�V
	ActorBase::CollisionUpdate();

	// ��Ԃ��Ƃ̍X�V
	stateUpdate_();

	// �A�j���[�V�����Đ�
	animationController_->Update(deltaTime);

	// ���f�������X�V
	transform_.Update();

}

void Player::UpdateDebugImGui()
{

	// �E�B���h�E�^�C�g��&�J�n����
	ImGui::Begin("Player");
	// �傫��
	ImGui::Text("scale");
	ImGui::InputFloat("Scl", &scl_);
	// �p�x
	VECTOR rotDeg = VECTOR();
	rotDeg.x = Utility::Rad2DegF(transform_.quaRot.x);
	rotDeg.y = Utility::Rad2DegF(transform_.quaRot.y);
	rotDeg.z = Utility::Rad2DegF(transform_.quaRot.z);
	ImGui::Text("angle(deg)");
	ImGui::SliderFloat("RotX", &rotDeg.x, -90.0f, 90.0f);
	ImGui::SliderFloat("RotY", &rotDeg.y, -90.0f, 90.0f);
	ImGui::SliderFloat("RotZ", &rotDeg.z, -90.0f, 90.0f);
	transform_.quaRot.x = Utility::Deg2RadF(rotDeg.x);
	transform_.quaRot.y = Utility::Deg2RadF(rotDeg.y);
	transform_.quaRot.z = Utility::Deg2RadF(rotDeg.z);
	// �ʒu
	ImGui::Text("position");
	// �\���̂̐擪�|�C���^��n���Axyz�ƘA�������������z�u�փA�N�Z�X
	ImGui::InputFloat3("Pos", &transform_.pos.x);
	// �I������
	ImGui::End();

}

bool Player::GetAttackState()
{

	for (const auto state : attackState_)
	{
		if (state_ == state)
		{
			return true;
		}
	}

	return false;

}

void Player::Move()
{

	// ���͕���
	VECTOR dir = inputController_->Dir();

	// �U�����͈ړ��ł��Ȃ�
	if (!GetAttackState())
	{
		// ���͂��Ă�����ړ�����
		if (!Utility::EqualsVZero(dir))
		{
			// �������X�V
			dir_ = dir;

			ChangeState(STATE::RUN);
		}
		// ���͂��Ă��Ȃ���Αҋ@��Ԃɂ���
		else if (Utility::EqualsVZero(dir))
		{
			ChangeState(STATE::IDLE);
		}
	}

}

void Player::ComboAttack(const float deltaTime)
{

	// �U���̐�s����
	if (inputController_->ComboAttack())
	{
		// �W���u
		if (state_ == STATE::IDLE)
		{
			jab_ = true;
		}
		// �X�g���[�g
		else if (state_ == STATE::JAB)
		{
			straight_ = true;
		}
		// �t�b�N
		else if (state_ == STATE::STRAIGHT)
		{
			hook_ = true;
		}
		// ���L�b�N
		else if (state_ == STATE::HOOK)
		{
			leftKick_ = true;
		}
		// �E�L�b�N
		else if (state_ == STATE::LEFT_KICK)
		{
			rightKick_ = true;
		}
	}

	// �W���u�ɑJ��
	if (jab_)
	{
		jab_ = false;
		ChangeState(STATE::JAB);
	}

	// �A�b�p�[�ɑJ��
	if (inputController_->Upper() && !animationController_->IsPlayNowAnimation())
	{
		ChangeState(STATE::UPPER);
	}

}

void Player::ChangeState(STATE state)
{

	// ��ԑJ��
	state_ = state;

	// �֐��|�C���^�̑J��
	stateChange_[state_]();

	// �O�̃A�j���[�V������ۑ�
	preKey_ = key_;

	// �V�����A�j���[�V������ۑ�
	key_ = ANIM_DATA_KEY[static_cast<int>(state)];

	// �A�j���[�V�����R���g���[���[���̃A�j���[�V�����J��
	animationController_->ChangeAnimation(key_);

}

void Player::ChangeIdle(void)
{
	stateUpdate_ = std::bind(&Player::UpdateIdle, this);
}

void Player::ChangeRun(void)
{
	stateUpdate_ = std::bind(&Player::UpdateRun, this);
}

void Player::ChangeJab()
{

	stateUpdate_ = std::bind(&Player::UpdateJab, this);

	// �����O�Ɉړ�
	transform_.pos = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// ���͎�t���Ԃ����Z�b�g
	acceptCnt_ = 0.0f;

}

void Player::ChangeStraight()
{

	stateUpdate_ = std::bind(&Player::UpdateStraight, this);

	// �����O�Ɉړ�
	transform_.pos = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// ���͎�t���Ԃ����Z�b�g
	acceptCnt_ = 0.0f;

}

void Player::ChangeHook()
{

	stateUpdate_ = std::bind(&Player::UpdateHook, this);

	// �����O�Ɉړ�
	transform_.pos = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// ���͎�t���Ԃ����Z�b�g
	acceptCnt_ = 0.0f;

}

void Player::ChangeLeftKick()
{

	stateUpdate_ = std::bind(&Player::UpdateLeftKick, this);

	// �����O�Ɉړ�
	transform_.pos = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// ���͎�t���Ԃ����Z�b�g
	acceptCnt_ = 0.0f;

}

void Player::ChangeRightKick()
{

	stateUpdate_ = std::bind(&Player::UpdateRightKick, this);

	// �����O�Ɉړ�
	transform_.pos = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// ���͎�t���Ԃ����Z�b�g
	acceptCnt_ = 0.0f;

}

void Player::ChangeUpper()
{

	stateUpdate_ = std::bind(&Player::UpdateUpper, this);

	// �����O�Ɉړ�
	transform_.pos = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// ���͎�t���Ԃ����Z�b�g
	acceptCnt_ = 0.0f;

}

void Player::UpdateIdle(void)
{
}

void Player::UpdateRun(void)
{

	// �J�����̊p�x
	VECTOR cameraAngle = SceneManager::GetInstance().GetCamera().lock()->GetAngle();

	// Y���̍s��
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotY(cameraAngle.y));

	// ��]�s����g�p���āA�x�N�g������]������
	moveDir_ = VTransform(dir_, mat);

	// �ړ���
	speed_ = 100.0f;

	// �ړ���
	movePow_ = VScale(moveDir_, speed_);

	// �ړ�����
	transform_.pos = VAdd(transform_.pos, movePow_);

	// �������p�x�ɕϊ�����(XZ���� Y��)
	float angle = atan2f(dir_.x, dir_.z);

	// �v���C���[�ɃJ������Ǐ]����Ƃ��͂������ɐ؂�ւ���
	LazyRotation(cameraAngle.y + angle);

}

void Player::UpdateJab()
{
	// �X�g���[�g�ɑJ��
	if (animationController_->IsPreEndPlayAnimation() && straight_)
	{
		straight_ = false;
		ChangeState(STATE::STRAIGHT);
	}
}

void Player::UpdateStraight()
{
	// �t�b�N�ɑJ��
	if (animationController_->IsPreEndPlayAnimation() && hook_)
	{
		hook_ = false;
		ChangeState(STATE::HOOK);
	}
}

void Player::UpdateHook()
{
	// ���L�b�N�ɑJ��
	if (animationController_->IsPreEndPlayAnimation() && leftKick_)
	{
		leftKick_ = false;
		ChangeState(STATE::LEFT_KICK);
	}
}

void Player::UpdateLeftKick()
{
	// �E�L�b�N�ɑJ��
	if (animationController_->IsPreEndPlayAnimation() && rightKick_)
	{
		rightKick_ = false;
		ChangeState(STATE::RIGHT_KICK);
	}
}

void Player::UpdateRightKick()
{
}

void Player::UpdateUpper()
{
}
