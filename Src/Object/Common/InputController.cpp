#include "../Manager/InputManager.h"
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ActorManager.h"
#include "../Scene/GameScene.h"
#include "../../Object/Common/AnimationController.h"
#include "../Player.h"
#include "InputController.h"

InputController::InputController(Player* player)
{
	player_ = player;
}

InputController::~InputController(void)
{
}

VECTOR InputController::Dir()
{

	auto& ins = InputManager::GetInstance();

	// ������������
	dir_ = Utility::VECTOR_ZERO;

	// �R���g���[���[
	// WASD�Ńv���C���[�̈ʒu��ς���
	if (!SceneManager::GetInstance().GetGamePad())
	{
		// �O����
		if (ins.IsNew(KEY_INPUT_W))
		{
			dir_ = { 0.0f, 0.0f, 1.0f };
		}
		// ������
		if (ins.IsNew(KEY_INPUT_A))
		{
			dir_ = { -1.0f, 0.0f, 0.0f };
		}
		// ������
		if (ins.IsNew(KEY_INPUT_S))
		{
			dir_ = { 0.0f, 0.0f, -1.0f };
		}
		// �E����
		if (ins.IsNew(KEY_INPUT_D))
		{
			dir_ = { 1.0f, 0.0f, 0.0f };
		}
	}
	else
	{
		// �Q�[���p�b�h�̔ԍ����擾
		auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

		// �p�b�h�̕�����dir�ɒ���
		dir_.x = pad.AKeyLX;
		dir_.z = -pad.AKeyLZ;
	}

	return dir_;

}

VECTOR InputController::LockOnDir(Transform transform)
{

	auto& ins = InputManager::GetInstance();

	// ������������
	dir_ = Utility::VECTOR_ZERO;

	// �R���g���[���[
	// WASD�Ńv���C���[�̈ʒu��ς���
	if (!SceneManager::GetInstance().GetGamePad())
	{

		// �O����
		if (ins.IsNew(KEY_INPUT_W))
		{
			dir_ = transform.GetForward();
		}
		// ������
		if (ins.IsNew(KEY_INPUT_A))
		{
			dir_ = transform.GetLeft();
		}
		// ������
		if (ins.IsNew(KEY_INPUT_S))
		{
			dir_ = transform.GetBack();
		}
		// �E����
		if (ins.IsNew(KEY_INPUT_D))
		{
			dir_ = transform.GetRight();
		}
	}
	else
	{
		// �Q�[���p�b�h�̔ԍ����擾
		auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

		// �p�b�h�̕�����dir�ɒ���
		dir_.x = pad.AKeyLX;
		dir_.z = -pad.AKeyLZ;
	}

	return dir_;

}

bool InputController::Run()
{

	auto& ins = InputManager::GetInstance();

	if (!SceneManager::GetInstance().GetGamePad() && ins.IsNew(KEY_INPUT_LSHIFT))
	{
		return true;
	}
	else if (SceneManager::GetInstance().GetGamePad() &&
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
	{
		return true;
	}

	return false;

}

bool InputController::ComboAttack()
{

	auto& ins = InputManager::GetInstance();

	if (!SceneManager::GetInstance().GetGamePad() && ins.IsTrgUpMouseLeft())
	{
		return true;
	}
	else if (SceneManager::GetInstance().GetGamePad() &&
		ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
	{
		return true;
	}

	return false;

}

bool InputController::Upper()
{

	auto& ins = InputManager::GetInstance();

	if (!SceneManager::GetInstance().GetGamePad() && ins.IsTrgUpMouseLeft() && ins.IsNew(KEY_INPUT_LSHIFT))
	{
		return true;
	}

	return false;

}
