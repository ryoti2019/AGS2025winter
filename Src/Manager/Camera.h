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

	// �v���C���[�Ǐ]���̃J�����̃��[�J�������_
	static constexpr VECTOR FOLLOW_CAMERA_LOCAL_TARGET_POS = { 0.0f,800.0f,0.0f };

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

	// �ŏ��̊p�x
	static constexpr float INIT_ANGLE = 90.0f;

	// ��_�J�����̍��W
	static constexpr VECTOR FIXED_POINT_CAMERA_POS = { 0.0f,200.0f,-500.0f };

	// ��_�J�����̒����_
	static constexpr VECTOR FIXED_POINT_CAMERA_TARGET_POS = { 0.0f,150.0f,0.0f };

	// �^�C�g���̃J�����̃��[�J�����W
	static constexpr VECTOR TITLE_CAMERA_LOCAL_POS = { -1200.0f,800.0f,2500.0f };

	// �^�C�g���̃J�����̃��[�J�������_
	static constexpr VECTOR TITLE_CAMERA_TARGET_LOCAL_POS = { -1200.0f, 1000.0f, 0.0f };

	// �K�E�Z�̃J�����̏������[�J�����W
	static constexpr VECTOR SPECIAL_CAMERA_INIT_LOCAL_POS = { 500.0f,2000.0f,500.0f };

	// �K�E�Z�̃J�����̃��[�J�������_
	static constexpr VECTOR SPECIAL_CAMERA_LOCAL_TARGET_POS = { 0.0f,1000.0f,0.0f };

	// �{�X�̓o��V�[���̃J�����̏������[�J�����W
	static constexpr VECTOR BOSS_APPEARANCE_CAMERA_INIT_LOCAL_POS = { -1000.0f, 2000.0f, -1000.0f };

	// �{�X�̓o��V�[���̃J�����̏��������_
	static constexpr VECTOR BOSS_APPEARANCE_CAMERA_INIT_LOCAL_TARGET_POS = { 0.0f,-1000.0f,1000.0f };

	// �Q�[���N���A�V�[���̃J�����̃��[�J�����W
	static constexpr VECTOR GAME_CLEAR_CAMERA_LOCAL_POS = { -1200.0f,800.0f,2500.0f };

	// �Q�[���N���A�V�[���̃J�����̒����_
	static constexpr VECTOR GAME_CLEAR_CAMERA_LOCAL_TRAGET_POS = { -1200.0f, 1000.0f, 0.0f };

	// �t���[���[�h�̎��̃J�����̉�]��
	static constexpr float FREE_MODE_ROTATION_POW = 0.01f;

	// �K�E�Z�̃J�����̈ړ���
	static constexpr VECTOR SPECIAL_CAMERA_MOVE_POW = { 20.0f,0.02f,20.0f };

	// �K�E�Z�̃J����1�ڂ̈ړ���
	static constexpr float CAMERA_MOVE_POW = 10.0f;

	// �K�E�Z�V�[����1�ڂ̓�������
	static constexpr float FIRST_CAMERA_TIME = 3.0f;

	// �J�������[�h
	enum class MODE
	{
		NONE,
		FIXED_POINT,	// ��_�J����
		FREE,			// �t���[���[�h
		TITLE,			// �^�C�g���V�[�����[�h
		FOLLOW,			// �Ǐ]���[�h
		SPECIAL,		// �K�E�Z���[�h
		APPEARANCE,		// �{�X�̓o�ꃂ�[�h
		GAME_CLEAR,		// �Q�[���N���A���[�h
		GAME_OVER		// �Q�[���I�[�o�[���[�h
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

	// �J�������[�h�̕ύX
	void ChangeMode(const MODE& mode);

	// �x����]
	void LazyRotation();

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

	// �{�X�̓o��V�[����3�ڂ̃J�����̓����̃t���O���擾
	const bool GetIsBossAppearanceCameraMove3() { return isBossAppearanceCameraMove3_; }

	// �{�X�V�[�����I��������̃t���O
	const bool GetIsEndBossAppearanceScene() { return isEndBossAppearanceScene_; }

	// �J���������b�ړ��������v��J�E���^
	const float GetElapsedTime()const { return elapsedTime_; }

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
	bool isLazy_;

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
	void ChangeSpecial();
	void ChangeAppearance();
	void ChangeGameClear();
	void ChangeGameOver();

	// �J�������[�h�̍X�V
	std::function<void(const float deltaTime)> modeDraw_;
	void SetBeforeDrawFixedPoint(const float deltaTime);
	void SetBeforeDrawFree(const float deltaTime);
	void SetBeforeDrawTitle(const float deltaTime);
	void SetBeforeDrawFollow(const float deltaTime);
	void SetBeforeDrawSpecial(const float deltaTime);
	void SetBeforeDrawAppearance(const float deltaTime);
	void SetBeforeDrawGameClear(const float deltaTime);
	void SetBeforeDrawGameOver(const float deltaTime);

	// �֐��|�C���^�̏�����
	void InitFunctionPointer();

	// �J�����������ʒu�ɖ߂�
	void SetDefault();

	// �����_���L�����N�^�[�O���ʒu�ɐݒ�
	void SetTargetPosFollowForward();

	// �L�[�{�[�h�̑���
	void KeyboardController();

	// �Q�[���p�b�h�̑���
	void GamePadController();

	// ImGui�̃f�o�b�O�`��̍X�V
	void UpdateDebugImGui();

};