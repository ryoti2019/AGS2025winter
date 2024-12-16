#pragma once
#include <DxLib.h>
#include <functional>
#include "../Component/InputComponent.h"
#include "../Manager/CollisionTypes.h"
#include "ActorBase.h"
#include "PlayerState.h"
#include "EnemyState.h"
#include "BossState.h"

class InputComponent;

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
		"ATTACK_SPECIAL_PUNCH",
		"POWER_CHARGE",
		"HIT_HEAD",
		"HIT_BODY"
	};

	// �W���u�̍U���J�n�t���[��
	const float ATTACK_JAB_START_FRAME;

	// �W���u�̍U���I���t���[��
	const float ATTACK_JAB_END_FRAME;

	// �W���u�̃_���[�W��
	const int ATTACK_JAB_DAMAGE;

	// �X�g���[�g�̍U���J�n�t���[��
	const float ATTACK_STRAIGHT_START_FRAME;

	// �X�g���[�g�̍U���I���t���[��
	const float ATTACK_STRAIGHT_END_FRAME;

	// �X�g���[�g�̃_���[�W��
	const int ATTACK_STRAIGHT_DAMAGE;

	// �t�b�N�̍U���J�n�t���[��
	const float ATTACK_HOOK_START_FRAME;

	// �t�b�N�̍U���I���t���[��
	const float ATTACK_HOOK_END_FRAME;

	// �t�b�N�̃_���[�W��
	const int ATTACK_HOOK_DAMAGE;

	// ���L�b�N�̍U���J�n�t���[��
	const float ATTACK_LEFT_KICK_START_FRAME;

	// ���L�b�N�̍U���I���t���[��
	const float ATTACK_LEFT_KICK_END_FRAME;

	// ���L�b�N�̃_���[�W��
	const int ATTACK_LEFT_KICK_DAMAGE;

	// �E�L�b�N�̍U���J�n�t���[��
	const float ATTACK_RIGHT_KICK_START_FRAME;

	// �E�L�b�N�̍U���I���t���[��
	const float ATTACK_RIGHT_KICK_END_FRAME;

	// �E�L�b�N�̃_���[�W��
	const int ATTACK_RIGHT_KICK_DAMAGE;

	// �A�b�p�[�̍U���J�n�t���[��
	const float ATTACK_UPPER_START_FRAME;

	// �A�b�p�[�̍U���I���t���[��
	const float ATTACK_UPPER_END_FRAME;

	// �A�b�p�[�̃_���[�W��
	const int ATTACK_UPPER_DAMAGE;

	// ���߃p���`�̍U���J�n�t���[��
	const float ATTACK_CHARGE_PUNCH_START_FRAME;

	// ���߃p���`�̍U���I���t���[��
	const float ATTACK_CHARGE_PUNCH_END_FRAME;

	// ���߃p���`�̃_���[�W��
	const int ATTACK_CHARGE_PUNCH_DAMAGE;

	// ���߃p���`���o�����߂Ƀ{�^����������
	const float ATTACK_CHARGE_PUNCH_TIME;

	// �K�E�Z�̍U���J�n�t���[��
	const float ATTACK_SPECIAL_PUNCH_START_FRAME;

	// �K�E�Z�̃_���[�W��
	const int ATTACK_SPECIAL_PUNCH_DAMAGE;

	// �K�E�Z�̍U������������
	const float ATTACK_SPECIAL_PUNCH_COLLISION_TIME;

	Player(const VECTOR& pos, const json& data);

	~Player() = default;

	void Init(const VECTOR& pos) override;
	void Update(const float deltaTime) override;
	void Draw(const float deltaTime)override;

	// �U�����̏�Ԃ����擾
	const bool GetAttackState()const override;

	// �U����ނ��擾
	const std::vector<int> GetTotalAttackTypes()const;

	// �U�����󂯂Ă����Ԃ����擾
	const bool GetHitState()const override;

	// �X�[�p�[�A�[�}�[��Ԃ����擾
	const bool GetSuperArmorState()const override;

	// �R���{���̏�Ԃ����擾
	bool GetComboState();

	// �U���̃q�b�g����
	void AttackHit(const int damage, const int type)override;

	// ��ѓ���̃q�b�g����
	void ProjectileHit(const int damage)override;

	// ���̏�Ԃ��擾
	const int GetState()const override { return static_cast<int>(state_); }

	// �_���[�W�ʂ��擾
	const int GetDamage()const override { return damage_; }

	// ���ߍU���̃J�E���^���擾
	float GetChargeCnt()const { return attackChargePunchCnt_; };

	// ���ߍU���̃J�E���^��ݒ�
	void SetChargeCnt(const float cnt) { attackChargePunchCnt_ = cnt; }

	// ���ߍU���̃J�E���^�����Z
	void AddChargeCnt(const float cnt) { attackChargePunchCnt_ += cnt; }

	// �U������͂��Ă��邩�擾
	const std::map<PlayerState, bool>& GetIsCombo()const { return isCombo_; }

	// �U������͂��Ă��邩�ݒ�
	void SetIsCombo(int i, bool isCombo)
	{
		isCombo_[static_cast<PlayerState>(i)] = isCombo;
	}

	// ��ԑJ��
	void ChangeState(const PlayerState state);

