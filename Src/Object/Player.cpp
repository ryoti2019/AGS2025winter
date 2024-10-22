#include <iostream>
#include <memory>
#include "../Utility/Utility.h"
#include "../Scene/GameScene.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/Common/InputController.h"
#include "Player.h"

Player::Player(const VECTOR& pos)
	:
	ActorBase(pos)
{

	// �@�\�̏�����
	InitFunction();

	// ���f��ID
	modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER);

	// ���f���̑傫��
	scl_ = 1.0f;

	// ���ʕ����͊��N���X�ŏ�����
	ActorBase::Init(pos);

	// �֐��|�C���^�̏�����
	InitFunctionPointer();

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
	stateChange_.emplace(STATE::KICK, std::bind(&Player::ChangeKick, this));
	stateChange_.emplace(STATE::UPPER, std::bind(&Player::ChangeUpper, this));
}

void Player::InitCollision()
{

	// �E��
	collisionData_.rightHand = MV1SearchFrame(transform_.modelId, "RightHandMiddle1");
	MATRIX matRighthandPos = MV1GetFrameLocalWorldMatrix(transform_.modelId, collisionData_.rightHand);
	collisionData_.rightHandPos = MGetTranslateElem(matRighthandPos);

}

void Player::InitParameter()
{

	dir_ = Utility::VECTOR_ZERO;

	// �A�N�^�[�̎��
	actorType_ = ActorType::PLAYER;

	// �U��1�i�K��
	jab_ = false;

	// �U��2�i�K��
	straight_ = false;

}

void Player::InitAnimation()
{

	// �A�j���[�V�����R���g���[���̐���
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);

	// �ҋ@
	animationController_->Add("IDLE", "Data/Model/Player/Idle.mv1", 0.0f, 30.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_IDLE), true, 0, false);

	// ����
	animationController_->Add("RUN", "Data/Model/Player/Run.mv1", 0.0f, 30.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_RUN), true, 0, false);

	// �W���u
	animationController_->Add("JAB", "Data/Model/Player/Jab.mv1", 0.0f, 80.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_JAB), false, 0, false);

	// �X�g���[�g
	animationController_->Add("STRAIGHT", "Data/Model/Player/Straight.mv1", 0.0f, 60.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_STRAIGHT), false, 0, false);

	// �L�b�N
	animationController_->Add("KICK", "Data/Model/Player/Kick.mv1", 0.0f, 40.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KICK), false, 0, false);

	// �A�b�p�[
	animationController_->Add("UPPER", "Data/Model/Player/Upper.mv1", 0.0f, 50.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_UPPER), false, 0, false);

	// �A�j���[�V�����Đ�����L�[
	key_ = "IDLE";

	// 1�O�̃A�j���[�V����
	preKey_ = key_;

	// �������
	ChangeState(STATE::IDLE);

}

void Player::Update(const float deltaTime)
{

	// �ړ�����
	Move();

	// �U������
	ComboAttack(deltaTime);

	// �R���{���Ɏ��̍U�����Ȃ�������ҋ@��Ԃɖ߂�
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(STATE::IDLE);
	}

	// ��Ԃ��Ƃ̍X�V
	stateUpdate_();

	// �A�j���[�V�����Đ�
	animationController_->Update(deltaTime);
	
	// ���f�������X�V
	transform_.Update();

}

void Player::Move()
{

	// ���͕���
	VECTOR dir = inputController_->Dir();

	// �U�����͈ړ��ł��Ȃ�
	if (!AttackState())
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

	// �������p�x�ɕϊ�����(XZ���� Y��)
	float angle = atan2f(dir_.x, dir_.z);

	// ��]����(��p)
	LazyRotation(angle);

	// �v���C���[�ɃJ������Ǐ]����Ƃ��͂������ɐ؂�ւ���
	//LazyRotation(cameraAngles.y + angle);

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
		// �L�b�N
		else if (state_ == STATE::STRAIGHT)
		{
			kick_ = true;
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

bool Player::AttackState()
{

	// �U����������
	if (state_ == STATE::JAB || state_ == STATE::STRAIGHT || state_ == STATE::KICK || state_ == STATE::UPPER)
	{
		return true;
	}

	return false;

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
	transform_.pos = VAdd(transform_.pos, VScale(dir_, 100.0f));

}

void Player::ChangeStraight()
{

	stateUpdate_ = std::bind(&Player::UpdateStraight, this);

	// �����O�Ɉړ�
	transform_.pos = VAdd(transform_.pos, VScale(dir_, 100.0f));

}

void Player::ChangeKick()
{
	stateUpdate_ = std::bind(&Player::UpdateKick, this);
}

void Player::ChangeUpper()
{

	stateUpdate_ = std::bind(&Player::UpdateUpper, this);

	// �����O�Ɉړ�
	transform_.pos = VAdd(transform_.pos, VScale(dir_, 100.0f));

}

void Player::UpdateIdle(void)
{
}

void Player::UpdateRun(void)
{
	// �ړ�����
	transform_.pos = VAdd(transform_.pos, VScale(dir_, 100.0f));
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
	// �L�b�N�ɑJ��
	if (animationController_->IsPreEndPlayAnimation() && kick_)
	{
		kick_ = false;
		ChangeState(STATE::KICK);
	}
}

void Player::UpdateKick()
{
}

void Player::UpdateUpper()
{
}
