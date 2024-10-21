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
	ActorBase(pos),
	COMBO_MAX_TIME(1.0f)
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
}

void Player::InitParameter()
{

	// �A�N�^�[�̎��
	actorType_ = ActorType::PLAYER;

	// �R���{�J�E���^
	comboCnt_ = COMBO_MAX_TIME;

	// �U��1�i�K��
	attack_ = false;

	// �U��2�i�K��
	attack2_ = false;

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
	animationController_->Add("JAB", "Data/Model/Player/Jab.mv1", 0.0f, 60.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_JAB), false, 0, false);

	// �X�g���[�g
	animationController_->Add("STRAIGHT", "Data/Model/Player/Straight.mv1", 0.0f, 60.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_STRAIGHT), false, 0, false);

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
	Attack(deltaTime);

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
	if (state_ != STATE::JAB && state_ != STATE::STRAIGHT)
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

void Player::Attack(const float deltaTime)
{

	// �R���{��t���Ԃ̏���
	if (comboCnt_ > 0.0f)
	{
		comboCnt_ -= deltaTime;
	}
	// �R���{���Ɏ��̍U�����Ȃ�������ҋ@��Ԃɖ߂�
	else
	{
		ChangeState(STATE::IDLE);
		comboCnt_ = COMBO_MAX_TIME;
	}

	// �U���̐�s����
	if (inputController_->Attack() && comboCnt_ > 0.0f)
	{
		if (state_ == STATE::IDLE)
		{
			attack_ = true;
		}
		else if (state_ == STATE::JAB)
		{
			attack2_ = true;
		}
	}

	// �W���u�ɑJ��
	if (attack_)
	{
		attack_ = false;
		ChangeState(STATE::JAB);
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
	stateDraw_ = std::bind(&Player::DrawIdle, this);
}

void Player::ChangeRun(void)
{
	stateUpdate_ = std::bind(&Player::UpdateRun, this);
	stateDraw_ = std::bind(&Player::DrawRun, this);
}

void Player::ChangeJab()
{

	stateUpdate_ = std::bind(&Player::UpdateJab, this);
	stateDraw_ = std::bind(&Player::DrawJab, this);

	// �R���{�J�E���^�����Ƃɖ߂�
	comboCnt_ = COMBO_MAX_TIME;

}

void Player::ChangeStraight()
{

	stateUpdate_ = std::bind(&Player::UpdateStraight, this);
	stateDraw_ = std::bind(&Player::DrawStraight, this);

	// �R���{�J�E���^�����Ƃɖ߂�
	comboCnt_ = COMBO_MAX_TIME;

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
	if (animationController_->IsEndPlayAnimation() && attack2_)
	{
		attack2_ = false;
		ChangeState(STATE::STRAIGHT);
	}

}

void Player::UpdateStraight()
{
	attack2_ = false;
}

void Player::DrawIdle(void)
{
}

void Player::DrawRun(void)
{
}

void Player::DrawJab()
{
}

void Player::DrawStraight()
{
}
