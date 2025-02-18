#include <DxLib.h>
#include "InputManager.h"

InputManager* InputManager::instance_ = nullptr;

void InputManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new InputManager();
	}
	instance_->Init();
}

InputManager& InputManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		InputManager::CreateInstance();
	}
	return *instance_;
}

void InputManager::Init(void)
{

	// ゲームで使用したいキーを、
	// 事前にここで登録しておいてください

	// シーン遷移キー
	InputManager::GetInstance().Add(KEY_INPUT_SPACE);

	// 移動キー
	InputManager::GetInstance().Add(KEY_INPUT_W);
	InputManager::GetInstance().Add(KEY_INPUT_A);
	InputManager::GetInstance().Add(KEY_INPUT_S);
	InputManager::GetInstance().Add(KEY_INPUT_D);
	InputManager::GetInstance().Add(KEY_INPUT_LSHIFT);

	// 攻撃キー
	InputManager::GetInstance().Add(KEY_INPUT_F);

	// 回避キー
	InputManager::GetInstance().Add(KEY_INPUT_R);

	// キャンセルキー
	InputManager::GetInstance().Add(KEY_INPUT_Z);
	

	// カメラの移動キー
	InputManager::GetInstance().Add(KEY_INPUT_Y);
	InputManager::GetInstance().Add(KEY_INPUT_G);
	InputManager::GetInstance().Add(KEY_INPUT_H);
	InputManager::GetInstance().Add(KEY_INPUT_J);
	InputManager::GetInstance().Add(KEY_INPUT_T);
	InputManager::GetInstance().Add(KEY_INPUT_U);

	InputManager::GetInstance().Add(KEY_INPUT_UP);
	InputManager::GetInstance().Add(KEY_INPUT_DOWN);
	InputManager::GetInstance().Add(KEY_INPUT_LEFT);
	InputManager::GetInstance().Add(KEY_INPUT_RIGHT);

	InputManager::MouseInfo info;

	// 左クリック
	info = InputManager::MouseInfo();
	info.key = MOUSE_INPUT_LEFT;
	info.keyOld = false;
	info.keyNew = false;
	info.keyTrgDown = false;
	info.keyTrgUp = false;
	mouseInfos_.emplace(info.key, info);

	// 右クリック
	info = InputManager::MouseInfo();
	info.key = MOUSE_INPUT_RIGHT;
	info.keyOld = false;
	info.keyNew = false;
	info.keyTrgDown = false;
	info.keyTrgUp = false;
	mouseInfos_.emplace(info.key, info);

}

void InputManager::Update(void)
{

	// キーボード検知
	for (auto& p : keyInfos_)
	{
		p.second.keyOld = p.second.keyNew;
		p.second.keyNew = CheckHitKey(p.second.key);
		p.second.keyTrgDown = p.second.keyNew && !p.second.keyOld;
		p.second.keyTrgUp = !p.second.keyNew && p.second.keyOld;
	}

	// マウス検知
	mouseInput_ = GetMouseInput();
	GetMousePoint(&mousePos_.x, &mousePos_.y);

	for (auto& p : mouseInfos_)
	{
		p.second.keyOld = p.second.keyNew;
		p.second.keyNew = mouseInput_ == p.second.key;
		p.second.keyTrgDown = p.second.keyNew && !p.second.keyOld;
		p.second.keyTrgUp = !p.second.keyNew && p.second.keyOld;
	}

	// パッド情報
	SetJPadInState(JOYPAD_NO::KEY_PAD1);
	SetJPadInState(JOYPAD_NO::PAD1);
	SetJPadInState(JOYPAD_NO::PAD2);
	SetJPadInState(JOYPAD_NO::PAD3);
	SetJPadInState(JOYPAD_NO::PAD4);

}

void InputManager::Destroy(void)
{
	keyInfos_.clear();
	mouseInfos_.clear();
	delete instance_;
}

void InputManager::Add(int key)
{
	InputManager::Info info = InputManager::Info();
	info.key = key;
	info.keyOld = false;
	info.keyNew = false;
	info.keyTrgDown = false;
	info.keyTrgUp = false;
	keyInfos_.emplace(key, info);
}

void InputManager::Clear(void)
{
	keyInfos_.clear();
}

