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
	stateChange_.emplace(STATE::KICK, std::bind(&Player::ChangeKick, this));
	stateChange_.emplace(STATE::UPPER, std::bind(&Player::ChangeUpper, this));
}

void Player::InitCollision()
{

	// 右手
	collisionData_.rightHand = MV1SearchFrame(transform_.modelId, "RightHandMiddle1");
	MATRIX matRighthandPos = MV1GetFrameLocalWorldMatrix(transform_.modelId, collisionData_.rightHand);
	collisionData_.rightHandPos = MGetTranslateElem(matRighthandPos);

}

void Player::InitParameter()
{

	dir_ = Utility::VECTOR_ZERO;

	// アクターの種類
	actorType_ = ActorType::PLAYER;

	// 攻撃1段階目
	jab_ = false;

	// 攻撃2段階目
	straight_ = false;

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
	animationController_->Add("JAB", "Data/Model/Player/Jab.mv1", 0.0f, 80.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_JAB), false, 0, false);

	// ストレート
	animationController_->Add("STRAIGHT", "Data/Model/Player/Straight.mv1", 0.0f, 60.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_STRAIGHT), false, 0, false);

	// キック
	animationController_->Add("KICK", "Data/Model/Player/Kick.mv1", 0.0f, 40.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KICK), false, 0, false);

	// アッパー
	animationController_->Add("UPPER", "Data/Model/Player/Upper.mv1", 0.0f, 50.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_UPPER), false, 0, false);

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

	// 攻撃処理
	ComboAttack(deltaTime);

	// コンボ中に次の攻撃がなかったら待機状態に戻す
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(STATE::IDLE);
	}

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

	// 攻撃中は移動できない
	if (!AttackState())
	{
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
	}

	// 方向を角度に変換する(XZ平面 Y軸)
	float angle = atan2f(dir_.x, dir_.z);

	// 回転処理(代用)
	LazyRotation(angle);

	// プレイヤーにカメラを追従するときはこっちに切り替える
	//LazyRotation(cameraAngles.y + angle);

}

void Player::ComboAttack(const float deltaTime)
{

	// 攻撃の先行入力
	if (inputController_->ComboAttack())
	{
		// ジャブ
		if (state_ == STATE::IDLE)
		{
			jab_ = true;
		}
		// ストレート
		else if (state_ == STATE::JAB)
		{
			straight_ = true;
		}
		// キック
		else if (state_ == STATE::STRAIGHT)
		{
			kick_ = true;
		}
	}

	// ジャブに遷移
	if (jab_)
	{
		jab_ = false;
		ChangeState(STATE::JAB);
	}

	// アッパーに遷移
	if (inputController_->Upper() && !animationController_->IsPlayNowAnimation())
	{
		ChangeState(STATE::UPPER);
	}

}

bool Player::AttackState()
{

	// 攻撃中か判定
	if (state_ == STATE::JAB || state_ == STATE::STRAIGHT || state_ == STATE::KICK || state_ == STATE::UPPER)
	{
		return true;
	}

	return false;

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
}

void Player::ChangeRun(void)
{
	stateUpdate_ = std::bind(&Player::UpdateRun, this);
}

void Player::ChangeJab()
{

	stateUpdate_ = std::bind(&Player::UpdateJab, this);

	// 少し前に移動
	transform_.pos = VAdd(transform_.pos, VScale(dir_, 100.0f));

}

void Player::ChangeStraight()
{

	stateUpdate_ = std::bind(&Player::UpdateStraight, this);

	// 少し前に移動
	transform_.pos = VAdd(transform_.pos, VScale(dir_, 100.0f));

}

void Player::ChangeKick()
{
	stateUpdate_ = std::bind(&Player::UpdateKick, this);
}

void Player::ChangeUpper()
{

	stateUpdate_ = std::bind(&Player::UpdateUpper, this);

	// 少し前に移動
	transform_.pos = VAdd(transform_.pos, VScale(dir_, 100.0f));

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

	// ストレートに遷移
	if (animationController_->IsPreEndPlayAnimation() && straight_)
	{
		straight_ = false;
		ChangeState(STATE::STRAIGHT);
	}

}

void Player::UpdateStraight()
{
	// キックに遷移
	if (animationController_->IsPreEndPlayAnimation() && kick_)
	{
		kick_ = false;
		ChangeState(STATE::KICK);
	}
}

void Player::UpdateKick()
{
}

void Player::UpdateUpper()
{
}
