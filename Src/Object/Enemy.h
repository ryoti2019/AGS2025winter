#pragma once
#include <DxLib.h>
#include <functional>
#include "../Component/AIComponent.h"
#include "ActorBase.h"
#include "EnemyState.h"
#include "Player.h"

class AIComponent;

class Enemy : public ActorBase
{

public:

	// �A�j���[�V�����R���g���[���[�ɓn������
	std::string ANIM_DATA_KEY[static_cast<int>(EnemyState::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN",
		"ATTACK_PUNCH",
		"ATTACK_KICK",
		"HIT_HEAD",
		"HIT_BODY",
		"HIT_FLY",
		"HIT_FLINCH_UP",
		"HIT_KNOCK_BACK",
		"KIP_UP",
		"DEATH"
	};

	// �p���`�̍U���J�n�t���[��
	const float PUNCH_ATTACK_START_FRAME;

	// �p���`�̍U���I���t���[��
	const float PUNCH_ATTACK_END_FRAME;

	// �L�b�N�̍U���J�n�t���[��
	const float KICK_ATTACK_START_FRAME;

	// �L�b�N�̍U���I���t���[��
	const float KICK_ATTACK_END_FRAME;

	// �N�[���^�C��
	const float COOL_TIME;

	// �v���C���[�����苗�����ꂽ��ړ����Ă��鋗��
	const float ACTIVATION_DISTANCE;

	// ������Ԏ��̏�����̗�
	const float HIT_FLY_UP_VEC_POW;

	// ������Ԏ��̈ړ���
	const float HIT_FLY_MOVE_POW;

	// �ǂ�������ő�̎���
	const float TRACKING_MAX_TIME;

	// �܂��������ł�������
	const float KNOCK_BACK_TIME;

	// �܂��������ł����Ƃ��������鍂��
	const float KNOCK_BACK_HEIGHT_OFFSET;

	// ��ɔ��ł����Ƃ��̏�����̗�
	const float FLINCH_UP_UP_VEC_POW;

	// ������ɔ��ł����Ƃ��̏�����̗�
	const float FLINCH_UP_UP_VEC_SMALL_POW;

	// ��ɔ��ł����Ƃ��̃X�s�[�h
	const float FLINCH_UP_SPEED;

	// ��ɔ��ł����Ƃ���X���̊p�x
	const float FLINCH_UP_ANGLE_X;

	// ��ɔ��ł����Ƃ��̏d�͂��ɂ����鋭��
	const float FLINCH_UP_GRAVITY_SCALE;

	// �ړ����̓���������
	const float MOVE_RATE;

	Enemy(const VECTOR& pos, const json& data);

	~Enemy() = default;

	void Init(const VECTOR& pos)override;
	void Update(const float deltaTime)override;
	void Draw()override;

	// �U�������擾
	const bool GetAttackState()const override;

	// �U����ނ��擾
	const std::vector<int> GetTotalAttackTypes()const;

	// �U�����󂯂Ă����Ԃ��擾
	const bool GetHitState()const override;

	// �U���̃q�b�g����
	void AttackHit(const int damage, const int state)override;

	// ���̏�Ԃ��擾
	const int GetState()const override { return static_cast<int>(state_); }

	// �_���[�W�ʂ��擾
	const int GetDamage()const override { return damage_; }

	// �s�������߂����ǂ������擾
	const bool GetIsActionDecided()const { return isActionDecided_; }

	// �s�������߂����ǂ�����ݒ�
	void SetIsActionDecided(const bool isActionDecided) { isActionDecided_ = isActionDecided; }

	// �N�[���^�C�����擾
	const float GetCoolTime()const { return coolTime_; }

	// �N�[���^�C����ݒ�
	void SetCoolTime(const float coolTime) { coolTime_ = coolTime; }

	// �v���C���[�̍��W���擾
	std::optional<VECTOR> GetPlayerPos();

	// ��ԑJ��
	void ChangeState(EnemyState state);

private:

	// ���͗p�̃R���|�[�l���g
	std::unique_ptr<AIComponent> aiComponent_;

	// �U�����̏��
	const std::vector<EnemyState>& attackState_ =
	{
		{EnemyState::ATTACK_PUNCH},
		{EnemyState::ATTACK_KICK}
	};

	// �U�����󂯂Ă�����
	const std::vector<EnemyState>& hitState_ =
	{
		{EnemyState::HIT_HEAD},
		{EnemyState::HIT_BODY},
		{EnemyState::HIT_FLY},
		{EnemyState::HIT_FLINCH_UP},
		{EnemyState::HIT_KNOCK_BACK},
		{EnemyState::KIP_UP}
	};

	// ���Ƀq�b�g����v���C���[�̍U��
	const std::vector<PlayerState>& hitHeadState_ =
	{
		{PlayerState::ATTACK_JAB},
		{PlayerState::ATTACK_STRAIGHT},
		{PlayerState::ATTACK_HOOK},
	};

	// �̂Ƀq�b�g����v���C���[�̍U��
	const std::vector<PlayerState>& hitBodyState_ =
	{
		{PlayerState::ATTACK_LEFT_KICK}
	};

	// ������΂����v���C���[�̍U��
	const std::vector<PlayerState>& hitFlyState_ =
	{
		{PlayerState::ATTACK_RIGHT_KICK},
		{PlayerState::ATTACK_SPECIAL_PUNCH}
	};

	// ��ɔ�΂����v���C���[�̍U��
	const std::vector<PlayerState>& hitFlinchUpState_ =
	{
		{PlayerState::ATTACK_UPPER}
	};

	// �󒆂ɕ���������v���C���[�̍U��
	const std::vector<PlayerState>& hitAirState_ =
	{
		{PlayerState::ATTACK_JAB},
		{PlayerState::ATTACK_STRAIGHT},
		{PlayerState::ATTACK_HOOK},
		{PlayerState::ATTACK_LEFT_KICK}
	};

	// �^��������΂����v���C���[�̍U��
	const std::vector<PlayerState>& hitKnockBackState_ =
	{
		{PlayerState::ATTACK_CHARGE_PUNCH}
	};

	// ���̏�Ŏ��ʂƂ��̃v���C���[�̍U��
	const std::vector<PlayerState>& deathState_ =
	{
		{PlayerState::ATTACK_JAB},
		{PlayerState::ATTACK_STRAIGHT},
		{PlayerState::ATTACK_HOOK},
		{PlayerState::ATTACK_LEFT_KICK},
		{PlayerState::ATTACK_UPPER}
	};

	// �������Ŏ��ʂƂ��̃v���C���[�̍U��
	const std::vector<PlayerState>& hitFlyDeathState_ =
	{
		{PlayerState::ATTACK_RIGHT_KICK}
	};

	// ���
	EnemyState state_;

	// �s�������߂����ǂ���
	bool isActionDecided_;

	// ���łɊp�x���ς���Ă��邩�ǂ���
	bool isChangeAngle_;

	// �N�[���^�C��
	float coolTime_;

	// �ǂ������Ă��鎞��
	float trackingTime_;

	// �G���܂��������ł����Ƃ��̃J�E���^
	float knockBackCnt_;

	// ��ԑJ��
	std::unordered_map<EnemyState, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();
	void ChangePunch();
	void ChangeKick();
	void ChangeHitHead();
	void ChangeHitBody();
	void ChangeHitFly();
	void ChangeHitFlinchUp();
	void ChangeHitKnockBack();
	void ChangeKipUp();
	void ChangeDeath();

	// ��Ԃ̍X�V
	std::function<void(const float deltaTime)> stateUpdate_;
	void UpdateIdle(const float deltaTime);
	void UpdateRun(const float deltaTime);
	void UpdatePunch(const float deltaTime);
	void UpdateKick(const float deltaTime);
	void UpdateHitHead(const float deltaTime);
	void UpdateHitBody(const float deltaTime);
	void UpdateHitFly(const float deltaTime);
	void UpdateHitFlinchUp(const float deltaTime);
	void UpdateHitKnockBack(const float deltaTime);
	void UpdateKipUp(const float deltaTime);
	void UpdateDeath(const float deltaTime);

	// �@�\�̏�����
	void InitFunction()override;

	// �p�����[�^�̏�����
	void InitParameter()override;

	// �A�j���[�V�����̏�����
	void InitAnimation()override;

	// �֐��|�C���^�̏�����
	void InitFunctionPointer()override;

	// ImGui�̃f�o�b�O�`��̍X�V
	void UpdateDebugImGui()override;

	// �A�j���[�V�����̃t���[�����Œ�
	void AnimationFrame()override;

	// �ǂ̃q�b�g�A�j���[�V�������`�F�b�N����
	void AttackHitCheck(const int state);

	// HP��0�ɂȂ����玀�S�A�j���[�V�����ɑJ��
	void DeathAnim(int state)override;

};