InputManager::InputManager(void)
{
	mouseInput_ = -1;
}

InputManager::InputManager(const InputManager& manager)
{
}

const InputManager::Info& InputManager::Find(int key) const
{

	auto it = keyInfos_.find(key);
	if (it != keyInfos_.end())
	{
		return it->second;
	}

	return infoEmpty_;

}

const InputManager::MouseInfo& InputManager::FindMouse(int key) const
{
	auto it = mouseInfos_.find(key);
	if (it != mouseInfos_.end())
	{
		return it->second;
	}

	return mouseInfoEmpty_;
}

void InputManager::SetJPadInState(JOYPAD_NO jpNo)
{

	int no = static_cast<int>(jpNo);
	auto stateNew = GetJPadInputState(jpNo);
	auto& stateNow = padInfos_[no];

	int max = static_cast<int>(JOYPAD_BTN::MAX);
	for (int i = 0; i < max; i++)
	{

		stateNow.ButtonsOld[i] = stateNow.ButtonsNew[i];
		stateNow.ButtonsNew[i] = stateNew.ButtonsNew[i];

		stateNow.IsOld[i] = stateNow.IsNew[i];
		stateNow.IsNew[i] = stateNow.ButtonsNew[i] > 0;

		stateNow.IsTrgDown[i] = stateNow.IsNew[i] && !stateNow.IsOld[i];
		stateNow.IsTrgUp[i] = !stateNow.IsNew[i] && stateNow.IsOld[i];

	}

	stateNow.AKeyLX = stateNew.AKeyLX;
	stateNow.AKeyLZ = stateNew.AKeyLZ;
	stateNow.AKeyRX = stateNew.AKeyRX;
	stateNow.AKeyRZ = stateNew.AKeyRZ;

	stateNow.AKeyLXTrgDown = stateNow.AKeyLX;
	stateNow.AKeyLZTrgDown = stateNow.AKeyLZ;
	stateNow.AKeyLTrgDown = stateNow.AKeyLX != 0 || stateNow.AKeyLZ != 0;
	stateNow.AKeyRXTrgDown = stateNow.AKeyRX;
	stateNow.AKeyRZTrgDown = stateNow.AKeyRZ;
	stateNow.AKeyRTrgDown = stateNow.AKeyRX != 0 || stateNow.AKeyRZ != 0;

}

