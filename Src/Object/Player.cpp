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
	ActorBase(pos, data),
	JAB_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::JAB) - 1]["ATTACK_START_FRAME"]),
	JAB_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::JAB) - 1]["ATTACK_END_FRAME"]),
	JAB_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::JAB) - 1]["DAMAGE"]),
	STRAIGHT_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::STRAIGHT) - 1]["ATTACK_START_FRAME"]),
	STRAIGHT_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::STRAIGHT) - 1]["ATTACK_END_FRAME"]),
	STRAIGHT_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::STRAIGHT) - 1]["DAMAGE"]),
	HOOK_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::HOOK) - 1]["ATTACK_START_FRAME"]),
	HOOK_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::HOOK) - 1]["ATTACK_END_FRAME"]),
	HOOK_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::HOOK) - 1]["DAMAGE"]),
	LEFT_KICK_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::LEFT_KICK) - 1]["ATTACK_START_FRAME"]),
	LEFT_KICK_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::LEFT_KICK) - 1]["ATTACK_END_FRAME"]),
	LEFT_KICK_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::LEFT_KICK) - 1]["DAMAGE"]),
	RIGHT_KICK_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::RIGHT_KICK) - 1]["ATTACK_START_FRAME"]),
	RIGHT_KICK_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::RIGHT_KICK) - 1]["ATTACK_END_FRAME"]),
	RIGHT_KICK_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::RIGHT_KICK) - 1]["DAMAGE"]),
	UPPER_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::UPPER) - 1]["ATTACK_START_FRAME"]),
	UPPER_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::UPPER) - 1]["ATTACK_END_FRAME"]),
	UPPER_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::UPPER) - 1]["DAMAGE"])
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
	stateChange_.emplace(PlayerState::IDLE, std::bind(&Player::ChangeIdle, this));
	stateChange_.emplace(PlayerState::RUN, std::bind(&Player::ChangeRun, this));
	stateChange_.emplace(PlayerState::JAB, std::bind(&Player::ChangeJab, this));
	stateChange_.emplace(PlayerState::STRAIGHT, std::bind(&Player::ChangeStraight, this));
	stateChange_.emplace(PlayerState::HOOK, std::bind(&Player::ChangeHook, this));
	stateChange_.emplace(PlayerState::LEFT_KICK, std::bind(&Player::ChangeLeftKick, this));
	stateChange_.emplace(PlayerState::RIGHT_KICK, std::bind(&Player::ChangeRightKick, this));
	stateChange_.emplace(PlayerState::UPPER, std::bind(&Player::ChangeUpper, this));
	stateChange_.emplace(PlayerState::HIT_HEAD, std::bind(&Player::ChangeHitHead, this));
	stateChange_.emplace(PlayerState::HIT_BODY, std::bind(&Player::ChangeHitBody, this));

}

void Player::InitParameter()
{

	// �U���̓��͂�������
	for (int i = static_cast<int>(PlayerState::JAB); i < static_cast<int>(PlayerState::MAX); i++)
	{
		isCombo_.emplace(static_cast<PlayerState>(i), false);
	}

	// ��������
	moveDir_ = { 0.0f,0.0f,1.0f };

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

	// �E��ɍU�����肪���邩�ǂ���
	collisionData_.isRightHandAttack = false;

	// ����ɍU�����肪���邩�ǂ���
	collisionData_.isLeftHandAttack = false;

	// �E���ɍU�����肪���邩�ǂ���
	collisionData_.isRightFootAttack = false;

	// �����ɍU�����肪���邩�ǂ���
	collisionData_.isLeftFootAttack = false;

	// ���̓J�E���^�̏�����
	acceptCnt_ = 0.0f;

	// ����Ƃ��̈ړ���
	RUN_MOVE_POW = jsonData_["RUN_MOVE_POW"];

	// �A�j���[�V�����ԍ�
	ANIM_INDEX = jsonData_["ANIM_INDEX"];

}

void Player::InitAnimation()
{

	// �A�j���[�V�����R���g���[���̐���
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);

	// �A�j���[�V�����R���g���[���[�ɃA�j���[�V������ǉ�
	for (int i = static_cast<int>(PlayerState::IDLE); i < static_cast<int>(PlayerState::MAX); ++i)
	{

		// ���[�v�Đ�����A�j���[�V��������isLoop��true�ɂ���
		bool isLoop = i == static_cast<int>(PlayerState::IDLE) || i == static_cast<int>(PlayerState::RUN);
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
			ANIM_INDEX,

			// �A�j���[�V�����̋t�Đ�
			false
		);
	}

	// �A�j���[�V�����Đ�����L�[
	key_ = "IDLE";

	// 1�O�̃A�j���[�V����
	preKey_ = key_;

	// �������
	ChangeState(PlayerState::IDLE);

}

