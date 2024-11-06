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
		HIT,
		HIT_FLY,
		MAX
	};

	// �A�j���[�V�����R���g���[���[�ɓn������
	std::string ANIM_DATA_KEY[static_cast<int>(STATE::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN",
		"HIT",
		"HIT_FLY"
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

	// ��ԑJ��
	std::unordered_map<STATE, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();
	void ChangeHit();
	void ChangeHitFly();

	// ��Ԃ̍X�V
	std::function<void()> stateUpdate_;
	void UpdateIdle();
	void UpdateRun();
	void UpdateHit();
	void UpdateHitFly();

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

	// �ړ�����
	void Move()override;

	// �U������
	void ComboAttack(const float deltaTime)override;

	// �U�����
	bool AttackState()override;

};

