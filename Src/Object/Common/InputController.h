#pragma once
#include <DxLib.h>
#include "../Object/Common/Transform.h"

class AnimationController;
class Player;

class InputController
{
public:

	// コンストラクタ
	InputController();

	// デストラクタ
	~InputController(void);

	// 方向
	VECTOR Dir();

	// ロックオン時の方向
	VECTOR LockOnDir(Transform transform);

	// 走る
	bool Run();

	// 攻撃
	bool Attack();

	// 長押し攻撃
	bool ChargeAttack();

	// アッパー
	bool Upper();

	// 必殺技
	bool SpecialAttack();

	// 決定
	bool Decide();

	// キャンセル
	bool Cancel();

	// 右選択
	bool SelectRight();

	// 左選択
	bool SelectLeft();

private:

	// アニメーション
	AnimationController* animationController_;

	// 方向
	VECTOR dir_;

};

