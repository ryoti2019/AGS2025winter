#pragma once
#include <DxLib.h>
#include <functional>
#include "ActorBase.h"

class Enemy : public ActorBase
{

public:

	enum class STATE
	{
		NONE,
		IDLE,
		RUN,
		PUNCH,
		KICK,
		HIT,
		HIT_FLY,
		KIP_UP,
		MAX
	};

	// �A�j���[�V�����R���g���[���[�ɓn������
	std::string ANIM_DATA_KEY[static_cast<int>(STATE::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN",
		"PUNCH",
		"KICK",
		"HIT",
		"HIT_FLY",
		"KIP_UP"
	};

	Enemy(const VECTOR& pos, const json& data);

	~Enemy() = default;

	void Init(const VECTOR& pos)override;
	void Update(const float deltaTime)override;

	// �U�������擾
	bool GetAttackState()override;

	// �U���̃q�b�g����
	void AttackHit()override;

	// �U���̃q�b�g�Ŕ��ł�������
	void AttackHitFly()override;

private:

	// ���
	STATE state_;

	// �s�������߂����ǂ���
	bool isActionDecided_;

	// ��ԑJ��
	std::unordered_map<STATE, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();
	void ChangePunch();
	void ChangeKick();
	void ChangeHit();
	void ChangeHitFly();
	void ChangeKipUp();

	// ��Ԃ̍X�V
	std::function<void()> stateUpdate_;
	void UpdateIdle();
	void UpdateRun();
	void UpdatePunch();
	void UpdateKick();
	void UpdateHit();
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
	void ChangeState(STATE state);

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

