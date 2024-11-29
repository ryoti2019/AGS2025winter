#pragma once
#include "Component.h"
#include "../Object/Common/InputController.h"

class InputComponent : public Component
{

public:

	InputComponent();

	virtual void Update()override;

private:

	// 移動処理
	void Move();

	// 攻撃処理
	void Attack(const float deltaTime);

	// 入力用コントローラー
	std::unique_ptr<InputController> inputController_;

	// 溜めパンチのカウンタ
	float chargeCnt_;

};

