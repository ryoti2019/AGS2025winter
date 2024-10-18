#pragma once
#include <DxLib.h>
#include <functional>
#include "ActorBase.h"
#include "Common/InputController.h"

class InputController;

class Player : public ActorBase
{

public:

	enum class STATE
	{
		NONE,
		IDLE,
		RUN,
		MAX,
	};

	// �A�j���[�V�����R���g���[���[�ɓn������
	std::string ANIM_DATA_KEY[static_cast<int>(STATE::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN"
	};

	using ActorBase::ActorBase;

	Player(const VECTOR& pos);

	~Player() = default;

	void Init(const VECTOR& pos) override;
	void Update(const float deltaTime) override;

private:

	// ���̓R���g���[���[
	//InputController inputController_;

	// ���
	STATE state_;

	// �A�j���[�V�����R���g���[���[�ɏ�Ԃ𕶎��^�œn���ϐ�
	std::string key_;
	std::string preKey_;

	// ��ԑJ��
	std::unordered_map<STATE, std::function<void(void)>> stateChange_;
	void ChangeIdle(void);
	void ChangeRun(void);

	// ��Ԃ̍X�V
	std::function<void(void)> stateUpdate_;
	void UpdateIdle(void);
	void UpdateRun(void);

	// ��Ԃ̕`��
	std::function<void(void)> stateDraw_;
	void DrawIdle(void);
	void DrawRun(void);

	// ��ԑJ��
	void ChangeState(STATE state);

};

