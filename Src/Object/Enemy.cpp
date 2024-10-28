#include "../Manager/SceneManager.h"
#include "../Manager/CollisionManager.h"
#include "../Scene/GameScene.h"
#include "Enemy.h"

Enemy::Enemy(const VECTOR& pos, const json& data)
	:
	ActorBase(pos, data)
{

	// �@�\�̏�����
	InitFunction();

	// ���f��ID
	modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY);

	// ���f���̑傫��
	scl_ = 10.0f;

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

	// �̂̃t���[����
	BODY_FRAME = jsonData_["BODY_FRAME_NAME"];

	// �̂̃t���[���ԍ����擾
	collisionData_.body = MV1SearchFrame(transform_.modelId, BODY_FRAME.c_str());

	// �̂̏Փ˔���̔��a
	collisionData_.bodyCollisionRadius = BODY_COLLISION_RADIUS;

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
	ChangeState(STATE::RUN);

}

void Enemy::Update(const float deltaTime)
{

	// �ړ�����
	Move();

	if (hp_ <= 0)
	{
		ChangeState(STATE::IDLE);
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

bool Enemy::GetAttackState()
{
	return false;
}

void Enemy::Move()
{
}

void Enemy::ComboAttack(const float deltaTime)
{
}

bool Enemy::AttackState()
{
	return false;
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