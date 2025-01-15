#include <memory>
#include "../Lib/ImGui/imgui.h"
#include "../Utility/Utility.h"
#include "../Scene/GameScene.h"
#include "../Manager/SceneManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/Camera.h"
#include "../Component/InputComponent.h"
#include "../Component/MoveComponent.h"
#include "../Object/Common/InputController.h"
#include "../Component/InputComponent.h"
#include "../Manager/ActorManager.h"
#include "Player.h"

Player::Player(const VECTOR& pos, const json& data)
	:
	ActorBase(pos, data),
	ATTACK_JAB_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_JAB) - 1]["ATTACK_START_FRAME"]),
	ATTACK_JAB_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_JAB) - 1]["ATTACK_END_FRAME"]),
	ATTACK_JAB_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_JAB) - 1]["DAMAGE"]),
	ATTACK_STRAIGHT_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_STRAIGHT) - 1]["ATTACK_START_FRAME"]),
	ATTACK_STRAIGHT_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_STRAIGHT) - 1]["ATTACK_END_FRAME"]),
	ATTACK_STRAIGHT_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_STRAIGHT) - 1]["DAMAGE"]),
	ATTACK_HOOK_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_HOOK) - 1]["ATTACK_START_FRAME"]),
	ATTACK_HOOK_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_HOOK) - 1]["ATTACK_END_FRAME"]),
	ATTACK_HOOK_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_HOOK) - 1]["DAMAGE"]),
	ATTACK_LEFT_KICK_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_LEFT_KICK) - 1]["ATTACK_START_FRAME"]),
	ATTACK_LEFT_KICK_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_LEFT_KICK) - 1]["ATTACK_END_FRAME"]),
	ATTACK_LEFT_KICK_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_LEFT_KICK) - 1]["DAMAGE"]),
	ATTACK_RIGHT_KICK_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_RIGHT_KICK) - 1]["ATTACK_START_FRAME"]),
	ATTACK_RIGHT_KICK_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_RIGHT_KICK) - 1]["ATTACK_END_FRAME"]),
	ATTACK_RIGHT_KICK_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_RIGHT_KICK) - 1]["DAMAGE"]),
	ATTACK_UPPER_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_UPPER) - 1]["ATTACK_START_FRAME"]),
	ATTACK_UPPER_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_UPPER) - 1]["ATTACK_END_FRAME"]),
	ATTACK_UPPER_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_UPPER) - 1]["DAMAGE"]),
	ATTACK_CHARGE_PUNCH_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_CHARGE_PUNCH) - 1]["ATTACK_START_FRAME"]),
	ATTACK_CHARGE_PUNCH_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_CHARGE_PUNCH) - 1]["ATTACK_END_FRAME"]),
	ATTACK_CHARGE_PUNCH_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_CHARGE_PUNCH) - 1]["DAMAGE"]),
	ATTACK_CHARGE_PUNCH_TIME(data["ATTACK_CHARGE_PUNCH_TIME"]),
	ATTACK_SPECIAL_PUNCH_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_SPECIAL_PUNCH) - 1]["ATTACK_START_FRAME"]),
	ATTACK_SPECIAL_PUNCH_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_SPECIAL_PUNCH) - 1]["DAMAGE"]),
	ATTACK_SPECIAL_PUNCH_COLLISION_TIME(data["ATTACK_SPECIAL_PUNCH_COLLISION_TIME"])
{

	// �@�\�̏�����
	InitFunction();

	// ���f��ID
	modelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::MODEL_PLAYER)]);

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

	// ���͗p�̃R���|�[�l���g��ǉ�
	inputComponent_ = std::make_unique<InputComponent>(std::static_pointer_cast<Player>(GetThis()));

	// �ړ��p�̃R���|�[�l���g��ǉ�
	moveComponent_ = std::make_unique<MoveComponent>(std::static_pointer_cast<Player>(GetThis()));

	// �`��p�̃R���|�[�l���g
	drawComponent_ = std::make_unique<DrawComponent>(std::static_pointer_cast<Player>(GetThis()));

}