private:

	// ���͗p�̃R���|�[�l���g
	std::unique_ptr<InputComponent> inputComponent_;

	// �U�����̏��
	const std::vector<PlayerState> attackState_ =
	{
		{PlayerState::ATTACK_JAB},
		{PlayerState::ATTACK_STRAIGHT},
		{PlayerState::ATTACK_HOOK},
		{PlayerState::ATTACK_LEFT_KICK},
		{PlayerState::ATTACK_RIGHT_KICK},
		{PlayerState::ATTACK_UPPER},
		{PlayerState::ATTACK_CHARGE_PUNCH},
		{PlayerState::POWER_CHARGE},
		{PlayerState::ATTACK_SPECIAL_PUNCH}
	};

	// �R���{���̔���
	const std::vector<PlayerState> comboState_ =
	{
		{PlayerState::ATTACK_JAB},
		{PlayerState::ATTACK_STRAIGHT},
		{PlayerState::ATTACK_HOOK},
		{PlayerState::ATTACK_LEFT_KICK},
		{PlayerState::ATTACK_RIGHT_KICK},
	};

	// �U�����󂯂Ă�����
	const std::vector<PlayerState> hitState_ =
	{
		{PlayerState::HIT_HEAD},
		{PlayerState::HIT_BODY}
	};

	// �X�[�p�[�A�[�}�[���
	const std::vector<PlayerState> superArmorState_ =
	{
		{PlayerState::POWER_CHARGE},
		{PlayerState::ATTACK_SPECIAL_PUNCH}
	};

	// ���Ƀq�b�g����G�̍U��
	const std::vector<EnemyState> hitHeadEnemyState_ =
	{
		{EnemyState::ATTACK_PUNCH}
	};

	// ���Ƀq�b�g����{�X�̍U��
	const std::vector<BossState> hitHeadBossState_ =
	{
		{BossState::ATTACK_PUNCH}
	};

	// �̂Ƀq�b�g����G�̍U��
	const std::vector<EnemyState> hitBodyEnemyState_ =
	{
		{EnemyState::ATTACK_KICK}
	};

	// �̂Ƀq�b�g����G�̍U��
	const std::vector<BossState> hitBodyBossState_ =
	{
		{BossState::ATTACK_KICK}
	};

	// ���
	PlayerState state_;

	// �U������͂��Ă��邩
	std::map<PlayerState, bool> isCombo_;

	// ���߃p���`�̃J�E���^
	float attackChargePunchCnt_;

	// �K�E�Z�̏Փ˔��肪�������Ԃ̃J�E���^
	float attackSpecialPunchCollisionCnt_;

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
	void ChangeSpecialAttack();
	void ChangePowerCharge();
	void ChangeHitHead();
	void ChangeHitBody();

	// ��Ԃ̍X�V
	std::function<void(const float deltaTime)> stateUpdate_;
	void UpdateIdle(const float deltaTime);
	void UpdateRun(const float deltaTime);
	void UpdateJab(const float deltaTime);
	void UpdateStraight(const float deltaTime);
	void UpdateHook(const float deltaTime);
	void UpdateLeftKick(const float deltaTime);
	void UpdateRightKick(const float deltaTime);
	void UpdateUpper(const float deltaTime);
	void UpdateChargePunch(const float deltaTime);
	void UpdateSpecialAttack(const float deltaTime);
	void UpdatePowerCharge(const float deltaTime);
	void UpdateHitHead(const float deltaTime);
	void UpdateHitBody(const float deltaTime);

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

	// �ǂ̃q�b�g�A�j���[�V�������`�F�b�N����
	virtual void AttackHitCheck(const int type);

	// ��]����
	void Rotation();

};

