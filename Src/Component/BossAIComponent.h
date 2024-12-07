#pragma once
#include "Component.h"

class Boss;

class BossAIComponent : public Component
{
public:

	BossAIComponent(const std::shared_ptr<Boss> boss);

	void Update(const float deltaTime);

private:

	// 敵のポインター
	std::shared_ptr<Boss> boss_;

	// アクションを選択
	void SelectAction(const float deltaTime);

	// 移動処理
	void Move(const float deltaTime);

	// 攻撃処理
	void Attack(const float deltaTime);

};