void Player::InitFunction()
{

	// �J�����𐶐�
	std::weak_ptr<Camera> camera = SceneManager::GetInstance().GetCamera();

	// �J�����̃^�[�Q�b�g���v���C���[�ɐݒ�
	camera.lock()->SetPlayer(transform_);

}

void Player::InitFunctionPointer()
{
	
	//�֐��|�C���^�̏�����
	stateChange_.emplace(PlayerState::IDLE, std::bind(&Player::ChangeIdle, this));
	stateChange_.emplace(PlayerState::RUN, std::bind(&Player::ChangeRun, this));
	stateChange_.emplace(PlayerState::ATTACK_JAB, std::bind(&Player::ChangeJab, this));
	stateChange_.emplace(PlayerState::ATTACK_STRAIGHT, std::bind(&Player::ChangeStraight, this));
	stateChange_.emplace(PlayerState::ATTACK_HOOK, std::bind(&Player::ChangeHook, this));
	stateChange_.emplace(PlayerState::ATTACK_LEFT_KICK, std::bind(&Player::ChangeLeftKick, this));
	stateChange_.emplace(PlayerState::ATTACK_RIGHT_KICK, std::bind(&Player::ChangeRightKick, this));
	stateChange_.emplace(PlayerState::ATTACK_UPPER, std::bind(&Player::ChangeUpper, this));
	stateChange_.emplace(PlayerState::ATTACK_CHARGE_PUNCH, std::bind(&Player::ChangeChargePunch, this));
	stateChange_.emplace(PlayerState::ATTACK_SPECIAL_PUNCH, std::bind(&Player::ChangeSpecialAttack, this));
	stateChange_.emplace(PlayerState::POWER_CHARGE, std::bind(&Player::ChangePowerCharge, this));
	stateChange_.emplace(PlayerState::HIT_HEAD, std::bind(&Player::ChangeHitHead, this));
	stateChange_.emplace(PlayerState::HIT_BODY, std::bind(&Player::ChangeHitBody, this));

}

void Player::InitParameter()
{

	// �U���̓���
	for (int i = static_cast<int>(PlayerState::ATTACK_JAB); i <= static_cast<int>(PlayerState::ATTACK_RIGHT_KICK); i++)
	{
		isCombo_.emplace(static_cast<PlayerState>(i), false);
	}

	// ����
	dir_ = { 0.0f,0.0f,1.0f };

	// ��������
	moveDir_ = transform_->quaRot.GetForward();

	// �{�X��̂݊p�x��ς���
	if (SceneManager::GetInstance().GetSceneID() == SCENE_ID::BOSS_BATTLE)
	{
		// �p�x��ύX
		transform_->quaRot = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(180.0f),Utility::Deg2RadF(0.0f) });
		transform_->Update();
	}

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
	collisionData_.rightHand = MV1SearchFrame(transform_->modelId, RIGHT_HAND_FRAME.c_str());

	// ����̃t���[���ԍ����擾
	collisionData_.leftHand = MV1SearchFrame(transform_->modelId, LEFT_HAND_FRAME.c_str());

	// �E���̃t���[���ԍ����擾
	collisionData_.rightFoot = MV1SearchFrame(transform_->modelId, RIGHT_FOOT_FRAME.c_str());

	// �����̃t���[���ԍ����擾
	collisionData_.leftFoot = MV1SearchFrame(transform_->modelId, LEFT_FOOT_FRAME.c_str());

	// �̂̃t���[���ԍ����擾
	collisionData_.body = MV1SearchFrame(transform_->modelId, BODY_FRAME.c_str());

	// �K�E�Z�̓����蔻��̍��W��ݒ�
	collisionData_.projectilePos = VAdd(transform_->pos, BODY_RELATIVE_CENTER_POS);

	// �葫�̏Փ˔���̔��a
	collisionData_.handAndFootCollisionRadius = HAND_AND_FOOT_COLLISION_RADIUS;

	// �̂̏Փ˔���̔��a
	collisionData_.bodyCollisionRadius = BODY_COLLISION_RADIUS;

	// �K�E�Z�̏Փ˔���̔��a
	collisionData_.projectileCollisionRadius = jsonData_["PROJECTILE_COLLISION_RADIUS"];

	// �E��ɍU�����肪���邩�ǂ���
	collisionData_.isRightHandAttack = false;

	// ����ɍU�����肪���邩�ǂ���
	collisionData_.isLeftHandAttack = false;

	// �E���ɍU�����肪���邩�ǂ���
	collisionData_.isRightFootAttack = false;

	// �����ɍU�����肪���邩�ǂ���
	collisionData_.isLeftFootAttack = false;

	// �K�E�Z�ɍU�����肪���邩�ǂ���
	collisionData_.isProjectileAttack = false;

	// ���߃p���`�̃J�E���^
	attackChargePunchCnt_ = 0.0f;

	// HP�̍ő�l
	HP_MAX = jsonData_["HP"];

	// HP
	hp_ = HP_MAX;

	// HP�o�[�̒���
	HP_BAR_LENGTH = jsonData_["HP_BAR_LENGTH"];

	// ����Ƃ��̈ړ���
	RUN_MOVE_POW = jsonData_["RUN_MOVE_POW"];

	// �A�j���[�V�����ԍ�
	ANIM_INDEX = jsonData_["ANIM_INDEX"];

}

