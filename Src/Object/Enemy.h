#pragma once
#include <DxLib.h>
#include <functional>
#include "ActorBase.h"
#include "EnemyState.h"
#include "Player.h"

class Enemy : public ActorBase
{

public:

	// �A�j���[�V�����R���g���[���[�ɓn������
	std::string ANIM_DATA_KEY[static_cast<int>(EnemyState::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN",
		"PUNCH",
		"KICK",
		"HIT_HEAD",
		"HIT_BODY",
		"HIT_FLY",
		"FLINCH_UP",
		"KIP_UP"
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

	// ������Ԏ��̈ړ���
	const float HIT_FLY_MOVE_POW;

	// �ǂ�������ő�̎���
	const float TRACKING_MAX_TIME;

	Enemy(const VECTOR& pos, const json& data);

	~Enemy() = default;

	void Init(const VECTOR& pos)override;
	void Update(const float deltaTime)override;
	void Draw()override;

	// �U�������擾
	bool GetAttackState()override;

	// �U����ނ��擾
	const std::vector<int> GetToatlAttackTypes()const
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

	// �U�����󂯂Ă����Ԃ��擾
	bool GetHitState()override;

	// �U���̃q�b�g����
	void AttackHit(const int damage, const int state)override;

	// ���̏�Ԃ��擾
	int GetState()override { return static_cast<int>(state_); }

	// �_���[�W�ʂ��擾
	int GetDamage()override { return damage_; }

private:

	// �U�����̏��
	const std::vector<EnemyState>& attackState_ =
	{
		{EnemyState::PUNCH},
		{EnemyState::KICK}
	};

	// �U�����󂯂Ă�����
	const std::vector<EnemyState>& hitState_ =
	{
		{EnemyState::HIT_HEAD},
		{EnemyState::HIT_BODY},
		{EnemyState::HIT_FLY},
		{EnemyState::FLINCH_UP},
		{EnemyState::KIP_UP}
	};

	// ���Ƀq�b�g����v���C���[�̍U��
	const std::vector<PlayerState>& hitHeadState_ =
	{
		{PlayerState::JAB},
		{PlayerState::STRAIGHT},
		{PlayerState::HOOK},
	};

	// �̂Ƀq�b�g����v���C���[�̍U��
	const std::vector<PlayerState>& hitBodyState_ =
	{
		{PlayerState::LEFT_KICK}
	};

	// ������΂����v���C���[�̍U��
	const std::vector<PlayerState>& hitFlyState_ =
	{
		{PlayerState::RIGHT_KICK}
	};

	// ��ɔ�΂����v���C���[�̍U��
	const std::vector<PlayerState>& hitFlinchUpState_ =
	{
		{PlayerState::UPPER}
	};

	// ���
	EnemyState state_;

	// �s�������߂����ǂ���
	bool isActionDecided_;

	// �N�[���^�C��
	float coolTime_;

	// �ǂ������Ă��鎞��
	float trackingTime_;

	// ��ԑJ��
	std::unordered_map<EnemyState, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();
	void ChangePunch();
	void ChangeKick();
	void ChangeHitHead();
	void ChangeHitBody();
	void ChangeHitFly();
	void ChangeFlinchUp();
	void ChangeKipUp();

	// ��Ԃ̍X�V
	std::function<void(const float deltaTime)> stateUpdate_;
	void UpdateIdle(const float deltaTime);
	void UpdateRun(const float deltaTime);
	void UpdatePunch(const float deltaTime);
	void UpdateKick(const float deltaTime);
	void UpdateHitHead(const float deltaTime);
	void UpdateHitBody(const float deltaTime);
	void UpdateHitFly(const float deltaTime);
	void UpdateFlinchUp(const float deltaTime);
	void UpdateKipUp(const float deltaTime);

	// �@�\�̏�����
	void InitFunction()override;

	// �p�����[�^�̏�����
	void InitParameter()override;

	// �A�j���[�V�����̏�����
	void InitAnimation()override;

	// �֐��|�C���^�̏�����
	void InitFunctionPointer()override;

	// ��ԑJ��
	void ChangeState(EnemyState state);

	// �ǂ̍s�������邩���߂�
	void SelsectAction(const float deltaTime);

	// �ړ�����
	void Move()override;

	// �U������
	void Attack(const float deltaTime)override;

	// �A�j���[�V�����̃t���[�����Œ�
	void AnimationFrame()override;

	// �v���C���[�̍��W���擾
	std::optional<VECTOR> GetPlayerPos();

	// �ǂ̃q�b�g�A�j���[�V�������`�F�b�N����
	void AttackHitCheck(const int state);

};

