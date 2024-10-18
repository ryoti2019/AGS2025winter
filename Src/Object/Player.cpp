#include <iostream>
#include <memory>
#include "../Utility/Utility.h"
#include "../Scene/GameScene.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "Player.h"

Player::Player(const VECTOR& pos) : ActorBase(pos)
{

	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	SetPos(pos);
	transform_.scl = { 1.0f,1.0f,1.0f };
	transform_.quaRot = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(0.0f),Utility::Deg2RadF(0.0f) });
	transform_.quaRotLocal = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(0.0f),Utility::Deg2RadF(0.0f) });
	transform_.Update();
	actorType_ = ActorType::PLAYER;
	std::weak_ptr<Camera> camera = SceneManager::GetInstance().GetCamera();
	camera.lock()->SetPlayer(&transform_);

	//�֐��|�C���^�̏�����
	stateChange_.emplace(STATE::IDLE, std::bind(&Player::ChangeIdle, this));
	stateChange_.emplace(STATE::RUN, std::bind(&Player::ChangeRun, this));

	// �A�j���[�V�����R���g���[���̐���
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);

	// �ҋ@
	animationController_->Add("IDLE", "Data/Model/Player.mv1", 0.0f, 30.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_IDLE), true, 0, false);

	// ����
	animationController_->Add("RUN", "Data/Model/Player.mv1", 0.0f, 30.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_RUN), true, 0, false);

	key_ = "IDLE";
	preKey_ = key_;

	// �������
	ChangeState(STATE::IDLE);

}

void Player::Init(const VECTOR& pos)
{
}

void Player::Update(const float deltaTime)
{
	//if (input)
	//{

	//}
	stateUpdate_();

	// �A�j���[�V�����Đ�
	animationController_->Update(deltaTime);

	transform_.Update();

}

void Player::ChangeIdle(void)
{
	stateUpdate_ = std::bind(&Player::UpdateIdle, this);
	stateDraw_ = std::bind(&Player::DrawIdle, this);
}

void Player::ChangeRun(void)
{
}

void Player::UpdateIdle(void)
{
}

void Player::UpdateRun(void)
{
	// �ړ�����
	transform_.pos = VAdd(transform_.pos, VScale(dir_, 10.0f));
}

void Player::DrawIdle(void)
{
}

void Player::DrawRun(void)
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
