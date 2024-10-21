#include "Enemy.h"

Enemy::Enemy(const VECTOR& pos)
	:
	ActorBase(pos)
{

	// �@�\�̏�����
	InitFunction();

	// ���f��ID
	modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY);

	// ���f���̑傫��
	scl_ = 10.0f;

	// ���ʕ����͊��N���X�ŏ�����
	ActorBase::Init(pos);

	// �֐��|�C���^�̏�����
	InitFunctionPointer();

	// �p�����[�^�̏�����
	InitParameter();

	// �A�j���[�V�����̏�����
	InitAnimation();

}

void Enemy::Init(const VECTOR& pos)
{

	// �@�\�̏�����
	InitFunction();

	// ���f��ID
	modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY);

	// ���f���̑傫��
	scl_ = 10.0f;

	// ���ʕ����͊��N���X�ŏ�����
	ActorBase::Init(pos);

	// �֐��|�C���^�̏�����
	InitFunctionPointer();

	// �p�����[�^�̏�����
	InitParameter();

	// �A�j���[�V�����̏�����
	InitAnimation();

}

void Enemy::InitFunction()
{
}

void Enemy::InitParameter()
{

	// �A�N�^�[�̎��
	actorType_ = ActorType::ENEMY;

}

void Enemy::InitFunctionPointer()
{
	//�֐��|�C���^�̏�����
	stateChange_.emplace(STATE::IDLE, std::bind(&Enemy::ChangeIdle, this));
	stateChange_.emplace(STATE::RUN, std::bind(&Enemy::ChangeRun, this));
}

void Enemy::InitAnimation()
{

	// �A�j���[�V�����R���g���[���̐���
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);

	// �ҋ@
	animationController_->Add("IDLE", "Data/Model/Enemy/Idle.mv1", 0.0f, 30.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY_IDLE), true, 0, false);

	// ����
	animationController_->Add("RUN", "Data/Model/Enemy/Run.mv1", 0.0f, 30.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY_RUN), true, 0, false);

	// �A�j���[�V�����Đ�����L�[
	key_ = "IDLE";

	// 1�O�̃A�j���[�V����
	preKey_ = key_;

	// �������
	ChangeState(STATE::IDLE);

}

void Enemy::Update(const float deltaTime)
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

void Enemy::Move()
{
}

void Enemy::Attack(const float deltaTime)
{
}

void Enemy::ChangeState(STATE state)
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

void Enemy::ChangeIdle()
{
	stateUpdate_ = std::bind(&Enemy::UpdateIdle, this);
	stateDraw_ = std::bind(&Enemy::DrawIdle, this);
}

void Enemy::ChangeRun()
{
	stateUpdate_ = std::bind(&Enemy::UpdateRun, this);
	stateDraw_ = std::bind(&Enemy::DrawRun, this);
}

void Enemy::UpdateIdle()
{
}

void Enemy::UpdateRun()
{
}

void Enemy::DrawIdle()
{
}

void Enemy::DrawRun()
{
}