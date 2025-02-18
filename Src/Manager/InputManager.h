#pragma once
#include <map>
#include <Dxlib.h>
#include "../Common/Vector2.h"

class InputManager
{

public:

	// ゲームコントローラーの認識番号
	// DxLib定数、DX_INPUT_PAD1等に対応
	enum class JOYPAD_NO
	{
		KEY_PAD1,			// キー入力とパッド１入力
		PAD1,				// パッド１入力
		PAD2,				// パッド２入力
		PAD3,				// パッド３入力
		PAD4,				// パッド４入力
		INPUT_KEY = 4096	// キー入力
	};

	// ゲームコントローラータイプ
	// DxLib定数、DX_OTHER等に対応
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

	// ゲームコントローラーボタン
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

	// XBOX_ONEコントローラー
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

	// XBOX_ONEコントローラー
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

	// ゲームコントローラーの入力情報
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

	// インスタンスを明示的に生成
	static void CreateInstance(void);

	// インスタンスの取得
	static InputManager& GetInstance(void);

	void Init(void);
	void Update(void);

	// リソースの破棄
	void Destroy(void);

	// 判定を行うキーを追加
	void Add(int key);

	// 判定を行うキーをクリア
	void Clear(void);

	// キーの押下判定
	const bool IsNew(int key) const { return Find(key).keyNew; }

	// キーの押下判定(押しっぱなしはNG)
	const bool IsTrgDown(int key) const { return Find(key).keyTrgDown; }

	// キーを離した時の判定
	const bool IsTrgUp(int key) const { return Find(key).keyTrgUp; }

	// マウス座標の取得
	const Vector2 GetMousePos(void) const { return mousePos_; }

	// マウスのクリック状態を取得(MOUSE_INPUT_LEFT、RIGHT)
	const int GetMouse(void) const { return mouseInput_; }

	// マウスが左クリックされたか
	const bool IsClickMouseLeft(void) const { return mouseInput_ == MOUSE_INPUT_LEFT; }

	// マウスが右クリックされたか
	const bool IsClickMouseRight(void) const { return mouseInput_ == MOUSE_INPUT_RIGHT; }

	// マウスが左クリックされたか(押しっぱなしはNG)
	const bool IsTrgDownMouseLeft(void) const { return FindMouse(MOUSE_INPUT_LEFT).keyTrgDown; }

	// マウスが左クリックされたか(押しっぱなしはNG)
	const bool IsTrgUpMouseLeft(void) const { return FindMouse(MOUSE_INPUT_LEFT).keyTrgUp; };

	// マウスが右クリックされたか(押しっぱなしはNG)
	const bool IsTrgDownMouseRight(void) const { return FindMouse(MOUSE_INPUT_RIGHT).keyTrgDown; }

	// マウスが右クリックされたか(押しっぱなしはNG)
	const bool IsTrgUpMouseRight(void) const { return FindMouse(MOUSE_INPUT_RIGHT).keyTrgUp; }

	// コントローラの入力情報を取得する
	JOYPAD_IN_STATE GetJPadInputState(JOYPAD_NO no);

	// ボタンが押された
	const bool IsPadBtnNew(JOYPAD_NO no, JOYPAD_BTN btn) const { return padInfos_[static_cast<int>(no)].IsNew[static_cast<int>(btn)]; }
	const bool IsPadBtnTrgDown(JOYPAD_NO no, JOYPAD_BTN btn) const { return padInfos_[static_cast<int>(no)].IsTrgDown[static_cast<int>(btn)]; }
	const bool IsPadBtnTrgUp(JOYPAD_NO no, JOYPAD_BTN btn) const { return padInfos_[static_cast<int>(no)].IsTrgUp[static_cast<int>(btn)]; }

	// 左のアナログキーが押された
	const bool IsPadLStickTrgDown(JOYPAD_NO no) const { return  padInfos_[static_cast<int>(no)].AKeyLTrgDown; }
	// 左のアナログキーが横軸に倒された
	const bool IsPadLXStickTrgDownX(JOYPAD_NO no) const { return  padInfos_[static_cast<int>(no)].AKeyLXTrgDown; }
	// 左のアナログキーが縦軸に倒された
	const bool IsPadLZStickTrgDownZ(JOYPAD_NO no) const { return  padInfos_[static_cast<int>(no)].AKeyLZTrgDown; }

	// 右のアナログキーが押された
	const bool IsPadRStickTrgDown(JOYPAD_NO no) const { return  padInfos_[static_cast<int>(no)].AKeyRTrgDown; }
	// 右のアナログキーが横軸に倒された
	const bool IsPadRXStickTrgDownX(JOYPAD_NO no) const { return  padInfos_[static_cast<int>(no)].AKeyRXTrgDown; }
	// 右のアナログキーが縦軸に倒された
	const bool IsPadRZStickTrgDownZ(JOYPAD_NO no) const { return  padInfos_[static_cast<int>(no)].AKeyRZTrgDown; }

private:

	// キー情報
	struct Info
	{
		int key;			// キーID
		bool keyOld;		// 1フレーム前の押下状態
		bool keyNew;		// 現フレームの押下状態
		bool keyTrgDown;	// 現フレームでボタンが押されたか
		bool keyTrgUp;		// 現フレームでボタンが離されたか
	};

	// マウス
	struct MouseInfo
	{
		int key;			// キーID
		bool keyOld;		// 1フレーム前の押下状態
		bool keyNew;		// 現フレームの押下状態
		bool keyTrgDown;	// 現フレームでボタンが押されたか
		bool keyTrgUp;		// 現フレームでボタンが離されたか
	};

	// コントローラ情報
	DINPUT_JOYSTATE joyDInState_;

	// コントローラ情報(XBOX)
	XINPUT_STATE joyXInState_;

	// シングルトン用インスタンス
	static InputManager* instance_;

	// キー情報
	std::map<int, InputManager::Info> keyInfos_;
	InputManager::Info infoEmpty_;

	// マウス情報
	std::map<int, InputManager::MouseInfo> mouseInfos_;
	InputManager::MouseInfo mouseInfoEmpty_;

	// マウスカーソルの位置
	Vector2 mousePos_;

	// マウスボタンの入力状態
	int mouseInput_;

	// パッド情報
	JOYPAD_IN_STATE padInfos_[5];

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	InputManager(void);
	InputManager(const InputManager& manager);
	~InputManager(void) = default;

	// 配列の中からキー情報を取得する
	const InputManager::Info& Find(int key) const;

	// 配列の中からマウス情報を取得する
	const InputManager::MouseInfo& FindMouse(int key) const;

	// 接続されたコントローラの種別を取得する
	JOYPAD_TYPE GetJPadType(JOYPAD_NO no) { return static_cast<InputManager::JOYPAD_TYPE>(GetJoypadType(static_cast<int>(no))); }

	// コントローラの入力情報を取得する
	DINPUT_JOYSTATE GetJPadDInputState(JOYPAD_NO no)	
	{
		// コントローラ情報
		GetJoypadDirectInputState(static_cast<int>(no), &joyDInState_);
		return joyDInState_;
	}

	// コントローラ(XBOX)の入力情報を取得する
	XINPUT_STATE GetJPadXInputState(JOYPAD_NO no)
	{
		// コントローラ情報
		GetJoypadXInputState(static_cast<int>(no), &joyXInState_);
		return joyXInState_;
	};

	// コントローラの入力情報を更新する
	void SetJPadInState(JOYPAD_NO jpNo);

};