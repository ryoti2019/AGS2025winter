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
		MAX
	};

	// �A�j���[�V�����R���g���[���[�ɓn������
	std::string ANIM_DATA_KEY[static_cast<int>(STATE::MAX)] =
	{
		"NONE",
		"IDLE",
		"JAB",
		"STRAIGHT",
		"RUN"
	};

	Player(const VECTOR& pos);

	~Player() = default;

	void Init(const VECTOR& pos) override;
	void Update(const float deltaTime) override;

private:

	// ���͗p�R���g���[���[
	std::unique_ptr<InputController> inputController_;

	// ���
	STATE state_;

	// ��ԑJ��
	std::unordered_map<STATE, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();
	void ChangeJab();
	void ChangeStraight();

	// ��Ԃ̍X�V
	std::function<void()> stateUpdate_;
	void UpdateIdle();
	void UpdateRun();
	void UpdateJab();
	void UpdateStraight();

	// ��Ԃ̕`��
	std::function<void()> stateDraw_;
	void DrawIdle();
	void DrawRun();
	void DrawJab();
	void DrawStraight();

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
	void Attack()override;

};

