#pragma once
#include <functional>
#include <DxLib.h>
#include "../Common/Quaternion.h"
#include "../Object/Common/Transform.h"

class Player;

class Camera
{

public:

	// �J�����̏������W
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 0.0f, 0.0f };

	// �v���C���[����J�����̈ʒu�܂ł̑��΍��W
	static constexpr VECTOR LOCAL_P2C_POS = { 0.0f, 1500.0f,-3000.0f };

	// �v���C���[���璍���_�܂ł̑��΍��W
	static constexpr VECTOR LOCAL_P2T_POS = { 0.0f,800.0f,0.0f };

	// �G����J�����̈ʒu�܂ł̑��΍��W
	static constexpr VECTOR LOCAL_E2C_POS = { 0.0f, 500.0f,-400.0f };

	// �G���璍���_�܂ł̑��΍��W
	static constexpr VECTOR LOCAL_E2T_POS = { 0.0f,0.0f,500.0f };

	// �J�����̍���
	static constexpr float HEIGHT = 250.0f;

	// �Ǐ]�Ώۂ���J�����܂ł̋���
	static constexpr float DIS_FOLLOW2CAMERA = 150.0f;

	// �Ǐ]�Ώۂ��璍���_�܂ł̋���
	static constexpr float DIS_FOLLOW2TARGET = 350.0f;

	// �J�����X�s�[�h
	static constexpr float SPEED = 100.0f;

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_NEAR = 40.0f;

	// �J�����N���b�v�FFAR
	static constexpr float CAMERA_FAR = 1000000.0f;

	// �K�E�Z���̃J��������������
	static constexpr float SPECIAL_MOVE_MAX_TIME = 2.0f;

	// �����蔻��̔��a
	static constexpr float COLLISION_RADIUS = 100.0f;

	// �J�������[�h
	enum class MODE
	{
		NONE,
		FIXED_POINT,	// ��_�J����
		FREE,			// �t���[���[�h
		TITLE,			// �^�C�g���V�[�����[�h
		FOLLOW,			// �Ǐ]���[�h
		LOCKON,			// ���b�N�I�����[�h
		SPECIAL,		// �K�E�Z���[�h
		APPEARANCE,		// �{�X�̓o�ꃂ�[�h
		GAME_CLEAR		// �Q�[���N���A�V�[��
	};

	Camera();
	~Camera();

	void Init();
	void Update();
	void SetBeforeDraw(const float deltaTime);
	void Draw();
	void Release();

	// �v���C���[�������Ă���p�x
	void SetLazyAngles(const VECTOR angles);

	// �Ǐ]�Ώۂ̐ݒ�
	void SetPlayer(const std::shared_ptr<Transform>& follow) { playerTransform_ = follow; }

	// �Ǐ]�Ώۂ̐ݒ�
	void SetBoss(const std::shared_ptr<Transform>& follow) { bossTransform_ = follow; }

	// ���b�N�I���̐ݒ�
	void SetLockOn(const bool lockOn);

	// �J�������[�h�̕ύX
	void ChangeMode(const MODE& mode);

	// �x����]
	void LazyRotation();

	//	���b�N�I������Y���̊p�x�𑫂�
	void AddLockOnAnglesY(float rad);

	// �X�e�[�W�̃��f��ID��ݒ�
	void SetStageID(const int modelId);

	// ���[�h���擾
	const Camera::MODE& GetMode()const { return mode_; }

	// �J�������W���擾
	const VECTOR& GetPos()const { return pos_; }

	// �J�������W��ݒ�
	void SetPos(const VECTOR& pos) { pos_ = pos; }

	// �����_���擾
	const VECTOR& GetTargetPos()const { return targetPos_; }

	// �p�x���擾
	const VECTOR& GetAngle()const { return angle_; }

	// �{�X�̓o��V�[����1�ڂ̃J�����̓����̃t���O���擾
	const bool GetIsBossAppearanceCameraMove1() { return isBossAppearanceCameraMove1_; }

	// �{�X�̓o��V�[����2�ڂ̃J�����̓����̃t���O���擾
	const bool GetIsBossAppearanceCameraMove2() { return isBossAppearanceCameraMove2_; }

	// �{�X�̓o��V�[����2�ڂ̃J�����̓����̃t���O���擾
	const bool GetIsBossAppearanceCameraMove3() { return isBossAppearanceCameraMove3_; }

	// �{�X�V�[�����I��������̃t���O
	const bool GetIsEndBossAppearanceScene() { return isEndBossAppearanceScene_; }

private:

	// �Ǐ]�Ώ�
	std::shared_ptr<Transform> playerTransform_;
	std::shared_ptr<Transform> bossTransform_;

	// �J�������[�h
	MODE mode_;

	// �J�����̈ʒu
	VECTOR pos_;

	// �ړ�����W
	VECTOR movedPos_;

	// �J�����̈ړ���
	VECTOR movePow_;

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

	// �K�E�Z���̃J�����̈ړ����鎞��
	float specialMoveCnt_;

	// �J���������b�ړ��������v��J�E���^
	float elapsedTime_;

	// �{�X�̓o��V�[����1�ڂ̃J�����̓����̃t���O
	bool isBossAppearanceCameraMove1_;

	// �{�X�̓o��V�[����2�ڂ̃J�����̓����̃t���O
	bool isBossAppearanceCameraMove2_;

	// �{�X�̓o��V�[����3�ڂ̃J�����̓����̃t���O
	bool isBossAppearanceCameraMove3_;

	// �{�X�̓o��V�[�����I��������̃t���O
	bool isEndBossAppearanceScene_;

	// �J�������[�h�̑J��
	std::unordered_map<MODE, std::function<void()>> modeChange_;
	void ChangeFixedPoint();
	void ChangeFree();
	void ChangeTitle();
	void ChangeFollow();
	void ChangeLockOn();
	void ChangeSpecial();
	void ChangeAppearance();
	void ChangeGameClear();

	// �J�������[�h�̍X�V
	std::function<void(const float deltaTime)> modeDraw_;
	void SetBeforeDrawFixedPoint(const float deltaTime);
	void SetBeforeDrawFree(const float deltaTime);
	void SetBeforeDrawTitle(const float deltaTime);
	void SetBeforeDrawFollow(const float deltaTime);
	void SetBeforeDrawLockOn(const float deltaTime);
	void SetBeforeDrawSpecial(const float deltaTime);
	void SetBeforeDrawAppearance(const float deltaTime);
	void SetBeforeDrawGameClear(const float deltaTime);

	// �֐��|�C���^�̏�����
	void InitFunctionPointer();

	// �J�����������ʒu�ɖ߂�
	void SetDefault();

	// �����_���L�����N�^�[�O���ʒu�ɐݒ�
	void SetTargetPosFollowForward();

	// �L�[�{�[�h�̑���
	void KeyboardController();
	void KeyboardLockOnController();

	// �Q�[���p�b�h�̑���
	void GamePadController();
	void GamePadLockOnController();

	// ImGui�̃f�o�b�O�`��̍X�V
	void UpdateDebugImGui();

};