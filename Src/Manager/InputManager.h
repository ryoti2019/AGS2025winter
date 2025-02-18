#pragma once
#include <map>
#include <Dxlib.h>
#include "../Common/Vector2.h"

class InputManager
{

public:

	// �Q�[���R���g���[���[�̔F���ԍ�
	// DxLib�萔�ADX_INPUT_PAD1���ɑΉ�
	enum class JOYPAD_NO
	{
		KEY_PAD1,			// �L�[���͂ƃp�b�h�P����
		PAD1,				// �p�b�h�P����
		PAD2,				// �p�b�h�Q����
		PAD3,				// �p�b�h�R����
		PAD4,				// �p�b�h�S����
		INPUT_KEY = 4096	// �L�[����
	};

	// �Q�[���R���g���[���[�^�C�v
	// DxLib�萔�ADX_OTHER���ɑΉ�
	enum class JOYPAD_TYPE
	{
		OTHER,
		XBOX_360,
		XBOX_ONE,
		DUAL_SHOCK_3,
		DUAL_SHOCK_4,
		DUAL_SENSE,
		SWITCH_JOY_CON_L,
		SWITCH_JOY_CON_R,
		SWITCH_PRO_CTRL,
		SWITCH_HORI_PAD,
		MAX
	};

	// �Q�[���R���g���[���[�{�^��
	enum class JOYPAD_BTN
	{
		LEFT = 0,
		RIGHT,
		TOP,
		DOWN,
		R_TRIGGER,
		L_TRIGGER,
		R_BUTTON,
		ZR_BUTTON,
		L_BUTTON,
		ZL_BUTTON,
		START_BUTTON,
		BACK_BUTTON,
		MAX
	};

	// XBOX_ONE�R���g���[���[
	enum class XBOX_ONE
	{
		A_BUTTON,
		B_BUTTON,
		X_BUTTON,
		Y_BUTTON,
		L_BUTTON,
		R_BUTTON,
		BACK_BUTTON,
		START_BUTTON,
		L_TRIGGER,
		R_TRIGGER
	};

	// XBOX_ONE�R���g���[���[
	enum class DUAL_SENSE
	{
		LEFT,
		DOWN,
		RIGHT,
		TOP,
		L_BUTTON,
		R_BOTTON,
		ZL_BUTTON,
		ZR_BUTTON,
		BACK_BOTTON,
		START_BOTTON,
		L_TRIGGER,
		R_TRIGGER
	};

	// �Q�[���R���g���[���[�̓��͏��
	struct JOYPAD_IN_STATE
	{
		unsigned char ButtonsOld[static_cast<int>(JOYPAD_BTN::MAX)];
		unsigned char ButtonsNew[static_cast<int>(JOYPAD_BTN::MAX)];
		bool IsOld[static_cast<int>(JOYPAD_BTN::MAX)];
		bool IsNew[static_cast<int>(JOYPAD_BTN::MAX)];
		bool IsTrgDown[static_cast<int>(JOYPAD_BTN::MAX)];
		bool IsTrgUp[static_cast<int>(JOYPAD_BTN::MAX)];
		int AKeyLX;
		int AKeyLZ;
		int AKeyRX;
		int AKeyRZ;
		bool AKeyLTrgDown;
		bool AKeyLXTrgDown;
		bool AKeyLZTrgDown;
		bool AKeyRTrgDown;
		bool AKeyRXTrgDown;
		bool AKeyRZTrgDown;
	};

	// �C���X�^���X�𖾎��I�ɐ���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static InputManager& GetInstance(void);

	void Init(void);
	void Update(void);

	// ���\�[�X�̔j��
	void Destroy(void);

	// ������s���L�[��ǉ�
	void Add(int key);

	// ������s���L�[���N���A
	void Clear(void);

	// �L�[�̉�������
	const bool IsNew(int key) const { return Find(key).keyNew; }

	// �L�[�̉�������(�������ςȂ���NG)
	const bool IsTrgDown(int key) const { return Find(key).keyTrgDown; }

	// �L�[�𗣂������̔���
	const bool IsTrgUp(int key) const { return Find(key).keyTrgUp; }

	// �}�E�X���W�̎擾
	const Vector2 GetMousePos(void) const { return mousePos_; }

	// �}�E�X�̃N���b�N��Ԃ��擾(MOUSE_INPUT_LEFT�ARIGHT)
	const int GetMouse(void) const { return mouseInput_; }

	// �}�E�X�����N���b�N���ꂽ��
	const bool IsClickMouseLeft(void) const { return mouseInput_ == MOUSE_INPUT_LEFT; }

	// �}�E�X���E�N���b�N���ꂽ��
	const bool IsClickMouseRight(void) const { return mouseInput_ == MOUSE_INPUT_RIGHT; }

	// �}�E�X�����N���b�N���ꂽ��(�������ςȂ���NG)
	const bool IsTrgDownMouseLeft(void) const { return FindMouse(MOUSE_INPUT_LEFT).keyTrgDown; }

	// �}�E�X�����N���b�N���ꂽ��(�������ςȂ���NG)
	const bool IsTrgUpMouseLeft(void) const { return FindMouse(MOUSE_INPUT_LEFT).keyTrgUp; };

