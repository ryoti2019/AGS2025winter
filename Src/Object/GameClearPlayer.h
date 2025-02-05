#pragma once
#include "ActorBase.h"
#include "PlayerState.h"

class GameClearPlayer : public ActorBase
{

public:

	// �A�j���[�V�����R���g���[���[�ɓn������
	std::string ANIM_DATA_KEY[static_cast<int>(PlayerState::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN",
		"ATTACK_JAB",
		"ATTACK_STRAIGHT",
		"ATTACK_HOOK",
		"ATTACK_LEFT_KICK",
		"ATTACK_RIGHT_KICK",
		"ATTACK_UPPER",
		"ATTACK_CHARGE_PUNCH",
		"ATTACK_SPECIAL_PUNCH",
		"POWER_CHARGE",
		"EVASION",
		"HIT_HEAD",
		"HIT_BODY",
		"DEATH",
		"TITLE_IDLE",
		"VICTORY"
	};

	GameClearPlayer(const VECTOR& pos, const json& data);

	~GameClearPlayer()override = default;

	void Init(const VECTOR& pos)override;
	void Update(const float deltaTime)override;

private:

	// ���
	PlayerState state_;

	// �@�\�̏�����
	void InitFunction()override;

	// �p�����[�^�̏�����
	void InitParameter()override;

	// �A�j���[�V�����̏�����
	void InitAnimation()override;

	// ��ԑJ��
	void ChangeState(const PlayerState state);

};

