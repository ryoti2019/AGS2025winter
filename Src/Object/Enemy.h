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

	Enemy(const VECTOR& pos, const json& data);

	~Enemy() = default;

	void Init(const VECTOR& pos)override;
	void Update(const float deltaTime)override;
	void Draw()override;

	// �U�������擾
	bool GetAttackState()override;

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
	const std::vector<EnemyState> attackState_ =
	{
		{EnemyState::PUNCH},
		{EnemyState::KICK}
	};

	// �U�����󂯂Ă�����
	const std::vector<EnemyState> hitState_ =
	{
		{EnemyState::HIT_HEAD},
		{EnemyState::HIT_BODY},
		{EnemyState::HIT_FLY},
		{EnemyState::KIP_UP}
	};

	// ���Ƀq�b�g����v���C���[�̍U��
	const std::vector<PlayerState> hitHeadState_ =
	{
		{PlayerState::JAB},
		{PlayerState::STRAIGHT},
		{PlayerState::HOOK}
	};

	// �̂Ƀq�b�g����v���C���[�̍U��
	const std::vector<PlayerState> hitBodyState_ =
	{
		{PlayerState::LEFT_KICK}
	};

	// ������΂����v���C���[�̍U��
	const std::vector<PlayerState> hitFlyState_ =
	{
		{PlayerState::RIGHT_KICK}
	};

	// ���
	EnemyState state_;

	// �s�������߂����ǂ���
	bool isActionDecided_;

	// �N�[���^�C��
	float coolTime_;

	// ��ԑJ��
	std::unordered_map<EnemyState, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();
	void ChangePunch();
	void ChangeKick();
	void ChangeHitHead();
	void ChangeHitBody();
	void ChangeHitFly();
	void ChangeKipUp();

	// ��Ԃ̍X�V
	std::function<void()> stateUpdate_;
	void UpdateIdle();
	void UpdateRun();
	void UpdatePunch();
	void UpdateKick();
	void UpdateHitHead();
	void UpdateHitBody();
	void UpdateHitFly();
	void UpdateKipUp();

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
	void SelsectAction();

	// �ړ�����
	void Move()override;

	// �U������
	void Attack()override;

	// �A�j���[�V�����̃t���[�����Œ�
	void AnimationFrame()override;

	// �v���C���[�̍��W���擾
	std::optional<VECTOR> GetPlayerPos();

};

