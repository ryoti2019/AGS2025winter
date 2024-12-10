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
	float COOL_TIME;

	// �v���C���[�����苗�����ꂽ��ړ����Ă��鋗��
	float ACTIVATION_DISTANCE;

	// �ǂ�������ő�̎���
	float TRACKING_MAX_TIME;

	// ������Ԏ��̏�����̗�
	float HIT_FLY_UP_VEC_POW;

	// ������Ԏ��̈ړ���
	float HIT_FLY_MOVE_POW;

	// ��ɔ��ł����Ƃ��̏�����̗�
	float FLINCH_UP_UP_VEC_POW;

	// ������ɔ��ł����Ƃ��̏�����̗�
	float FLINCH_UP_UP_VEC_SMALL_POW;

	// ��ɔ��ł����Ƃ��̃X�s�[�h
	float FLINCH_UP_SPEED;

	// ��ɔ��ł����Ƃ��̏d�͂��ɂ����鋭��
	float FLINCH_UP_GRAVITY_SCALE;

	// ��ɔ��ł����Ƃ���X���̊p�x
	float FLINCH_UP_ANGLE_X;

	// �܂��������ł����Ƃ��������鍂��
	float KNOCK_BACK_HEIGHT_OFFSET;

	// �܂��������ł�������
	float KNOCK_BACK_TIME;

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

	// ���łɊp�x���ς���Ă��邩�ǂ���
	bool isChangeAngle_;

	// �G���܂��������ł����Ƃ��̃J�E���^
	float knockBackCnt_;

private:

};
