#pragma once
#include <DxLib.h>
#include <functional>
#include "ActorBase.h"
#include "Common/InputController.h"

class Player : public ActorBase
{

public:

	enum class STATE
	{
		NONE,
		IDLE,
		RUN,
		MAX
	};

	// アニメーションコントローラーに渡す引数
	std::string ANIM_DATA_KEY[static_cast<int>(STATE::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN"
	};

	Player(const VECTOR& pos);

	~Player() = default;

	void Init(const VECTOR& pos) override;
	void Update(const float deltaTime) override;

private:

	// 入力用コントローラー
	std::unique_ptr<InputController> inputController_;

	// 状態
	STATE state_;

	// アニメーションコントローラーに状態を文字型で渡す変数
	std::string key_;
	std::string preKey_;

	// 状態遷移
	std::unordered_map<STATE, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();

	// 状態の更新
	std::function<void()> stateUpdate_;
	void UpdateIdle();
	void UpdateRun();

	// 状態の描画
	std::function<void()> stateDraw_;
	void DrawIdle();
	void DrawRun();

	// 機能の初期化
	void InitFunction();

	// パラメータの初期化
	void InitPrameter();

	// アニメーションの初期化
	void InitAnimation();

	// 関数ポインタの初期化
	void InitFunctionPointer();

	// 状態遷移
	void ChangeState(STATE state);

	// 移動処理
	void Move()override;

};

