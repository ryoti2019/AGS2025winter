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

	// 方向を初期化
	dir_ = Utility::VECTOR_ZERO;

	// コントローラー
	// WASDでプレイヤーの位置を変える
	if (!SceneManager::GetInstance().GetGamePad())
	{
		// 前方向
		if (ins.IsNew(KEY_INPUT_W))
		{
			dir_ = { 0.0f, 0.0f, 1.0f };
		}
		// 左方向
		if (ins.IsNew(KEY_INPUT_A))
		{
			dir_ = { -1.0f, 0.0f, 0.0f };
		}
		// 後ろ方向
		if (ins.IsNew(KEY_INPUT_S))
		{
			dir_ = { 0.0f, 0.0f, -1.0f };
		}
		// 右方向
		if (ins.IsNew(KEY_INPUT_D))
		{
			dir_ = { 1.0f, 0.0f, 0.0f };
		}
	}
	else
	{
		// ゲームパッドの番号を取得
		auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

		// パッドの方向をdirに直す
		dir_.x = pad.AKeyLX;
		dir_.z = -pad.AKeyLZ;
	}

	return dir_;

}

VECTOR InputController::LockOnDir(Transform transform)
{

	auto& ins = InputManager::GetInstance();

	// 方向を初期化
	dir_ = Utility::VECTOR_ZERO;

	// コントローラー
	// WASDでプレイヤーの位置を変える
	if (!SceneManager::GetInstance().GetGamePad())
	{

		// 前方向
		if (ins.IsNew(KEY_INPUT_W))
		{
			dir_ = transform.GetForward();
		}
		// 左方向
		if (ins.IsNew(KEY_INPUT_A))
		{
			dir_ = transform.GetLeft();
		}
		// 後ろ方向
		if (ins.IsNew(KEY_INPUT_S))
		{
			dir_ = transform.GetBack();
		}
		// 右方向
		if (ins.IsNew(KEY_INPUT_D))
		{
			dir_ = transform.GetRight();
		}
	}
	else
	{
		// ゲームパッドの番号を取得
		auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

		// パッドの方向をdirに直す
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
