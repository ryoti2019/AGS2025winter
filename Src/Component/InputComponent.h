#pragma once
#include "Component.h"
#include "../Object/Common/InputController.h"

class Player;

class InputComponent : public Component
{

public:

	InputComponent(std::shared_ptr<Player>& player);

	void Update(const float deltaTime);

private:

	// 移動処理
	void Move();

	// 攻撃処理
	void Attack(const float deltaTime);

	// プレイヤーのポインター
	std::shared_ptr<Player> player_;

	// 入力用コントローラー
	std::unique_ptr<InputController> inputController_;

};

