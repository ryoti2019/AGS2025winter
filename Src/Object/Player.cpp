#include <iostream>
#include <memory>
#include "../Utility/Utility.h"
#include "../Scene/GameScene.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/Common/InputController.h"
#include "Player.h"

Player::Player(const VECTOR& pos)
	:
	ActorBase(pos)
{

	// 機能の初期化
	InitFunction();

	// モデルID
	modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER);

	// モデルの大きさ
	scl_ = 1.0f;

	// 共通部分は基底クラスで初期化
	ActorBase::Init(pos);

	// 関数ポインタの初期化
	InitFunctionPointer();

	// パラメータの初期化
	InitParameter();

	// アニメーションの初期化
	InitAnimation();

}

void Player::Init(const VECTOR& pos)
{
}

void Player::InitFunction()
{

	// インプットコントローラーの生成
	inputController_ = std::make_unique<InputController>(this);

	// カメラを生成
	std::weak_ptr<Camera> camera = SceneManager::GetInstance().GetCamera();

	// カメラのターゲットをプレイヤーに設定
	camera.lock()->SetPlayer(&transform_);

}

void Player::InitFunctionPointer()
{
	//関数ポインタの初期化
	stateChange_.emplace(STATE::IDLE, std::bind(&Player::ChangeIdle, this));
	stateChange_.emplace(STATE::RUN, std::bind(&Player::ChangeRun, this));
	stateChange_.emplace(STATE::JAB, std::bind(&Player::ChangeJab, this));
	stateChange_.emplace(STATE::STRAIGHT, std::bind(&Player::ChangeStraight, this));
}

void Player::InitParameter()
{

	// アクターの種類
	actorType_ = ActorType::PLAYER;

}

void Player::InitAnimation()
{

	// アニメーションコントローラの生成
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);

	// 待機
	animationController_->Add("IDLE", "Data/Model/Player/Idle.mv1", 0.0f, 30.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_IDLE), true, 0, false);

	// 走る
	animationController_->Add("RUN", "Data/Model/Player/Run.mv1", 0.0f, 30.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_RUN), true, 0, false);

	// ジャブ
	animationController_->Add("JAB", "Data/Model/Player/Run.mv1", 0.0f, 30.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_RUN), true, 0, false);

	// ストレート
	animationController_->Add("STRAIGHT", "Data/Model/Player/Run.mv1", 0.0f, 30.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_RUN), true, 0, false);

	// アニメーション再生するキー
	key_ = "IDLE";

	// 1個前のアニメーション
	preKey_ = key_;

	// 初期状態
	ChangeState(STATE::IDLE);

}

void Player::Update(const float deltaTime)
{

	// 移動処理
	Move();

	// 状態ごとの更新
	stateUpdate_();

	// アニメーション再生
	animationController_->Update(deltaTime);
	
	// モデル情報を更新
	transform_.Update();

}

void Player::Move()
{

	// 入力方向
	VECTOR dir = inputController_->Dir();

	// 入力していたら移動する
	if (!Utility::EqualsVZero(dir))
	{
		// 方向を更新
		dir_ = dir;
		ChangeState(STATE::RUN);
	}
	// 入力していなければ待機状態にする
	else if (Utility::EqualsVZero(dir))
	{
		ChangeState(STATE::IDLE);
	}

	// 方向を角度に変換する(XZ平面 Y軸)
	float angle = atan2f(dir_.x, dir_.z);

	// 回転処理(代用)
	LazyRotation(angle);

	// プレイヤーにカメラを追従するときはこっちに切り替える
	//LazyRotation(cameraAngles.y + angle);

}

void Player::Attack()
{
}

void Player::ChangeState(STATE state)
{

	// 状態遷移
	state_ = state;

	// 関数ポインタの遷移
	stateChange_[state_]();

	// 前のアニメーションを保存
	preKey_ = key_;

	// 新しいアニメーションを保存
	key_ = ANIM_DATA_KEY[static_cast<int>(state)];

	// アニメーションコントローラー側のアニメーション遷移
	animationController_->ChangeAnimation(key_);

}

void Player::ChangeIdle(void)
{
	stateUpdate_ = std::bind(&Player::UpdateIdle, this);
	stateDraw_ = std::bind(&Player::DrawIdle, this);
}

void Player::ChangeRun(void)
{
	stateUpdate_ = std::bind(&Player::UpdateRun, this);
	stateDraw_ = std::bind(&Player::DrawRun, this);
}

void Player::ChangeJab()
{
}

void Player::ChangeStraight()
{
}

void Player::UpdateIdle(void)
{
}

void Player::UpdateRun(void)
{
	// 移動処理
	transform_.pos = VAdd(transform_.pos, VScale(dir_, 100.0f));
}

void Player::UpdateJab()
{
}

void Player::UpdateStraight()
{
}

void Player::DrawIdle(void)
{
}

void Player::DrawRun(void)
{
}

void Player::DrawJab()
{
}

void Player::DrawStraight()
{
}
