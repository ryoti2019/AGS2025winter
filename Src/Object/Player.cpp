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
}

void Player::InitParameter()
{

	// �A�N�^�[�̎��
	actorType_ = ActorType::PLAYER;

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
	animationController_->Add("JAB", "Data/Model/Player/Run.mv1", 0.0f, 30.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_RUN), true, 0, false);

	// �X�g���[�g
	animationController_->Add("STRAIGHT", "Data/Model/Player/Run.mv1", 0.0f, 30.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_RUN), true, 0, false);

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

	// �������p�x�ɕϊ�����(XZ���� Y��)
	float angle = atan2f(dir_.x, dir_.z);

	// ��]����(��p)
	LazyRotation(angle);

	// �v���C���[�ɃJ������Ǐ]����Ƃ��͂������ɐ؂�ւ���
	//LazyRotation(cameraAngles.y + angle);

}

void Player::Attack()
{
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
}

void Player::ChangeStraight()
{
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
}

void Player::UpdateStraight()
{
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