void Player::InitAnimation()
{

	// �A�j���[�V�����R���g���[���̐���
	animationController_ = std::make_unique<AnimationController>(transform_->modelId);

	// �A�j���[�V�����R���g���[���[�ɃA�j���[�V������ǉ�
	for (int i = static_cast<int>(PlayerState::IDLE); i < static_cast<int>(PlayerState::MAX); ++i)
	{

		// ���[�v�Đ�����A�j���[�V��������isLoop��true�ɂ���
		bool isLoop = i == static_cast<int>(PlayerState::IDLE) || i == static_cast<int>(PlayerState::RUN);
		animationController_->Add(

			// �A�j���[�V�����̖��O
			jsonData_["ANIM"][i - 1]["NAME"],

			// �A�j���[�V�������n�܂鎞��
			0.0f,

			// �A�j���[�V�����X�s�[�h
			jsonData_["ANIM"][i - 1]["SPEED"],

			// �A�j���[�V�����n���h��
			resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::MODEL_PLAYER) + i]),

			// �A�j���[�V�����̃��[�v�Đ�
			isLoop,

			// �A�j���[�V�����ԍ�
			ANIM_INDEX,

			// �A�j���[�V�����̋t�Đ�
			false
		);
	}

	// �A�j���[�V�����Đ�����L�[
	key_ = "";

	// 1�O�̃A�j���[�V����
	preKey_ = "";

	// �������
	ChangeState(PlayerState::IDLE);

}

void Player::Update(const float deltaTime)
{

	// ImGui�̃f�o�b�O�`��̍X�V
	UpdateDebugImGui();

	// ���͂̍X�V
	inputComponent_->Update(deltaTime);

	// �Փ˔���̍X�V
	ActorBase::CollisionUpdate();

	// ��Ԃ��Ƃ̍X�V
	stateUpdate_(deltaTime);

	//gravityScale_ = 10.0f;

	// �d��
	//if (velocity_.y != 0.0f)
	//{
		Gravity(gravityScale_);
	//}

	// ���f�������X�V
	transform_->Update();

	// �A�j���[�V�����Đ�
	animationController_->Update(deltaTime);

}

