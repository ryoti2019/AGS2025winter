#pragma once
#include <functional>
#include <map>
#include "Component.h"
#include "../Object/Common/InputController.h"

class InputComponent : public Component
{

public:

	enum class ACTION
	{
		NONE,
		RUN,
		ATTACK,
	};

	InputComponent();

	virtual void Update()override;

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

};

