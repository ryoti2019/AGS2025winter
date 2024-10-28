#pragma once
#include <DxLib.h>

class AnimationController;
class Player;

class InputController
{
public:

	// コンストラクタ
	InputController(Player* player);

	// デストラクタ
	~InputController(void);

	// 方向
	VECTOR Dir();

	// ロックオン時の方向
	VECTOR LockOnDir(Transform transform);

	// 走る
	bool Run();

	// 攻撃
	bool ComboAttack();

	// アッパー
	bool Upper();

private:

	// アニメーション
	AnimationController* animationController_;

	// プレイヤー
	Player* player_;

	// 方向
	VECTOR dir_;

};