	// �}�E�X���E�N���b�N���ꂽ��(�������ςȂ���NG)
	const bool IsTrgDownMouseRight(void) const { return FindMouse(MOUSE_INPUT_RIGHT).keyTrgDown; }

	// �}�E�X���E�N���b�N���ꂽ��(�������ςȂ���NG)
	const bool IsTrgUpMouseRight(void) const { return FindMouse(MOUSE_INPUT_RIGHT).keyTrgUp; }

	// �R���g���[���̓��͏����擾����
	JOYPAD_IN_STATE GetJPadInputState(JOYPAD_NO no);

	// �{�^���������ꂽ
	const bool IsPadBtnNew(JOYPAD_NO no, JOYPAD_BTN btn) const { return padInfos_[static_cast<int>(no)].IsNew[static_cast<int>(btn)]; }
	const bool IsPadBtnTrgDown(JOYPAD_NO no, JOYPAD_BTN btn) const { return padInfos_[static_cast<int>(no)].IsTrgDown[static_cast<int>(btn)]; }
	const bool IsPadBtnTrgUp(JOYPAD_NO no, JOYPAD_BTN btn) const { return padInfos_[static_cast<int>(no)].IsTrgUp[static_cast<int>(btn)]; }

	// ���̃A�i���O�L�[�������ꂽ
	const bool IsPadLStickTrgDown(JOYPAD_NO no) const { return  padInfos_[static_cast<int>(no)].AKeyLTrgDown; }
	// ���̃A�i���O�L�[�������ɓ|���ꂽ
	const bool IsPadLXStickTrgDownX(JOYPAD_NO no) const { return  padInfos_[static_cast<int>(no)].AKeyLXTrgDown; }
	// ���̃A�i���O�L�[���c���ɓ|���ꂽ
	const bool IsPadLZStickTrgDownZ(JOYPAD_NO no) const { return  padInfos_[static_cast<int>(no)].AKeyLZTrgDown; }

	// �E�̃A�i���O�L�[�������ꂽ
	const bool IsPadRStickTrgDown(JOYPAD_NO no) const { return  padInfos_[static_cast<int>(no)].AKeyRTrgDown; }
	// �E�̃A�i���O�L�[�������ɓ|���ꂽ
	const bool IsPadRXStickTrgDownX(JOYPAD_NO no) const { return  padInfos_[static_cast<int>(no)].AKeyRXTrgDown; }
	// �E�̃A�i���O�L�[���c���ɓ|���ꂽ
	const bool IsPadRZStickTrgDownZ(JOYPAD_NO no) const { return  padInfos_[static_cast<int>(no)].AKeyRZTrgDown; }

private:

	// �L�[���
	struct Info
	{
		int key;			// �L�[ID
		bool keyOld;		// 1�t���[���O�̉������
		bool keyNew;		// ���t���[���̉������
		bool keyTrgDown;	// ���t���[���Ń{�^���������ꂽ��
		bool keyTrgUp;		// ���t���[���Ń{�^���������ꂽ��
	};

	// �}�E�X
	struct MouseInfo
	{
		int key;			// �L�[ID
		bool keyOld;		// 1�t���[���O�̉������
		bool keyNew;		// ���t���[���̉������
		bool keyTrgDown;	// ���t���[���Ń{�^���������ꂽ��
		bool keyTrgUp;		// ���t���[���Ń{�^���������ꂽ��
	};

	// �R���g���[�����
	DINPUT_JOYSTATE joyDInState_;

	// �R���g���[�����(XBOX)
	XINPUT_STATE joyXInState_;

	// �V���O���g���p�C���X�^���X
	static InputManager* instance_;

	// �L�[���
	std::map<int, InputManager::Info> keyInfos_;
	InputManager::Info infoEmpty_;

	// �}�E�X���
	std::map<int, InputManager::MouseInfo> mouseInfos_;
	InputManager::MouseInfo mouseInfoEmpty_;

	// �}�E�X�J�[�\���̈ʒu
	Vector2 mousePos_;

	// �}�E�X�{�^���̓��͏��
	int mouseInput_;

	// �p�b�h���
	JOYPAD_IN_STATE padInfos_[5];

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	InputManager(void);
	InputManager(const InputManager& manager);
	~InputManager(void) = default;

	// �z��̒�����L�[�����擾����
	const InputManager::Info& Find(int key) const;

	// �z��̒�����}�E�X�����擾����
	const InputManager::MouseInfo& FindMouse(int key) const;

	// �ڑ����ꂽ�R���g���[���̎�ʂ��擾����
	JOYPAD_TYPE GetJPadType(JOYPAD_NO no) { return static_cast<InputManager::JOYPAD_TYPE>(GetJoypadType(static_cast<int>(no))); }

	// �R���g���[���̓��͏����擾����
	DINPUT_JOYSTATE GetJPadDInputState(JOYPAD_NO no)	
	{
		// �R���g���[�����
		GetJoypadDirectInputState(static_cast<int>(no), &joyDInState_);
		return joyDInState_;
	}

	// �R���g���[��(XBOX)�̓��͏����擾����
	XINPUT_STATE GetJPadXInputState(JOYPAD_NO no)
	{
		// �R���g���[�����
		GetJoypadXInputState(static_cast<int>(no), &joyXInState_);
		return joyXInState_;
	};

	// �R���g���[���̓��͏����X�V����
	void SetJPadInState(JOYPAD_NO jpNo);

};