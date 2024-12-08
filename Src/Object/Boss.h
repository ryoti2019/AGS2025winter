#pragma once
#include "../Component/BossAIComponent.h"
#include "EnemyBase.h"
#include "BossState.h"

class Boss : public EnemyBase
{

public:

	// �A�j���[�V�����R���g���[���[�ɓn������
	std::string ANIM_DATA_KEY[static_cast<int>(BossState::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN",
		"PUNCH",
		"KICK",
		"SONIC_BOOM"
	};

	Boss(const VECTOR& pos, const json& data);

	~Boss() = default;

	void Init(const VECTOR& pos)override;
	void Update(const float deltaTime)override;

	// ��ԑJ��
	void ChangeState(const BossState state);

	// �U�������擾
	const bool GetAttackState()const override;

	// �U����ނ��擾
	const std::vector<int> GetTotalAttackTypes()const;

	// ���̏�Ԃ��擾
	const int GetState()const override { return static_cast<int>(state_); }

private:

	// AI�R���|�[�l���g
	std::unique_ptr<BossAIComponent> aiComponent_;

	// �U�����̏��
	const std::vector<BossState>& attackState_ =
	{
		{BossState::PUNCH},
		{BossState::KICK}
	};

	// ���
	BossState state_;

	// ��ԑJ��
	std::unordered_map<BossState, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();
	void ChangePunch();
	void ChangeKick();
	void ChangeSonicBoom();

	// ��Ԃ̍X�V
	std::function<void(const float deltaTime)> stateUpdate_;
	void UpdateIdle(const float deltaTime);
	void UpdateRun(const float deltaTime);
	void UpdatePunch(const float deltaTime);
	void UpdateKick(const float deltaTime);
	void UpdateSonicBoom(const float deltaTime);

	// �@�\�̏�����
	void InitFunction()override;

	// �p�����[�^�̏�����
	void InitParameter()override;

	// �A�j���[�V�����̏�����
	void InitAnimation()override;

	// �֐��|�C���^�̏�����
	void InitFunctionPointer()override;

	// ImGui�̃f�o�b�O�`��̍X�V
	void UpdateDebugImGui()override;

	// �A�j���[�V�����̃t���[�����Œ�
	void AnimationFrame()override;

};

