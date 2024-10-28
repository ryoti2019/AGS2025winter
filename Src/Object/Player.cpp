#include <memory>
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
	stateChange_.emplace(STATE::KICK, std::bind(&Player::ChangeKick, this));
	stateChange_.emplace(STATE::UPPER, std::bind(&Player::ChangeUpper, this));
}

void Player::InitParameter()
{

	// �W���u
	jab_ = false;

	// �X�g���[�g
	straight_ = false;

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

	// �ړ�����
	Move();

	// �J�����̊p�x
	VECTOR cameraAngle = SceneManager::GetInstance().GetCamera().lock()->GetAngle();

	// ���N���X����g�������^�փL���X�g����
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	auto activeData = gameScene->GetActorManager()->GetActiveActorData();

	// �G�̕���
	VECTOR dir = Utility::VECTOR_ZERO;;

	for (auto& data : activeData)
	{
		for (const std::shared_ptr<ActorBase>& actor : data.second)
		{

			// �G
			const auto& enemys = activeData.find(ActorType::ENEMY);

			// ���g�������Ă��邩�m�F
			if (enemys == activeData.end())continue;

			for (const std::shared_ptr<ActorBase>& enemy : enemys->second)
			{
				// true�������炱�̓G�����b�N�I������
				if (enemy->GetIsLockOn())
				{
					dir = VSub(enemy->GetPos(), transform_.pos);
					dir = VNorm(dir);
					isLockOn_ = true;
				}
			}

		}
	}

	// �������p�x�ɕϊ�����(XZ���� Y��)
	float angle = atan2f(dir.x, dir.z);

	// �v���C���[�ɃJ������Ǐ]����Ƃ��͂������ɐ؂�ւ���
	LazyRotation(cameraAngle.y + angle);

	// �U������
	ComboAttack(deltaTime);

	// �R���{���Ɏ��̍U�����Ȃ�������ҋ@��Ԃɖ߂�
	if (animationController_->IsEndPlayAnimation())
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

	if (isLockOn_)
	{
		dir = inputController_->LockOnDir(transform_);
	}

	//// �J�����̊p�x
	//VECTOR cameraAngle = SceneManager::GetInstance().GetCamera().lock()->GetAngle();

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

	//// �������p�x�ɕϊ�����(XZ���� Y��)
	//float angle = atan2f(dir_.x, dir_.z);

	//// �v���C���[�ɃJ������Ǐ]����Ƃ��͂������ɐ؂�ւ���
	//LazyRotation(cameraAngle.y + angle);

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
	transform_.pos = VAdd(transform_.pos, VScale(dir_, ATTACK_MOVE_POW));

}

void Player::ChangeStraight()
{

	stateUpdate_ = std::bind(&Player::UpdateStraight, this);

	// �����O�Ɉړ�
	transform_.pos = VAdd(transform_.pos, VScale(dir_, ATTACK_MOVE_POW));

}

void Player::ChangeKick()
{
	stateUpdate_ = std::bind(&Player::UpdateKick, this);
}

void Player::ChangeUpper()
{

	stateUpdate_ = std::bind(&Player::UpdateUpper, this);

	// �����O�Ɉړ�
	transform_.pos = VAdd(transform_.pos, VScale(dir_, ATTACK_MOVE_POW));

}

void Player::UpdateIdle(void)
{
}

void Player::UpdateRun(void)
{
	// �ړ�����
	transform_.pos = VAdd(transform_.pos, VScale(dir_, ATTACK_MOVE_POW));
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
