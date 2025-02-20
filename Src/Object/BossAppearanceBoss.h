#pragma once
#include "EnemyBase.h"
#include "BossState.h"

class BossAppearanceBoss : public EnemyBase
{

public:

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
		"DEATH"
	};

	BossAppearanceBoss(const VECTOR& pos, const json& data);

	~BossAppearanceBoss()override = default;

	void Init(const VECTOR& pos)override;
	void Update(const float deltaTime)override;

private:

	// ���
	BossState state_;

	// �n�ʂɂ������̃t���O
	bool isLand_;

	// ��ԑJ��
	std::unordered_map<BossState, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeJumpIdle();
	void ChangeLand();

	// ��Ԃ̍X�V
	std::function<void(const float deltaTime)> stateUpdate_;
	void UpdateIdle(const float deltaTime);
	void UpdateJumpIdle(const float deltaTime);
	void UpdateLand(const float deltaTime);

	// �@�\�̏�����
	void InitComponent()override;

	// �p�����[�^�̏�����
	void InitParameter()override;

	// �A�j���[�V�����̏�����
	void InitAnimation()override;

	// �֐��|�C���^�̏�����
	void InitFunctionPointer()override;

	// ImGui�̃f�o�b�O�`��̍X�V
	void UpdateDebugImGui()override;

	// ��ԑJ��
	void ChangeState(const BossState state);

};

