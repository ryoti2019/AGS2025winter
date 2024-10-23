#pragma once
#include <DxLib.h>
#include <functional>
#include "ActorBase.h"
#include "Common/InputController.h"

class Player : public ActorBase
{

public:

	enum class STATE
	{
		NONE,
		IDLE,
		RUN,
		JAB,
		STRAIGHT,
		KICK,
		UPPER,
		MAX
	};

	// �A�j���[�V�����R���g���[���[�ɓn������
	std::string ANIM_DATA_KEY[static_cast<int>(STATE::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN",
		"JAB",
		"STRAIGHT",
		"KICK",
		"UPPER"
	};

	Player(const VECTOR& pos, const json& data);

	~Player() = default;

	void Init(const VECTOR& pos) override;
	void Update(const float deltaTime) override;

private:

	// ���͗p�R���g���[���[
	std::unique_ptr<InputController> inputController_;

	// ���
	STATE state_;

	// �U��1�i�K��
	bool jab_;

	// �U��2�i�K��
	bool straight_;

	// �U��3�i�K��
	bool kick_;

	// �U��4�i�K��
	bool upper_;

	// ��ԑJ��
	std::unordered_map<STATE, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();
	void ChangeJab();
	void ChangeStraight();
	void ChangeKick();
	void ChangeUpper();

	// ��Ԃ̍X�V
	std::function<void()> stateUpdate_;
	void UpdateIdle();
	void UpdateRun();
	void UpdateJab();
	void UpdateStraight();
	void UpdateKick();
	void UpdateUpper();

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

	// �U����������
	bool AttackState()override;

};

