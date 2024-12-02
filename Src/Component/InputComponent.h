#pragma once
#include <functional>
#include <map>
#include "Component.h"
#include "../Object/Common/InputController.h"

class Player;

class InputComponent : public Component
{

public:

	InputComponent(std::shared_ptr<Player>& player);

	void Update(const float deltaTime);

	// アクションを追加する
	void AddAction(ACTION action, std::function<void(void)> func);

private:

	// 入力用コントローラー
	std::unique_ptr<InputController> inputController_;

	// 溜めパンチのカウンタ
	float chargeCnt_;

	std::map<ACTION, std::function<void(void)>> funcMap_;

	// 移動処理
	void Move();

	// 攻撃処理
	void Attack(const float deltaTime);

	// プレイヤーのポインター
	std::shared_ptr<Player> player_;

	// 入力用コントローラー
	std::unique_ptr<InputController> inputController_;

};