InputManager::JOYPAD_IN_STATE InputManager::GetJPadInputState(JOYPAD_NO no)
{

	JOYPAD_IN_STATE ret = JOYPAD_IN_STATE();

	auto type = GetJPadType(no);

	switch (type)
	{
	case InputManager::JOYPAD_TYPE::OTHER:
		break;
	case InputManager::JOYPAD_TYPE::XBOX_360:
	{
	}
	break;
	case InputManager::JOYPAD_TYPE::XBOX_ONE:
	{

		auto d = GetJPadDInputState(no);
		auto x = GetJPadXInputState(no);

		int idx;

		//   Y
		// X   B
		//   A
		idx = static_cast<int>(JOYPAD_BTN::DOWN);
		ret.ButtonsNew[idx] = d.Buttons[static_cast<int>(XBOX_ONE::A_BUTTON)];// Aボタン

		idx = static_cast<int>(JOYPAD_BTN::RIGHT);
		ret.ButtonsNew[idx] = d.Buttons[static_cast<int>(XBOX_ONE::B_BUTTON)];// Bボタン

		idx = static_cast<int>(JOYPAD_BTN::LEFT);
		ret.ButtonsNew[idx] = d.Buttons[static_cast<int>(XBOX_ONE::X_BUTTON)];// Xボタン

		idx = static_cast<int>(JOYPAD_BTN::TOP);
		ret.ButtonsNew[idx] = d.Buttons[static_cast<int>(XBOX_ONE::Y_BUTTON)];// Yボタン

		idx = static_cast<int>(JOYPAD_BTN::L_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[static_cast<int>(XBOX_ONE::L_BUTTON)];// Lボタン

		idx = static_cast<int>(JOYPAD_BTN::R_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[static_cast<int>(XBOX_ONE::R_BUTTON)];// Rボタン

		idx = static_cast<int>(JOYPAD_BTN::BACK_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[static_cast<int>(XBOX_ONE::BACK_BUTTON)];// Backボタン

		idx = static_cast<int>(JOYPAD_BTN::START_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[static_cast<int>(XBOX_ONE::START_BUTTON)];// Startボタン

		idx = static_cast<int>(JOYPAD_BTN::L_TRIGGER);
		ret.ButtonsNew[idx] = d.Buttons[static_cast<int>(XBOX_ONE::L_TRIGGER)]; // Lスティック押し込み

		idx = static_cast<int>(JOYPAD_BTN::R_TRIGGER);
		ret.ButtonsNew[idx] = d.Buttons[static_cast<int>(XBOX_ONE::R_TRIGGER)];// Rスティック押し込み

		// 左スティック
		ret.AKeyLX = d.X;
		ret.AKeyLZ = d.Y;

		// 右スティック
		ret.AKeyRX = d.Rx;
		ret.AKeyRZ = d.Ry;

	}
	break;
	case InputManager::JOYPAD_TYPE::DUAL_SHOCK_3:
		break;
	case InputManager::JOYPAD_TYPE::DUAL_SHOCK_4:
		break;
	case InputManager::JOYPAD_TYPE::DUAL_SENSE:
	{

		auto d = GetJPadDInputState(no);
		auto x = GetJPadXInputState(no);

		int idx;

		//   △
		// □  〇
		//   ×

		idx = static_cast<int>(JOYPAD_BTN::LEFT);
		ret.ButtonsNew[idx] = d.Buttons[static_cast<int>(DUAL_SENSE::LEFT)];// □

		idx = static_cast<int>(JOYPAD_BTN::DOWN);
		ret.ButtonsNew[idx] = d.Buttons[static_cast<int>(DUAL_SENSE::DOWN)];// ×

		idx = static_cast<int>(JOYPAD_BTN::RIGHT);
		ret.ButtonsNew[idx] = d.Buttons[static_cast<int>(DUAL_SENSE::RIGHT)];// 〇

		idx = static_cast<int>(JOYPAD_BTN::TOP);
		ret.ButtonsNew[idx] = d.Buttons[static_cast<int>(DUAL_SENSE::TOP)];// △

		idx = static_cast<int>(JOYPAD_BTN::L_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[static_cast<int>(DUAL_SENSE::L_BUTTON)];// L1ボタン

		idx = static_cast<int>(JOYPAD_BTN::R_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[static_cast<int>(DUAL_SENSE::R_BOTTON)];// R1ボタン

		idx = static_cast<int>(JOYPAD_BTN::ZL_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[static_cast<int>(DUAL_SENSE::ZL_BUTTON)];// L2ボタン

		idx = static_cast<int>(JOYPAD_BTN::ZR_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[static_cast<int>(DUAL_SENSE::ZR_BUTTON)];// R2ボタン

		idx = static_cast<int>(JOYPAD_BTN::BACK_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[static_cast<int>(DUAL_SENSE::BACK_BOTTON)]; // SHAREボタン

		idx = static_cast<int>(JOYPAD_BTN::START_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[static_cast<int>(DUAL_SENSE::START_BOTTON)]; // OPTIONボタン

		idx = static_cast<int>(JOYPAD_BTN::L_TRIGGER);
		ret.ButtonsNew[idx] = d.Buttons[static_cast<int>(DUAL_SENSE::L_TRIGGER)];// Lスティック押し込み

		idx = static_cast<int>(JOYPAD_BTN::R_TRIGGER);
		ret.ButtonsNew[idx] = d.Buttons[static_cast<int>(DUAL_SENSE::R_TRIGGER)];// Rスティック押し込み

		// 左スティック
		ret.AKeyLX = d.X;
		ret.AKeyLZ = d.Y;

		// 右スティック
		ret.AKeyRX = d.Z;
		ret.AKeyRZ = d.Rz;

	}
	break;
	case InputManager::JOYPAD_TYPE::SWITCH_JOY_CON_L:
		break;
	case InputManager::JOYPAD_TYPE::SWITCH_JOY_CON_R:
		break;
	case InputManager::JOYPAD_TYPE::SWITCH_PRO_CTRL:
		break;
	case InputManager::JOYPAD_TYPE::SWITCH_HORI_PAD:
		break;
	case InputManager::JOYPAD_TYPE::MAX:
		break;
	}

	return ret;

}