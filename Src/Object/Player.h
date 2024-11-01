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
		JAB,
		STRAIGHT,
		HOOK,
		LEFT_KICK,
		RIGHT_KICK,
		UPPER,
		MAX
	};

	// アニメーションコントローラーに渡す引数
	std::string ANIM_DATA_KEY[static_cast<int>(STATE::MAX)] =
	{
		"NONE",
		"IDLE",
		"RUN",
		"JAB",
		"STRAIGHT",
		"HOOK",
		"LEFT_KICK",
		"RIGHT_KICK",
		"UPPER"
	};

	struct ANIM_ACCEPT_TIME
	{

		// アニメーション
		STATE key;

		// アニメーションの受付開始時間
		float animAcceptStartTime;

		// アニメーションの受付終了時間
		float animAcceptEndTime;

	};

	Player(const VECTOR& pos, const json& data);

	~Player() = default;

	void Init(const VECTOR& pos) override;
	void Update(const float deltaTime) override;

	bool GetAttackState()override;

private:

	// 入力用コントローラー
	std::unique_ptr<InputController> inputController_;

	const std::vector<STATE> attackState_ = 
	{
		{STATE::JAB},
		{STATE::STRAIGHT},
		{STATE::HOOK},
		{STATE::LEFT_KICK},
		{STATE::RIGHT_KICK},
		{STATE::UPPER}
	};

	// 状態
	STATE state_;

	// アニメーションの受付時間
	std::vector<ANIM_ACCEPT_TIME> animAcceptTime_;

	// ジャブ
	bool jab_;

	// ストレート
	bool straight_;

	// フック
	bool hook_;

	// 左キック
	bool leftKick_;

	// 右キック
	bool rightKick_;

	// 攻撃4段階目
	bool upper_;

	// 入力カウンタ
	float acceptCnt_;

	// 状態遷移
	std::unordered_map<STATE, std::function<void()>> stateChange_;
	void ChangeIdle();
	void ChangeRun();
	void ChangeJab();
	void ChangeStraight();
	void ChangeHook();
	void ChangeLeftKick();
	void ChangeRightKick();
	void ChangeUpper();

	// 状態の更新
	std::function<void()> stateUpdate_;
	void UpdateIdle();
	void UpdateRun();
	void UpdateJab();
	void UpdateStraight();
	void UpdateHook();
	void UpdateLeftKick();
	void UpdateRightKick();
	void UpdateUpper();

	// 機能の初期化
	void InitFunction()override;

	// パラメータの初期化
	void InitParameter()override;

	// アニメーションの初期化
	void InitAnimation()override;

	// 関数ポインタの初期化
	void InitFunctionPointer()override;

	// ImGuiのデバッグ描画の更新
	virtual void UpdateDebugImGui();

	// 状態遷移
	void ChangeState(STATE state);

	// 移動処理
	void Move()override;

	// 攻撃処理
	void ComboAttack(const float deltaTime)override;

};

