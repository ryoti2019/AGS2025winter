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
		HOOK,
		LEFT_KICK,
		RIGHT_KICK,
		UPPER,
		MAX
	};

	enum class ATTACK_STATE
	{
		JAB,
		STRAIGHT,
		HOOK,
		LEFT_KICK,
		RIGHT_KICK,
		UPPER
	};

	// �A�j���[�V�����R���g���[���[�ɓn������
	std::string ANIM_DATA_KEY[static_cast<int>(STATE::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN",
		"JAB",
		"STRAIGHT",
		"HOOK",
		"LEFT_KICK",
		"RIGHT_KICK",
		"UPPER"
	};

	// �W���u��ł��Ă���Ԃɓ����n�߂�t���[����
	const float JAB_MOVE_START_FRAME;

	// �W���u��ł��Ă���Ԃɓ����I���t���[����
	const float JAB_MOVE_END_FRAME;

	// �X�g���[�g��ł��Ă���Ԃɓ����n�߂�t���[����
	const float STRAIGHT_MOVE_START_FRAME;

	// �X�g���[�g��ł��Ă���Ԃɓ����I���t���[����
	const float STRAIGHT_MOVE_END_FRAME;

	// �t�b�N��ł��Ă���Ԃɓ����n�߂�t���[����
	const float HOOK_MOVE_START_FRAME;

	// �t�b�N��ł��Ă���Ԃɓ����I���t���[����
	const float HOOK_MOVE_END_FRAME;

	// ���L�b�N��ł��Ă���Ԃɓ����n�߂�t���[����
	const float LEFT_KICK_MOVE_START_FRAME;

	// ���L�b�N��ł��Ă���Ԃɓ����I���t���[����
	const float LEFT_KICK_MOVE_END_FRAME;

	// �E�L�b�N��ł��Ă���Ԃɓ����n�߂�t���[����
	const float RIGHT_KICK_MOVE_START_FRAME;

	// �E�L�b�N��ł��Ă���Ԃɓ����I���t���[����
	const float RIGHT_KICK_MOVE_END_FRAME;

	Player(const VECTOR& pos, const json& data);

	~Player() = default;

	void Init(const VECTOR& pos) override;
	void Update(const float deltaTime) override;

	// �U�����̏�Ԃ����擾
	bool GetAttackState()override;

	// �R���{���̏�Ԃ����擾
	bool GetComboState();

private:

	// ���͗p�R���g���[���[
	std::unique_ptr<InputController> inputController_;

	// �U�����̏��
	const std::vector<STATE> attackState_ =
	{
		{STATE::JAB},
		{STATE::STRAIGHT},
		{STATE::HOOK},
		{STATE::LEFT_KICK},
		{STATE::RIGHT_KICK},
		{STATE::UPPER}
	};

	// �R���{���̔���
	const std::vector<STATE> comboState_=
	{
		{STATE::JAB},
		{ STATE::STRAIGHT },
		{ STATE::HOOK },
		{ STATE::LEFT_KICK },
		{ STATE::RIGHT_KICK }
	};

	// ���
	STATE state_;

	// �U������͂��Ă��邩
	std::map<STATE, bool> isCombo_;

	// ���̓J�E���^
	float acceptCnt_;

	// ��ԑJ��
	std::unordered_map<STATE, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();
	void ChangeJab();
	void ChangeStraight();
	void ChangeHook();
	void ChangeLeftKick();
	void ChangeRightKick();
	void ChangeUpper();

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

	// �@�\�̏�����
	void InitFunction()override;

	// �p�����[�^�̏�����
	void InitParameter()override;

	// �A�j���[�V�����̏�����
	void InitAnimation()override;

	// �֐��|�C���^�̏�����
	void InitFunctionPointer()override;

	// ImGui�̃f�o�b�O�`��̍X�V
	virtual void UpdateDebugImGui();

	// ��ԑJ��
	void ChangeState(STATE state);

	// �ړ�����
	void Move()override;

	// �U������
	void ComboAttack(const float deltaTime)override;

};

