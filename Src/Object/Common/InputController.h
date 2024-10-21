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

	// 走る
	bool Run();

	// 攻撃
	bool Attack();

private:

	// アニメーション
	AnimationController* animationController_;

	// プレイヤー
	Player* player_;

	// 方向
	VECTOR dir_;

};