void Player::Update(const float deltaTime)
{

	// ImGui�̃f�o�b�O�`��̍X�V
	UpdateDebugImGui();

	// �ړ�����
	Move();

	// �U������
	Attack();

	// �d��
	Gravity(gravityScale_);

	// ���͎�t���Ԃ��J�E���g
	acceptCnt_++;

	// ��Ԃ��Ƃ̍X�V
	stateUpdate_();

	// �A�j���[�V�����Đ�
	animationController_->Update(deltaTime);

	// ���f�������X�V
	transform_.Update();

	// �Փ˔���̍X�V
	ActorBase::CollisionUpdate();
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

	// �U���̏�Ԃ�����
	for (const auto state : attackState_)
	{
		if (state_ == state)
		{
			return true;
		}
	}

	return false;

}

bool Player::GetHitState()
{

	// �U�����󂯂Ă����Ԃ�����
	for (const auto state : hitState_)
	{
		if (state_ == state)
		{
			return true;
		}
	}

	return false;

}

bool Player::GetComboState()
{

	// �R���{��������
	for (const auto state : comboState_)
	{
		if (state_ == state)
		{
			return true;
		}
	}

	return false;

}

void Player::AttackHit(const int damage, const int state)
{

	// �ǂ̃A�j���[�V�������`�F�b�N����
	AttackHitCheck(state);

	// HP�����炷
	SubHp(damage);
	
	// �A�j���[�V�����̍Đ����Ԃ����Z�b�g����
	animationController_->ResetStepAnim();

}

void Player::AttackHitCheck(const int state)
{

	// ���Ƀq�b�g����A�j���[�V�������`�F�b�N
	for (const auto hitState : hitHeadState_)
	{
		if (hitState == static_cast<EnemyState>(state))
		{
			ChangeState(PlayerState::HIT_HEAD);
			return;
		}
	}

	// �̂Ƀq�b�g����A�j���[�V�������`�F�b�N
	for (const auto hitState : hitBodyState_)
	{
		if (hitState == static_cast<EnemyState>(state))
		{
			ChangeState(PlayerState::HIT_BODY);
			return;
		}
	}

}

void Player::Move()
{

	// �q�b�g���͍s���ł��Ȃ�
	for (const auto hitState : hitState_)
	{
		if (hitState == state_)
		{
			return;
		}
	}

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

			ChangeState(PlayerState::RUN);
		}
		// ���͂��Ă��Ȃ���Αҋ@��Ԃɂ���
		else if (Utility::EqualsVZero(dir))
		{
			ChangeState(PlayerState::IDLE);
		}
	}

}

void Player::Attack()
{

	// �q�b�g���͍s���ł��Ȃ�
	for (const auto hitState : hitState_)
	{
		if (hitState == state_)
		{
			return;
		}
	}

	// �U���̐�s����
	if (inputController_->ComboAttack())
	{
		// �R���{�̐�s���͂̏���
		for (int i = static_cast<int>(PlayerState::JAB); i <= static_cast<int>(PlayerState::RIGHT_KICK); i++)
		{

			// �����画�f������̂�T��
			auto isCombo = isCombo_.find(static_cast<PlayerState>(i));

			// false�ł����true�ɂ��Đ�s���͂�ێ�����
			if (!isCombo->second)
			{
				isCombo->second = true;
				break;
			}
		}

		// �W���u�ɑJ��
		if (isCombo_.at(PlayerState::JAB) && !isCombo_.at(PlayerState::STRAIGHT))
		{
			ChangeState(PlayerState::JAB);
		}

	}

	// �A�b�p�[�ɑJ��
	if (inputController_->Upper() && state_ != PlayerState::UPPER)
	{
		ChangeState(PlayerState::UPPER);
	}

	//�R���{��������
	if (!GetComboState())return;

	// ���̓��͂��Ȃ���΃R���{���L�����Z������
	for(int i = static_cast<int>(PlayerState::JAB); i < static_cast<int>(PlayerState::RIGHT_KICK); i++)
	{

		// ���̏�Ԃ̓��͂����邽�߂̌v�Z
		const int nextState = static_cast<int>(state_) + 1;

		// false��������R���{���L�����Z��
		if (!isCombo_.at(static_cast<PlayerState>(nextState)))
		{
			// �ҋ@��ԂɑJ��
			if (animationController_->IsEndPlayAnimation())
			{
				ChangeState(PlayerState::IDLE);
				return;
			}
		}

	}

}

