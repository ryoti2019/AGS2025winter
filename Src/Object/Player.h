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

	struct ANIM_ACCEPT_TIME
	{

		// �A�j���[�V����
		STATE key;

		// �A�j���[�V�����̎�t�J�n����
		float animAcceptStartTime;

		// �A�j���[�V�����̎�t�I������
		float animAcceptEndTime;

	};

	Player(const VECTOR& pos, const json& data);

	~Player() = default;

	void Init(const VECTOR& pos) override;
	void Update(const float deltaTime) override;

	bool GetAttackState()override;

private:

	// ���͗p�R���g���[���[
	std::unique_ptr<InputController> inputController_;

	const std::vector<STATE> attackState_ = 
	{
		{STATE::JAB},
		{STATE::STRAIGHT},
		{STATE::HOOK},
		{STATE::LEFT_KICK},
		{STATE::RIGHT_KICK},
		{STATE::UPPER}
	};

	// ���
	STATE state_;

	// �A�j���[�V�����̎�t����
	std::vector<ANIM_ACCEPT_TIME> animAcceptTime_;

	// �W���u
	bool jab_;

	// �X�g���[�g
	bool straight_;

	// �t�b�N
	bool hook_;

	// ���L�b�N
	bool leftKick_;

	// �E�L�b�N
	bool rightKick_;

	// �U��4�i�K��
	bool upper_;

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

