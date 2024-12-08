#pragma once
#include "Component.h"

class Enemy;

class EnemyAIComponent : public Component
{

public:

	EnemyAIComponent(const std::shared_ptr<Enemy> enemy);

	void Update(const float deltaTime);

private:

	// 敵のポインター
	std::shared_ptr<Enemy> enemy_;

	// アクションを選択
	void SelectAction(const float deltaTime);

	// 移動処理
	void Move(const float deltaTime);

	// 攻撃処理
	void Attack(const float deltaTime);

};

