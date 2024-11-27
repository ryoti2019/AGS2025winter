#pragma once
#include <DxLib.h>
#include <functional>
#include "ActorBase.h"
#include "PlayerState.h"
#include "EnemyState.h"

class Player : public ActorBase
{

public:

	// �A�j���[�V�����R���g���[���[�ɓn������
	std::string ANIM_DATA_KEY[static_cast<int>(PlayerState::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN",
		"ATTACK_JAB",
		"ATTACK_STRAIGHT",
		"ATTACK_HOOK",
		"ATTACK_LEFT_KICK",
		"ATTACK_RIGHT_KICK",
		"ATTACK_UPPER",
		"ATTACK_CHARGE_PUNCH",
		"HIT_HEAD",
		"HIT_BODY"
	};

	// �W���u�̍U���J�n�t���[��
	const float JAB_ATTACK_START_FRAME;

	// �W���u�̍U���I���t���[��
	const float JAB_ATTACK_END_FRAME;

	// �W���u�̃_���[�W��
	const int JAB_DAMAGE;

	// �X�g���[�g�̍U���J�n�t���[��
	const float STRAIGHT_ATTACK_START_FRAME;

	// �X�g���[�g�̍U���I���t���[��
	const float STRAIGHT_ATTACK_END_FRAME;

	// �X�g���[�g�̃_���[�W��
	const int STRAIGHT_DAMAGE;

	// �t�b�N�̍U���J�n�t���[��
	const float HOOK_ATTACK_START_FRAME;

	// �t�b�N�̍U���I���t���[��
	const float HOOK_ATTACK_END_FRAME;

	// �t�b�N�̃_���[�W��
	const int HOOK_DAMAGE;

	// ���L�b�N�̍U���J�n�t���[��
	const float LEFT_KICK_ATTACK_START_FRAME;

	// ���L�b�N�̍U���I���t���[��
	const float LEFT_KICK_ATTACK_END_FRAME;

	// ���L�b�N�̃_���[�W��
	const int LEFT_KICK_DAMAGE;

	// �E�L�b�N�̍U���J�n�t���[��
	const float RIGHT_KICK_ATTACK_START_FRAME;

	// �E�L�b�N�̍U���I���t���[��
	const float RIGHT_KICK_ATTACK_END_FRAME;

	// �E�L�b�N�̃_���[�W��
	const int RIGHT_KICK_DAMAGE;

	// �A�b�p�[�̍U���J�n�t���[��
	const float UPPER_ATTACK_START_FRAME;

	// �A�b�p�[�̍U���I���t���[��
	const float UPPER_ATTACK_END_FRAME;

	// �A�b�p�[�̃_���[�W��
	const int UPPER_DAMAGE;

	// ���߃p���`�̍U���J�n�t���[��
	const float CHARGE_PUNCH_ATTACK_START_FRAME;

	// ���߃p���`�̍U���I���t���[��
	const float CHARGE_PUNCH_ATTACK_END_FRAME;

	// ���߃p���`�̃_���[�W��
	const int CHARGE_PUNCH_DAMAGE;

	// ���߃p���`���o�����߂Ƀ{�^����������
	const float CHARGE_TIME;

	Player(const VECTOR& pos, const json& data);

	~Player() = default;

	void Init(const VECTOR& pos) override;
	void Update(const float deltaTime) override;

	// �U�����̏�Ԃ����擾
	bool GetAttackState()override;

	// �U����ނ��擾
	const std::vector<int>& GetToatlAttackTypes()const
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

	// �R���{���̏�Ԃ����擾
	bool GetComboState();

	// �U���̃q�b�g����
	void AttackHit(const int damage, const int state)override;

	// ���̏�Ԃ��擾
	int GetState()const override { return static_cast<int>(state_); }

	// �_���[�W�ʂ��擾
	int GetDamage()const override { return damage_; }

private:

	// �U�����̏��
	const std::vector<PlayerState> attackState_ =
	{
		{PlayerState::ATTACK_JAB},
		{PlayerState::ATTACK_STRAIGHT},
		{PlayerState::ATTACK_HOOK},
		{PlayerState::ATTACK_LEFT_KICK},
		{PlayerState::ATTACK_RIGHT_KICK},
		{PlayerState::ATTACK_UPPER},
		{PlayerState::ATTACK_CHARGE_PUNCH}
	};

	// �R���{���̔���
	const std::vector<PlayerState> comboState_=
	{
		{PlayerState::ATTACK_JAB},
		{ PlayerState::ATTACK_STRAIGHT },
		{ PlayerState::ATTACK_HOOK },
		{ PlayerState::ATTACK_LEFT_KICK },
		{ PlayerState::ATTACK_RIGHT_KICK }
	};

	// �U�����󂯂Ă�����
	const std::vector<PlayerState> hitState_ =
	{
		{PlayerState::HIT_HEAD},
		{PlayerState::HIT_BODY}
	};

	// ���Ƀq�b�g����G�̍U��
	const std::vector<EnemyState> hitHeadState_ =
	{
		{EnemyState::ATTACK_PUNCH}
	};

	// �̂Ƀq�b�g����G�̍U��
	const std::vector<EnemyState> hitBodyState_ =
	{
		{EnemyState::ATTACK_KICK}
	};

	// ���
	PlayerState state_;

	// �U������͂��Ă��邩
	std::map<PlayerState, bool> isCombo_;

	// ���̓J�E���^
	float acceptCnt_;

	// ���߃p���`�̃J�E���^
	float chargeCnt_;

	// ��ԑJ��
	std::unordered_map<PlayerState, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();
	void ChangeJab();
	void ChangeStraight();
	void ChangeHook();
	void ChangeLeftKick();
	void ChangeRightKick();
	void ChangeUpper();
	void ChangeChargePunch();
	void ChangeHitHead();
	void ChangeHitBody();

	// ��Ԃ̍X�V
	std::function<void()> stateUpdate_;
	void UpdateIdle();
	void UpdateRun();
	void UpdateJab();
	void UpdateStraight();
	void UpdateHook();
	void UpdateLeftKick();
	void UpdateRightKick();
	void UpdateUpper();
	void UpdateChargePunch();
	void UpdateHitHead();
	void UpdateHitBody();

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

	// ��ԑJ��
	void ChangeState(const int state)override;

	// �ړ�����
	void Move()override;

	// �U������
	void Attack(const float deltaTime)override;

	// �U������Ƃ��̈ړ����]�̏���
	void MoveAndRotate();

	// �ǂ̃q�b�g�A�j���[�V�������`�F�b�N����
	virtual void AttackHitCheck(const int state);


};