void Player::Draw(const float deltaTime)
{

	// ���N���X�̕`�揈��
	ActorBase::Draw(deltaTime);

	// HP�o�[�`��
	int hpLength = HP_BAR_LENGTH;
	int H;
	int hpGauge;
	H = hp_ * (512.0f / HP_MAX) - 100;
	int R = min(max((384 - H), 0), 0xff);
	int G = min(max((H + 64), 0), 0xff);
	int B = max((H - 384), 0);
	hpGauge = hpLength * hp_ / HP_MAX;

	if (hp_ >= 0)
	{
		DrawBox(40, 20, 40 + hpGauge, 40, GetColor(R, G, B), true);
	}


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
	rotDeg.x = Utility::Rad2DegF(transform_->quaRot.x);
	rotDeg.y = Utility::Rad2DegF(transform_->quaRot.y);
	rotDeg.z = Utility::Rad2DegF(transform_->quaRot.z);
	ImGui::Text("angle(deg)");
	ImGui::SliderFloat("RotX", &rotDeg.x, -90.0f, 90.0f);
	ImGui::SliderFloat("RotY", &rotDeg.y, -90.0f, 90.0f);
	ImGui::SliderFloat("RotZ", &rotDeg.z, -90.0f, 90.0f);
	transform_->quaRot.x = Utility::Deg2RadF(rotDeg.x);
	transform_->quaRot.y = Utility::Deg2RadF(rotDeg.y);
	transform_->quaRot.z = Utility::Deg2RadF(rotDeg.z);

	// �ʒu
	ImGui::Text("position");
	// �\���̂̐擪�|�C���^��n���Axyz�ƘA�������������z�u�փA�N�Z�X
	ImGui::InputFloat3("Pos", &transform_->pos.x);
	// �I������
	ImGui::End();

}

const bool Player::GetAttackState()const
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

const std::vector<int> Player::GetTotalAttackTypes() const
{

	std::vector<int> intStates;
	intStates.reserve(attackState_.size());

	// �ϊ�����
	for (const auto& state : attackState_)
	{
		intStates.push_back(static_cast<int>(state));
	}

	return intStates;

}

const bool Player::GetHitState()const
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

const bool Player::GetSuperArmorState() const
{
	// �X�[�p�[�A�[�}�[��Ԃ�����
	for (const auto state : superArmorState_)
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

void Player::AttackHit(const int damage, const int type)
{

	// �ǂ̃A�j���[�V�������`�F�b�N����
	AttackHitCheck(type);

	// HP�����炷
	SubHp(damage);
	
	// �A�j���[�V�����̍Đ����Ԃ����Z�b�g����
	animationController_->ResetStepAnim();

}

void Player::ProjectileHit(const int damage)
{

	// �q�b�g�A�j���[�V�����ɑJ��
	ChangeState(PlayerState::HIT_BODY);

	// HP�����炷
	SubHp(damage);

	// �A�j���[�V�����̍Đ����Ԃ����Z�b�g����
	animationController_->ResetStepAnim();

}

void Player::AttackHitCheck(const int type)
{

	// ���Ƀq�b�g����A�j���[�V�������`�F�b�N
	for (const auto hitState : hitHeadEnemyState_)
	{
		if (hitState == static_cast<EnemyState>(type))
		{
			ChangeState(PlayerState::HIT_HEAD);
			return;
		}
	}

	// ���Ƀq�b�g����A�j���[�V�������`�F�b�N
	for (const auto hitState : hitHeadBossState_)
	{
		if (hitState == static_cast<BossState>(type))
		{
			ChangeState(PlayerState::HIT_HEAD);
			return;
		}
	}

	// �̂Ƀq�b�g����A�j���[�V�������`�F�b�N
	for (const auto hitState : hitBodyEnemyState_)
	{
		if (hitState == static_cast<EnemyState>(type))
		{
			ChangeState(PlayerState::HIT_BODY);
			return;
		}
	}

	// �̂Ƀq�b�g����A�j���[�V�������`�F�b�N
	for (const auto hitState : hitBodyBossState_)
	{
		if (hitState == static_cast<BossState>(type))
		{
			ChangeState(PlayerState::HIT_BODY);
			return;
		}
	}

}

void Player::Rotation()
{

	// �J�����̊p�x
	VECTOR cameraAngle = SceneManager::GetInstance().GetCamera().lock()->GetAngle();

	// Y���̍s��
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotY(cameraAngle.y));

	// ��]�s����g�p���āA�x�N�g������]������
	moveDir_ = VTransform(dir_, mat);

	// �������p�x�ɕϊ�����(XZ���� Y��)
	float angle = atan2f(dir_.x, dir_.z);

	// ��������]����
	LazyRotation(cameraAngle.y + angle);

}

