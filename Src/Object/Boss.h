#pragma once
#include "../Component/BossAIComponent.h"
#include "EnemyBase.h"
#include "BossState.h"
#include "PlayerState.h"

class Boss : public EnemyBase
{

public:

	// ��ѓ���̍U���J�n�t���[��
	const float ATTACK_PROJECTILE_START_FRAME;

	// ��ѓ���̃_���[�W��
	const int ATTACK_PROJECTILE_DAMAGE;

	// ��ѓ���̍U������������
	const float ATTACK_PROJECTILE_COLLISION_TIME;

	// �������U�����Ă��鋗��
	const float LONG_RANGE_ATTACK_DISTANCE;

	// �X�[�p�[�A�[�}�[HP�̍ő�l
	const int SUPER_ARMOR_HP;

	// �X�[�p�[�A�[�}�[HP���񕜂���܂ł̃N�[���^�C��
	const int SUPER_ARMOR_HP_COOL_TIME;

	// �G�𐶐�����Ƃ��̃N�[���^�C��
	const float CREATE_ENEMY_COOL_TIME;

	// �G�𐶐�����t���[��
	const float CREATE_ENEMY_FRAME;

	// �A�j���[�V�����R���g���[���[�ɓn������
	std::string ANIM_DATA_KEY[static_cast<int>(BossState::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN",
		"ATTACK_PUNCH",
		"ATTACK_KICK",
		"ATTACK_PROJECTILE",
		"HIT_HEAD",
		"HIT_BODY",
		"HIT_FLY",
		"HIT_FLINCH_UP",
		"HIT_KNOCK_BACK",
		"KIP_UP",
		"JUMP_IDLE",
		"LAND",
		"CALL",
		"DEATH"
	};

	// �G�t�F�N�g�f�[�^
	enum class EffectData
	{
		NONE,
		PROJECTILE,
		HIT,
		MAX
	};

	Boss(const VECTOR& pos, const json& data);

	~Boss()override = default;

	void Init(const VECTOR& pos)override;
	void Update(const float deltaTime)override;
	void Draw(const float deltaTime)override;

	// ��ԑJ��
	void ChangeState(const BossState state);

	// �U�������擾
	const bool GetAttackState()const override;

	// �ߐڍU�����擾
	const bool GetCloseRangeAttackState()const override;

	// �U����ނ��擾
	const std::vector<int> GetTotalAttackTypes()const;

	// �U�����󂯂Ă����Ԃ��擾
	const bool GetHitState()const override;

	// �X�[�p�[�A�[�}�[��Ԃ����擾
	const bool GetIsSuperArmor()const override;

	// �U���̃q�b�g����
	void AttackHit(const int damage, const int state)override;

	// ��ѓ���̃q�b�g����
	void ProjectileHit(const int damage)override;

	// ���̏�Ԃ��擾
	const int GetState()const override { return static_cast<int>(state_); }

	// �_���[�W�ʂ��擾
	const int GetDamage()const { return damage_; }

	// �G�𐶐��������ǂ����̃t���O���擾
	const bool GetIsCreateEnemy()const { return isCreateEnemy_; }

	// �G�𐶐��������ǂ����̃t���O��ݒ�
	void SetIsCreateEnemy(const bool isCreateEnemy) { isCreateEnemy_ = isCreateEnemy; }

	// �G�𐶐�����Ƃ��̃N�[���^�C���̃J�E���^
	const float GetCreateEnemyCoolTimeCnt()const { return createEnemyCoolTimeCnt_; }

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

	// �ߐڍU�����̏��
	const std::vector<BossState> closeRangeAttackState_ =
	{
		{BossState::ATTACK_PUNCH},
		{BossState::ATTACK_KICK}
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
		{PlayerState::ATTACK_RIGHT_KICK},
	};

	// ���
	BossState state_;

	// ��ѓ���̔��ł�������
	VECTOR projectileDir_;

	// ��ѓ���̏Փ˔��肪�������Ԃ̃J�E���^
	float projectileCollisionCnt_;

	// �X�[�p�[�A�[�}�[��HP
	int superArmorHp_;

	// �X�[�p�[�A�[�}�[���񕜒��̎��̐��l
	float superArmorRecoveryHp_;

	// �X�[�p�[�A�[�}�[���񕜂���܂ł̃N�[���^�C���̃J�E���^
	float superArmorCoolTimeCnt_;

	// �G�𐶐�����Ƃ��̃N�[���^�C���̃J�E���^
	float createEnemyCoolTimeCnt_;

	// ��ѓ���̉�
	int projectileSE_;
	
	// �G�𐶐��������ǂ����̃t���O
	bool isCreateEnemy_;

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
	void ChangeKipUp();
	void ChangeCall();
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
	void UpdateKipUp(const float deltaTime);
	void UpdateCall(const float deltaTime);
	void UpdateDeath(const float deltaTime);

	// �@�\�̏�����
	void InitFunction()override;

	// �p�����[�^�̏�����
	void InitParameter()override;

	// �A�j���[�V�����̏�����
	void InitAnimation()override;

	// �G�t�F�N�g�̏�����
	void InitEffect(void)override;

	// BGM��SE�̏�����
	void InitBGMAndSE()override;

	// �֐��|�C���^�̏�����
	void InitFunctionPointer()override;

	// ImGui�̃f�o�b�O�`��̍X�V
	void UpdateDebugImGui()override;

	// �A�j���[�V�����̃t���[�����Œ�
	void AnimationFrame()override;

	// �ǂ̃q�b�g�A�j���[�V�������`�F�b�N����
	void AttackHitCheck(const int state);

	// ��ѓ���̍X�V
	void Projectile(const float deltaTime);

	// �X�[�p�[�A�[�}�[��HP�����炷
	void SubSuperArmorHp(const int superArmorHp);

	// �X�[�p�[�A�[�}�[��HP���񕜂���܂ł̃N�[���^�C��
	void ResetSuperArmorCoolTime(const float deltaTime);

	// HP��0�ɂȂ����玀�S�A�j���[�V�����ɑJ��
	void DeathAnim(int state)override;

};