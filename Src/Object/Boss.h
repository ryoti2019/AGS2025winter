#pragma once
#include "../Component/BossAIComponent.h"
#include "EnemyBase.h"
#include "BossState.h"

class Boss : public EnemyBase
{

public:

	// ��ѓ���̍U���J�n�t���[��
	const float ATTACK_PROJECTILE_START_FRAME;

	// ��ѓ���̃_���[�W��
	const int ATTACK_PROJECTILE_DAMAGE;

	// ��ѓ���̍U������������
	const float ATTACK_PROJECTILE_COLLISION_TIME;

	// �A�j���[�V�����R���g���[���[�ɓn������
	std::string ANIM_DATA_KEY[static_cast<int>(BossState::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN",
		"ATTACK_PUNCH",
		"ATTACK_KICK",
		"ATTACK_PROJECTILE"
	};

	enum class EffectData
	{
		NONE,
		PROJECTILE,
		MAX
	};

	Boss(const VECTOR& pos, const json& data);

	~Boss() = default;

	void Init(const VECTOR& pos)override;
	void Update(const float deltaTime)override;

	// ��ԑJ��
	void ChangeState(const BossState state);

	// �U�������擾
	const bool GetAttackState()const override;

	// �U����ނ��擾
	const std::vector<int> GetTotalAttackTypes()const;

	// �U���̃q�b�g����
	void AttackHit(const int damage, const int state)override;

	// ��ѓ���̃q�b�g����
	void ProjectileHit(const int damage)override;

	// ���̏�Ԃ��擾
	const int GetState()const override { return static_cast<int>(state_); }

private:

	// AI�R���|�[�l���g
	std::unique_ptr<BossAIComponent> aiComponent_;

	// �U�����̏��
	const std::vector<BossState>& attackState_ =
	{
		{BossState::ATTACK_PUNCH},
		{BossState::ATTACK_KICK},
		{BossState::ATTACK_PROJECTILE}
	};

	// �U�����󂯂Ă�����
	const std::vector<BossState>& hitState_ =
	{
		{BossState::HIT_HEAD},
		{BossState::HIT_BODY},
		{BossState::HIT_FLY},
		{BossState::HIT_FLINCH_UP},
		{BossState::HIT_KNOCK_BACK},
		{BossState::KIP_UP}
	};

	// ���
	BossState state_;

	// ��ѓ���̔��ł�������
	VECTOR projectileDir_;

	// ��ѓ���̏Փ˔��肪�������Ԃ̃J�E���^
	float projectileCollisionCnt_;

	// ��ԑJ��
	std::unordered_map<BossState, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();
	void ChangePunch();
	void ChangeKick();
	void ChangeProjectile();
	void ChangeHitHead();
	void ChangeHitBody();
	void ChangeHitFly();
	void ChangeHitFlinchUp();
	void ChangeHitKnockback();
	void ChangeHitKipUp();
	void ChangeDeath();


	// ��Ԃ̍X�V
	std::function<void(const float deltaTime)> stateUpdate_;
	void UpdateIdle(const float deltaTime);
	void UpdateRun(const float deltaTime);
	void UpdatePunch(const float deltaTime);
	void UpdateKick(const float deltaTime);
	void UpdateProjectile(const float deltaTime);
	void UpdateHitHead(const float deltaTime);
	void UpdateHitBody(const float deltaTime);
	void UpdateHitFly(const float deltaTime);
	void UpdateHitFlinchUp(const float deltaTime);
	void UpdateHitKnockback(const float deltaTime);
	void UpdateHitKipUp(const float deltaTime);
	void UpdateDeath(const float deltaTime);

	// �@�\�̏�����
	void InitFunction()override;

	// �p�����[�^�̏�����
	void InitParameter()override;

	// �A�j���[�V�����̏�����
	void InitAnimation()override;

	// �G�t�F�N�g�̏�����
	void InitEffect(void)override;

	// �֐��|�C���^�̏�����
	void InitFunctionPointer()override;

	// ImGui�̃f�o�b�O�`��̍X�V
	void UpdateDebugImGui()override;

	// �A�j���[�V�����̃t���[�����Œ�
	void AnimationFrame()override;

	// ��ѓ���̍X�V
	void Projectile(const float deltaTime);

};