void Player::BossAreaCollisionCheck()
{



}

void Player::ChangeState(const PlayerState state)
{

	// �O�̃A�j���[�V������ۑ�
	preKey_ = key_;

	// �V�����A�j���[�V������ۑ�
	key_ = ANIM_DATA_KEY[static_cast<int>(state)];

	// �O�Ɠ����A�j���[�V�����Ȃ珈�����Ȃ�
	if (preKey_ == key_)return;

	// ��ԑJ��
	state_ = state;

	// �֐��|�C���^�̑J��
	stateChange_[state_]();

	// �O�Ɠ�����ԂłȂ���΃J�E���^�����Z�b�g����
	if (preKey_ != key_)
	{
		// ���߃p���`�̃J�E���^�����Z�b�g
		attackChargePunchCnt_ = 0.0f;
	}

	// �A�j���[�V�����R���g���[���[���̃A�j���[�V�����J��
	animationController_->ChangeAnimation(key_);

}

void Player::ChangeIdle(void)
{

	stateUpdate_ = std::bind(&Player::UpdateIdle, this, std::placeholders::_1);

	// �d�͂�ʏ��Ԃɖ߂�
	gravityScale_ = 1.0f;

}

void Player::ChangeRun(void)
{

	stateUpdate_ = std::bind(&Player::UpdateRun, this, std::placeholders::_1);

	// �X�s�[�h
	speed_ = RUN_MOVE_POW;

}

void Player::ChangeJab()
{

	stateUpdate_ = std::bind(&Player::UpdateJab, this, std::placeholders::_1);

	// ����̍U��������Ȃ���
	collisionData_.isLeftHandAttack = false;

	// �_���[�W��
	damage_ = ATTACK_JAB_DAMAGE;

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

	// �ړ�����
	inputComponent_->Dir();

}

void Player::ChangeStraight()
{

	stateUpdate_ = std::bind(&Player::UpdateStraight, this, std::placeholders::_1);

	// �E��̍U��������Ȃ���
	collisionData_.isRightHandAttack = false;

	// �_���[�W��
	damage_ = ATTACK_STRAIGHT_DAMAGE;

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

	// �ړ�����
	inputComponent_->Dir();

}

void Player::ChangeHook()
{

	stateUpdate_ = std::bind(&Player::UpdateHook, this, std::placeholders::_1);

	// ����̍U��������Ȃ���
	collisionData_.isLeftHandAttack = false;

	// �_���[�W��
	damage_ = ATTACK_HOOK_DAMAGE;

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

	// �ړ�����
	inputComponent_->Dir();

}

void Player::ChangeLeftKick()
{

	stateUpdate_ = std::bind(&Player::UpdateLeftKick, this, std::placeholders::_1);

	// �����̍U��������Ȃ���
	collisionData_.isLeftFootAttack = false;

	// �_���[�W��
	damage_ = ATTACK_LEFT_KICK_DAMAGE;

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

	// �ړ�����
	inputComponent_->Dir();

}

void Player::ChangeRightKick()
{

	stateUpdate_ = std::bind(&Player::UpdateRightKick, this, std::placeholders::_1);

	// �E���̍U��������Ȃ���
	collisionData_.isRightFootAttack = false;

	// �_���[�W��
	damage_ = ATTACK_RIGHT_KICK_DAMAGE;

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

	// �ړ�����
	inputComponent_->Dir();

}

void Player::ChangeUpper()
{

	stateUpdate_ = std::bind(&Player::UpdateUpper, this, std::placeholders::_1);

	// �E��̍U��������Ȃ���
	collisionData_.isRightHandAttack = false;

	// �_���[�W��
	damage_ = ATTACK_UPPER_DAMAGE;

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

}

