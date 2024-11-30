#pragma once
#include <DxLib.h>
#include "../Component/TransformComponent.h"

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
	VECTOR LockOnDir(TransformComponent transform);

	// 走る
	bool Run();

	// 攻撃
	bool Attack();

	// 長押し攻撃
	bool ChargeAttack();

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

