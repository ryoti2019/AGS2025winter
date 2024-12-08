#pragma once
#include "ActorBase.h"

class EnemyBase : public ActorBase
{

public:

	// �p���`�̍U���J�n�t���[��
	float PUNCH_ATTACK_START_FRAME;

	// �p���`�̍U���I���t���[��
	float PUNCH_ATTACK_END_FRAME;

	// �L�b�N�̍U���J�n�t���[��
	float KICK_ATTACK_START_FRAME;

	// �L�b�N�̍U���I���t���[��
	float KICK_ATTACK_END_FRAME;

	// �N�[���^�C��
	const float COOL_TIME;

	// �v���C���[�����苗�����ꂽ��ړ����Ă��鋗��
	const float ACTIVATION_DISTANCE;

	// �ǂ�������ő�̎���
	const float TRACKING_MAX_TIME;

	EnemyBase(const VECTOR& pos, const json& data);

	virtual ~EnemyBase() = default;

	void Init();

	// �s�������߂����ǂ������擾
	const bool GetIsActionDecided()const { return isActionDecided_; }

	// �s�������߂����ǂ�����ݒ�
	void SetIsActionDecided(const bool isActionDecided) { isActionDecided_ = isActionDecided; }

	// �N�[���^�C�����擾
	const float GetCoolTime()const { return coolTime_; }

	// �N�[���^�C����ݒ�
	void SetCoolTime(const float coolTime) { coolTime_ = coolTime; }

	// �v���C���[�̍��W���擾
	std::optional<VECTOR> GetPlayerPos();

protected:

	// �s�������߂����ǂ���
	bool isActionDecided_;

	// �N�[���^�C��
	float coolTime_;

	// �ǂ������Ă��鎞��
	float trackingTime_;

private:

};