void Player::ChangeChargePunch()
{

	stateUpdate_ = std::bind(&Player::UpdateChargePunch, this, std::placeholders::_1);

	// �E��̍U��������Ȃ���
	collisionData_.isRightHandAttack = false;

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

}

void Player::ChangeSpecialAttack()
{

	stateUpdate_ = std::bind(&Player::UpdateSpecialAttack, this, std::placeholders::_1);

	// �_���[�W��
	damage_ = ATTACK_SPECIAL_PUNCH_DAMAGE;

	// �K�E�Z�̓����蔻��̍��W��ݒ�
	collisionData_.projectilePos = VAdd(transform_->pos, BODY_RELATIVE_CENTER_POS);

	// �K�E�Z�̏Փ˔��肪�������Ԃ̃J�E���^�����Z�b�g
	attackSpecialPunchCollisionCnt_ = 0.0f;

	// �J�����𐶐�
	std::weak_ptr<Camera> camera = SceneManager::GetInstance().GetCamera();
	camera.lock()->ChangeMode(Camera::MODE::FOLLOW);

}

void Player::ChangePowerCharge()
{

	stateUpdate_ = std::bind(&Player::UpdatePowerCharge, this, std::placeholders::_1);

	// �J�����𐶐�
	std::weak_ptr<Camera> camera = SceneManager::GetInstance().GetCamera();
	camera.lock()->ChangeMode(Camera::MODE::SPECIAL);

}

void Player::ChangeHitHead()
{

	stateUpdate_ = std::bind(&Player::UpdateHitHead, this, std::placeholders::_1);

	// �v���C���[�̕��������߂�
	VECTOR vec = VSub(targetPos_, transform_->pos);

	// ���K��
	vec = VNorm(vec);

	// �v���C���[�̕����Ƌt�����̃x�N�g��
	vec = { -vec.x, vec.y,-vec.z };

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

}

void Player::ChangeHitBody()
{

	stateUpdate_ = std::bind(&Player::UpdateHitBody, this, std::placeholders::_1);

	// �v���C���[�̕��������߂�
	VECTOR vec = VSub(targetPos_, transform_->pos);

	// ���K��
	vec = VNorm(vec);

	// �v���C���[�̕����Ƌt�����̃x�N�g��
	vec = { -vec.x, vec.y,-vec.z };

	// �X�s�[�h
	speed_ = ATTACK_MOVE_POW;

}

void Player::UpdateIdle(const float deltaTime)
{

	// �U���̓��͂�������
	for (int i = static_cast<int>(PlayerState::ATTACK_JAB); i <= static_cast<int>(PlayerState::ATTACK_RIGHT_KICK); i++)
	{
		isCombo_.at(static_cast<PlayerState>(i)) = false;
	}

}

void Player::UpdateRun(const float deltaTime)
{

	// �U���̓��͂�������
	for (int i = static_cast<int>(PlayerState::ATTACK_JAB); i <= static_cast<int>(PlayerState::ATTACK_RIGHT_KICK); i++)
	{
		isCombo_.at(static_cast<PlayerState>(i)) = false;
	}

	// ��]����
	Rotation();

	// �ړ�����
	moveComponent_->Move();

}

void Player::UpdateJab(const float deltaTime)
{

	// ��]����
	Rotation();

	// �����O�ɂ������ړ�
	moveComponent_->Lerp();

	// �U�����肪����t���[��
	if (ATTACK_JAB_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= ATTACK_JAB_END_FRAME)
	{
		collisionData_.isLeftHandAttack = true;
	}
	else
	{
		collisionData_.isLeftHandAttack = false;
	}

	// �X�g���[�g�ɑJ��
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(PlayerState::ATTACK_STRAIGHT))
	{
		ChangeState(PlayerState::ATTACK_STRAIGHT);
	}

}

