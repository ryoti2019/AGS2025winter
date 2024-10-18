#pragma once
#include <DxLib.h>
#include <functional>
#include "ActorBase.h"
#include "Common/InputController.h"

class InputController;

class Player : public ActorBase
{

public:

	enum class STATE
	{
		NONE,
		IDLE,
		RUN,
		MAX,
	};

	// アニメーションコントローラーに渡す引数
	std::string ANIM_DATA_KEY[static_cast<int>(STATE::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN"
	};

	using ActorBase::ActorBase;

	Player(const VECTOR& pos);

	~Player() = default;

	void Init(const VECTOR& pos) override;
	void Update(const float deltaTime) override;

private:

	// 入力コントローラー
	//InputController inputController_;

	// 状態
	STATE state_;

	// アニメーションコントローラーに状態を文字型で渡す変数
	std::string key_;
	std::string preKey_;

	// 状態遷移
	std::unordered_map<STATE, std::function<void(void)>> stateChange_;
	void ChangeIdle(void);
	void ChangeRun(void);

	// 状態の更新
	std::function<void(void)> stateUpdate_;
	void UpdateIdle(void);
	void UpdateRun(void);

	// 状態の描画
	std::function<void(void)> stateDraw_;
	void DrawIdle(void);
	void DrawRun(void);

	// 状態遷移
	void ChangeState(STATE state);

};