void Player::ChangeState(PlayerState state)
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

	// �E��̍U��������Ȃ���
	collisionData_.isRightHandAttack = false;

	// ����̍U��������Ȃ���
	collisionData_.isLeftHandAttack = false;

	// �E���̍U��������Ȃ���
	collisionData_.isRightFootAttack = false;

	// �����̍U��������Ȃ���
	collisionData_.isLeftFootAttack = false;

	// �U�����������Ă��邩�����Z�b�g����
	isAttackHit_ = false;

	// �d�͂�ʏ��Ԃɖ߂�
	gravityScale_ = 1.0f;

}

void Player::ChangeRun(void)
{
	stateUpdate_ = std::bind(&Player::UpdateRun, this);
}

void Player::ChangeJab()
{

	stateUpdate_ = std::bind(&Player::UpdateJab, this);

	// ���͎�t���Ԃ����Z�b�g
	acceptCnt_ = 0.0f;

	// �U�����������Ă��邩�����Z�b�g����
	isAttackHit_ = false;

	// �_���[�W��
	damage_ = JAB_DAMAGE;

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

	// �ǂꂾ���i�ނ��v�Z
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, speed_));

}

void Player::ChangeStraight()
{

	stateUpdate_ = std::bind(&Player::UpdateStraight, this);

	// ���͎�t���Ԃ����Z�b�g
	acceptCnt_ = 0.0f;

	// �U�����������Ă��邩�����Z�b�g����
	isAttackHit_ = false;

	// �_���[�W��
	damage_ = STRAIGHT_DAMAGE;

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

	// �ǂꂾ���i�ނ��v�Z
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, speed_));

}

void Player::ChangeHook()
{

	stateUpdate_ = std::bind(&Player::UpdateHook, this);

	// ���͎�t���Ԃ����Z�b�g
	acceptCnt_ = 0.0f;

	// �U�����������Ă��邩�����Z�b�g����
	isAttackHit_ = false;

	// �_���[�W��
	damage_ = HOOK_DAMAGE;

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

	// �ǂꂾ���i�ނ��v�Z
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, speed_));

}

void Player::ChangeLeftKick()
{

	stateUpdate_ = std::bind(&Player::UpdateLeftKick, this);

	// ���͎�t���Ԃ����Z�b�g
	acceptCnt_ = 0.0f;

	// �U�����������Ă��邩�����Z�b�g����
	isAttackHit_ = false;

	// �_���[�W��
	damage_ = LEFT_KICK_DAMAGE;

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

	// �ǂꂾ���i�ނ��v�Z
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, speed_));

}

void Player::ChangeRightKick()
{

	stateUpdate_ = std::bind(&Player::UpdateRightKick, this);

	// ���͎�t���Ԃ����Z�b�g
	acceptCnt_ = 0.0f;

	// �U�����������Ă��邩�����Z�b�g����
	isAttackHit_ = false;

	// �_���[�W��
	damage_ = RIGHT_KICK_DAMAGE;

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

	// �ǂꂾ���i�ނ��v�Z
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, speed_));

}

void Player::ChangeUpper()
{

	stateUpdate_ = std::bind(&Player::UpdateUpper, this);

	// �ǂꂾ���i�ނ��v�Z
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// ���͎�t���Ԃ����Z�b�g
	acceptCnt_ = 0.0f;

	// �U�����������Ă��邩�����Z�b�g����
	isAttackHit_ = false;

	// �_���[�W��
	damage_ = UPPER_DAMAGE;

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

	// �ǂꂾ���i�ނ��v�Z
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, speed_));

}

void Player::ChangeHitHead()
{

	stateUpdate_ = std::bind(&Player::UpdateHitHead, this);

	// �v���C���[�̕��������߂�
	VECTOR vec = VSub(targetPos_, transform_.pos);

	// ���K��
	vec = VNorm(vec);

	// �v���C���[�̕����Ƌt�����̃x�N�g��
	vec = { -vec.x, vec.y,-vec.z };

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

	// �ǂꂾ���i�ނ��v�Z
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, speed_));

}

