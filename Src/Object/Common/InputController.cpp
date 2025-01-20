#include "../Manager/InputManager.h"
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ActorManager.h"
#include "../Scene/GameScene.h"
#include "../../Object/Common/AnimationController.h"
#include "../Player.h"
#include "InputController.h"

InputController::InputController()
{
}

InputController::~InputController(void)
{
}

VECTOR InputController::Dir()
{

	auto& ins = InputManager::GetInstance();

	// ������������
	dir_ = Utility::VECTOR_ZERO;

	// �L�[�{�[�h
	if (!SceneManager::GetInstance().GetGamePad())
	{
		// �O����
		if (ins.IsNew(KEY_INPUT_W))
		{
			dir_.z += 1.0f;
		}
		// ������
		if (ins.IsNew(KEY_INPUT_A))
		{
			dir_.x -= 1.0f;
		}
		// ������
		if (ins.IsNew(KEY_INPUT_S))
		{
			dir_.z -= 1.0f;
		}
		// �E����
		if (ins.IsNew(KEY_INPUT_D))
		{
			dir_.x += 1.0f;
		}
	}
	// �Q�[���p�b�h
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

	// �L�[�{�[�h
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
	// �Q�[���p�b�h
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

	// �L�[�{�[�h
	if (!SceneManager::GetInstance().GetGamePad() && ins.IsNew(KEY_INPUT_LSHIFT))
	{
		return true;
	}
	// �Q�[���p�b�h
	else if (SceneManager::GetInstance().GetGamePad() && ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
	{
		return true;
	}

	return false;

}

bool InputController::Attack()
{

	auto& ins = InputManager::GetInstance();

	// �L�[�{�[�h
	if (!SceneManager::GetInstance().GetGamePad() && ins.IsTrgUpMouseLeft())
	{
		return true;
	}
	// �Q�[���p�b�h
	else if (SceneManager::GetInstance().GetGamePad() && ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
	{
		return true;
	}

	return false;

}

bool InputController::ChargeAttack()
{

	auto& ins = InputManager::GetInstance();

	// �L�[�{�[�h
	if (!SceneManager::GetInstance().GetGamePad() && ins.IsClickMouseLeft())
	{
		return true;
	}
	// �Q�[���p�b�h
	else if (SceneManager::GetInstance().GetGamePad() && ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
	{
		return true;
	}

	return false;

}

bool InputController::Upper()
{

	auto& ins = InputManager::GetInstance();

	// �L�[�{�[�h
	if (!SceneManager::GetInstance().GetGamePad() && ins.IsTrgDown(KEY_INPUT_F))
	{
		return true;
	}
	// �Q�[���p�b�h
	else if (SceneManager::GetInstance().GetGamePad() && ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_BUTTON))
	{
		return true;
	}
	return false;

}

bool InputController::SpecialAttack()
{

	auto& ins = InputManager::GetInstance();

	// �L�[�{�[�h
	if (!SceneManager::GetInstance().GetGamePad() && ins.IsTrgDown(KEY_INPUT_G))
	{
		return true;
	}
	// �Q�[���p�b�h
	else if (SceneManager::GetInstance().GetGamePad() && ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::LEFT))
	{
		return true;
	}
	return false;

}

bool InputController::Decide()
{

	auto& ins = InputManager::GetInstance();

	// �L�[�{�[�h
	if (!SceneManager::GetInstance().GetGamePad() && ins.IsNew(KEY_INPUT_SPACE))
	{
		return true;
	}
	// �Q�[���p�b�h
	else if (SceneManager::GetInstance().GetGamePad() && ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
	{
		return true;
	}
	return false;

}

bool InputController::Cancel()
{

	auto& ins = InputManager::GetInstance();

	// �L�[�{�[�h
	if (!SceneManager::GetInstance().GetGamePad() && ins.IsNew(KEY_INPUT_ESCAPE))
	{
		return true;
	}
	// �Q�[���p�b�h
	else if (SceneManager::GetInstance().GetGamePad() && ins.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
	{
		return true;
	}
	return false;

}
