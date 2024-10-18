#pragma once
#include <DxLib.h>

class AnimationController;
class Player;

class InputContoroller
{
public:

	// コンストラクタ
	InputContoroller(Player* player);

	// デストラクタ
	~InputContoroller(void);

	// 方向
	VECTOR Dir();

	// 移動
	VECTOR Move();

	// 溜め歩き
	bool ChargeWalk();

	// 走る
	bool Run();

	// 回避
	bool Roll();
	
	// 攻撃
	bool Attack();

	// 溜め攻撃
	bool ChargeAttack();

private:

	// アニメーション
	AnimationController* animationController_;

	// プレイヤー
	Player* player_;

	// 方向
	VECTOR dir_;

};

