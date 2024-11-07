#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"
#include "../Object/Common/Transform.h"

class Player;

class Camera
{

public:

	// �J�����̏������W
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 500.0f, -500.0f };

	// �v���C���[����J�����̈ʒu�܂ł̑��΍��W
	static constexpr VECTOR LOCAL_P2C_POS = { 0.0f, 1500.0f,-3000.0f };

	// �v���C���[���璍���_�܂ł̑��΍��W
	static constexpr VECTOR LOCAL_P2T_POS = { 0.0f,800.0f,0.0f };

	// �G����J�����̈ʒu�܂ł̑��΍��W
	static constexpr VECTOR LOCAL_E2C_POS = { 0.0f, 500.0f,-400.0f };

	static constexpr VECTOR LOCAL_POS = { 0.0f,300.0f,-500.0f };

	// �G���璍���_�܂ł̑��΍��W
	static constexpr VECTOR LOCAL_E2T_POS = { 0.0f,0.0f,500.0f };

	static constexpr VECTOR LOCAL_LOCK_ON_F2C_POS = { 0.0f,300.0f,-500.0f };

	// �J�����̍���
	static constexpr float HEIGHT = 250.0f;

	// �Ǐ]�Ώۂ���J�����܂ł̋���
	static constexpr float DIS_FOLLOW2CAMERA = 150.0f;

	// �Ǐ]�Ώۂ��璍���_�܂ł̋���
	static constexpr float DIS_FOLLOW2TARGET = 350.0f;

	// �J�����X�s�[�h
	static constexpr float SPEED = 20.0f;

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_NEAR = 40.0f;

	// �J�����N���b�v�FFAR
	static constexpr float CAMERA_FAR = 50000.0f;

	// �J�������[�h
	enum class MODE
	{
		NONE,
		FIXED_POINT,	// ��_�J����
		FREE,			// �t���[���[�h
		FOLLOW,			// �Ǐ]���[�h
		LOCKON,			// ���b�N�I�����[�h
	};

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);
	void SetBeforeDrawLockOn(void);
	void Draw(void);
	void Release(void);

	// �v���C���[�������Ă���p�x
	void SetLazyAngles(const VECTOR angles);

	void SetBeforeDrawFollow(void);
	
	// �Ǐ]�Ώۂ̐ݒ�
	void SetPlayer(const Transform* follow);

	// �Ǐ]�Ώۂ̐ݒ�
	void SetEnemy(const Transform* follow);

	// ���b�N�I���̐ݒ�
	void SetLockOn(const bool lockOn);

	// �J�������[�h�̕ύX
	void ChangeMode(MODE mode);

	// �x����]
	void LazyRotation(void);

	//	���b�N�I������Y���̊p�x�𑫂�
	void AddLockOnAnglesY(float rad);

	// �X�e�[�W�̃��f��ID��ݒ�
	void SetStageID(const int modelId);

	Camera::MODE GetMode(void);

	VECTOR GetPos(void) const;

	VECTOR GetAngle(void) const;

	VECTOR GetTargetPos(void) const;

	Quaternion GetRotY(void) const;

	// ���b�N�I���̎擾
	bool GetLockOn(void);


private:

	// �Ǐ]�Ώ�
	const Transform* playerTransform_;
	const Transform* enemyTransform_;

	// �J�������[�h
	MODE mode_;

	// �J�����̈ʒu
	VECTOR pos_;

	// �ړ�����W
	VECTOR movedPos_;

	// �J�����̒����_
	VECTOR targetPos_;

	// �J�����̏����
	VECTOR cameraUp_;

	// ���b�N�I����p�R���g���[������p
	VECTOR lockOnAngles_;

	VECTOR goalCameraPos_;

	// �J�����̉�]
	VECTOR angle_;

	// �Ǐ]�Ώۂ���J�����܂ł̑��΍��W	
	VECTOR relativeCPos_;

	// �J�����̉�]
	Quaternion rotXY_;
	Quaternion rotY_;

	// Y���̃S�[���̊p�x
	Quaternion lazyGoalRotY_;

	Quaternion lockOnLook_;

	// �X�e�[�W��ID
	int stageId_;

	// ���艹
	int musicDecisionId_;

	// �}�E�X�̉��ړ�
	float rotPowY_;

	// �}�E�X�̏c�ړ�
	float rotPowX_;

	// ����������X�L�b�v�����Ȃ��悤�ɂ���J�E���^
	float operationCnt_;

	// SetLazyAngle�ɒl����������true
	bool isLazy_ = false;

	// ���b�N�I���̃t���O
	bool lockOn_;

	bool isNearLockOnTarget_;

	// �|���S���Ɠ������Ă��邩
	bool pHit_;

	// �J�����������ʒu�ɖ߂�
	void SetDefault(void);

	// �����_���L�����N�^�[�O���ʒu�ɐݒ�
	void SetTargetPosFollowForward(void);

	// �L�[�{�[�h�̑���
	void KeybordController(void);
	void KeybordLockOnContoroller(void);

	// �Q�[���p�b�h�̑���
	void GamePadController(void);
	void GamePadLockOnContoroller(void);

	// �X�e�[�W�Ƃ̓����蔻��
	void CollisionStage(void);

};

