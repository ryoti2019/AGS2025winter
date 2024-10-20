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
		MAX
	};

	// �A�j���[�V�����R���g���[���[�ɓn������
	std::string ANIM_DATA_KEY[static_cast<int>(STATE::MAX)] =
	{
		"NONE",
		"IDLE",
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

	// �A�j���[�V�����R���g���[���[�ɏ�Ԃ𕶎��^�œn���ϐ�
	std::string key_;
	std::string preKey_;

	// ��ԑJ��
	std::unordered_map<STATE, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();

	// ��Ԃ̍X�V
	std::function<void()> stateUpdate_;
	void UpdateIdle();
	void UpdateRun();

	// ��Ԃ̕`��
	std::function<void()> stateDraw_;
	void DrawIdle();
	void DrawRun();

	// �@�\�̏�����
	void InitFunction();

	// �p�����[�^�̏�����
	void InitPrameter();

	// �A�j���[�V�����̏�����
	void InitAnimation();

	// �֐��|�C���^�̏�����
	void InitFunctionPointer();

	// ��ԑJ��
	void ChangeState(STATE state);

	// �ړ�����
	void Move()override;

};