void Player::UpdateStraight(const float deltaTime)
{

	// ��]����
	Rotation();

	// �����O�ɂ������ړ�
	moveComponent_->Lerp();

	// �U�����肪����t���[��
	if (ATTACK_STRAIGHT_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= ATTACK_STRAIGHT_END_FRAME)
	{
		collisionData_.isRightHandAttack = true;
	}
	else
	{
		collisionData_.isRightHandAttack = false;
	}

	// �t�b�N�ɑJ��
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(PlayerState::ATTACK_HOOK))
	{
		ChangeState(PlayerState::ATTACK_HOOK);
	}

}

void Player::UpdateHook(const float deltaTime)
{

	// ��]����
	Rotation();

	// �����O�ɂ������ړ�
	moveComponent_->Lerp();

	// �U�����肪����t���[��
	if (ATTACK_HOOK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= ATTACK_HOOK_END_FRAME)
	{
		collisionData_.isLeftHandAttack = true;
	}
	else
	{
		collisionData_.isLeftHandAttack = false;
	}

	// ���L�b�N�ɑJ��
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(PlayerState::ATTACK_LEFT_KICK))
	{
		ChangeState(PlayerState::ATTACK_LEFT_KICK);
	}

}

void Player::UpdateLeftKick(const float deltaTime)
{

	// ��]����
	Rotation();

	// �����O�ɂ������ړ�
	moveComponent_->Lerp();

	// �U�����肪����t���[��
	if (ATTACK_LEFT_KICK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= ATTACK_LEFT_KICK_END_FRAME)
	{
		collisionData_.isLeftFootAttack = true;
	}
	else
	{
		collisionData_.isLeftFootAttack = false;
	}

	// �E�L�b�N�ɑJ��
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(PlayerState::ATTACK_RIGHT_KICK))
	{
		ChangeState(PlayerState::ATTACK_RIGHT_KICK);
	}

}

void Player::UpdateRightKick(const float deltaTime)
{

	// ��]����
	Rotation();

	// �����O�ɂ������ړ�
	moveComponent_->Lerp();

	// �U�����肪����t���[��
	if (ATTACK_RIGHT_KICK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= ATTACK_RIGHT_KICK_END_FRAME)
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

void Player::UpdateUpper(const float deltaTime)
{

	// �����O�ɂ������ړ�
	moveComponent_->Lerp();

	// �U�����肪����t���[��
	if (ATTACK_UPPER_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= ATTACK_UPPER_END_FRAME)
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

void Player::UpdateChargePunch(const float deltaTime)
{

	// �����O�ɂ������ړ�
	moveComponent_->Lerp();

	// �U�����肪����t���[��
	if (ATTACK_CHARGE_PUNCH_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= ATTACK_CHARGE_PUNCH_END_FRAME)
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

void Player::UpdateSpecialAttack(const float deltaTime)
{

	// �U�����肪����t���[��
	if (ATTACK_SPECIAL_PUNCH_START_FRAME <= animationController_->GetStepAnim() && ATTACK_SPECIAL_PUNCH_COLLISION_TIME >= attackSpecialPunchCollisionCnt_)
	{

		// �����蔻�������
		collisionData_.isProjectileAttack = true;

		// �K�E�Z�̓����蔻��̍��W�ݒ�
		collisionData_.projectilePos = VAdd(collisionData_.projectilePos, VScale(transform_->quaRot.GetForward(), 1000.0f));

		// �K�E�Z�̏Փ˔��肪�������Ԃ̃J�E���^�����Z
		attackSpecialPunchCollisionCnt_ += deltaTime;

	}

	// �ҋ@��ԂɑJ��
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);

		// �����蔻�������
		collisionData_.isProjectileAttack = false;

	}

}

void Player::UpdatePowerCharge(const float deltaTime)
{

	// �K�E�Z�ɑJ��
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::ATTACK_SPECIAL_PUNCH);
	}

}

void Player::UpdateHitHead(const float deltaTime)
{

	// �ҋ@��ԂɑJ��
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);
	}

}

void Player::UpdateHitBody(const float deltaTime)
{

	// �ҋ@��ԂɑJ��
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);
	}

}