void Player::ChangeHitBody()
{

	stateUpdate_ = std::bind(&Player::UpdateHitBody, this);

	// �v���C���[�̕��������߂�
	VECTOR vec = VSub(targetPos_, transform_.pos);

	// ���K��
	vec = VNorm(vec);

	// �v���C���[�̕����Ƌt�����̃x�N�g��
	vec = { -vec.x, vec.y,-vec.z };

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

	// �ǂꂾ���i�ނ��v�Z
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, speed_));

}

void Player::UpdateIdle(void)
{

	// �U���̓��͂�������
	for (int i = static_cast<int>(PlayerState::JAB); i < static_cast<int>(PlayerState::MAX); i++)
	{
		isCombo_.at(static_cast<PlayerState>(i)) = false;
	}

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

	// ���K��
	moveDir_ = VNorm(moveDir_);

	// �X�s�[�h
	speed_ = RUN_MOVE_POW;

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

	// �����O�ɂ������ړ�
	transform_.pos = Utility::Lerp(transform_.pos, movePow_, 0.1f);

	// �U�����肪����t���[��
	if (JAB_ATTACK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= JAB_ATTACK_END_FRAME)
	{
		collisionData_.isLeftHandAttack = true;
	}
	else
	{
		collisionData_.isLeftHandAttack = false;
	}

	// �X�g���[�g�ɑJ��
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(PlayerState::STRAIGHT))
	{
		ChangeState(PlayerState::STRAIGHT);
	}

}

void Player::UpdateStraight()
{

	// �����O�ɂ������ړ�
	transform_.pos = Utility::Lerp(transform_.pos, movePow_, 0.1f);

	// �U�����肪����t���[��
	if (STRAIGHT_ATTACK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= STRAIGHT_ATTACK_END_FRAME)
	{
		collisionData_.isRightHandAttack = true;
	}
	else
	{
		collisionData_.isRightHandAttack = false;
	}

	// �t�b�N�ɑJ��
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(PlayerState::HOOK))
	{
		ChangeState(PlayerState::HOOK);
	}

}

void Player::UpdateHook()
{

	// �����O�ɂ������ړ�
	transform_.pos = Utility::Lerp(transform_.pos, movePow_, 0.1f);

	// �U�����肪����t���[��
	if (HOOK_ATTACK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= HOOK_ATTACK_END_FRAME)
	{
		collisionData_.isLeftHandAttack = true;
	}
	else
	{
		collisionData_.isLeftHandAttack = false;
	}

	// ���L�b�N�ɑJ��
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(PlayerState::LEFT_KICK))
	{
		ChangeState(PlayerState::LEFT_KICK);
	}

}

void Player::UpdateLeftKick()
{

	// �����O�ɂ������ړ�
	transform_.pos = Utility::Lerp(transform_.pos, movePow_, 0.1f);

	// �U�����肪����t���[��
	if (LEFT_KICK_ATTACK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= LEFT_KICK_ATTACK_END_FRAME)
	{
		collisionData_.isLeftFootAttack = true;
	}
	else
	{
		collisionData_.isLeftFootAttack = false;
	}

	// �E�L�b�N�ɑJ��
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(PlayerState::RIGHT_KICK))
	{
		ChangeState(PlayerState::RIGHT_KICK);
	}

}

void Player::UpdateRightKick()
{

	// �����O�ɂ������ړ�
	transform_.pos = Utility::Lerp(transform_.pos, movePow_, 0.1f);

	// �U�����肪����t���[��
	if (RIGHT_KICK_ATTACK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= RIGHT_KICK_ATTACK_END_FRAME)
	{
		collisionData_.isRightFootAttack = true;
	}
	else
	{
		collisionData_.isRightFootAttack = false;
	}

	// �ҋ@��ԂɑJ��
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);
	}

}

void Player::UpdateUpper()
{

	// �����O�ɂ������ړ�
	transform_.pos = Utility::Lerp(transform_.pos, movePow_, 0.1f);

	// �U�����肪����t���[��
	if (UPPER_ATTACK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= UPPER_ATTACK_END_FRAME)
	{
		collisionData_.isRightHandAttack = true;
	}
	else
	{
		collisionData_.isRightHandAttack = false;
	}

	// �ҋ@��ԂɑJ��
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);
	}

}

void Player::UpdateHitHead()
{

	// �ҋ@��ԂɑJ��
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);
	}

}

void Player::UpdateHitBody()
{

	// �ҋ@��ԂɑJ��
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);
	}

}
