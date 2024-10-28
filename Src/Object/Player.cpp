#include <memory>
#include "../Utility/Utility.h"
#include "../Scene/GameScene.h"
#include "../Manager/SceneManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/Camera.h"
#include "../Object/Common/InputController.h"
#include "../Manager/ActorManager.h"
#include "Player.h"

Player::Player(const VECTOR& pos, const json& data)
	:
	ActorBase(pos, data)
{

	// 機能の初期化
	InitFunction();

	// モデルID
	modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER);

	// 関数ポインタの初期化
	InitFunctionPointer();

	// 共通部分は基底クラスで初期化
	ActorBase::Init(pos);

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

void Player::InitParameter()
{

	// ジャブ
	jab_ = false;

	// ストレート
	straight_ = false;

	// 右手のフレーム名
	RIGHT_HAND_FRAME = jsonData_["RIGHT_HAND_FRAME_NAME"];

	// 左手のフレーム名
	LEFT_HAND_FRAME = jsonData_["LEFT_HAND_FRAME_NAME"];

	// 右足のフレーム名
	RIGHT_FOOT_FRAME = jsonData_["RIGHT_FOOT_FRAME_NAME"];

	// 左足のフレーム名
	LEFT_FOOT_FRAME = jsonData_["LEFT_FOOT_FRAME_NAME"];

	// 体のフレーム名
	BODY_FRAME = jsonData_["BODY_FRAME_NAME"];

	// 右手のフレーム番号を取得
	collisionData_.rightHand = MV1SearchFrame(transform_.modelId, RIGHT_HAND_FRAME.c_str());

	// 左手のフレーム番号を取得
	collisionData_.leftHand = MV1SearchFrame(transform_.modelId, LEFT_HAND_FRAME.c_str());

	// 右足のフレーム番号を取得
	collisionData_.rightFoot = MV1SearchFrame(transform_.modelId, RIGHT_FOOT_FRAME.c_str());

	// 左足のフレーム番号を取得
	collisionData_.leftFoot = MV1SearchFrame(transform_.modelId, LEFT_FOOT_FRAME.c_str());

	// 体のフレーム番号を取得
	collisionData_.body = MV1SearchFrame(transform_.modelId, BODY_FRAME.c_str());

	// 手足の衝突判定の半径
	collisionData_.handAndFootCollisionRadius = HAND_AND_FOOT_COLLISION_RADIUS;

	// 体の衝突判定の半径
	collisionData_.bodyCollisionRadius = BODY_COLLISION_RADIUS;

}

void Player::InitAnimation()
{

	// アニメーションコントローラの生成
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);

	// アニメーションコントローラーにアニメーションを追加
	for (int i = static_cast<int>(STATE::IDLE); i < static_cast<int>(STATE::MAX); ++i)
	{

		// ループ再生するアニメーションだけisLoopをtrueにする
		bool isLoop = i == static_cast<int>(STATE::IDLE) || i == static_cast<int>(STATE::RUN);
			animationController_->Add(

			// アニメーションの名前
			jsonData_["ANIM"][i - 1]["NAME"],

			// アニメーションのパス
			jsonData_["ANIM"][i - 1]["PATH"],

			// アニメーションが始まる時間
			0.0f,
			
			// アニメーションスピード
			jsonData_["ANIM"][i - 1]["SPEED"],

			// アニメーションハンドル
			resMng_.LoadModelDuplicate(static_cast<ResourceManager::SRC>(i)),

			// アニメーションのループ再生
			isLoop,

			// アニメーション番号
			0,

			// アニメーションの逆再生
			false
			);
	}

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

	// カメラの角度
	VECTOR cameraAngle = SceneManager::GetInstance().GetCamera().lock()->GetAngle();

	// 基底クラスから使いたい型へキャストする
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	auto activeData = gameScene->GetActorManager()->GetActiveActorData();

	// 敵の方向
	VECTOR dir = Utility::VECTOR_ZERO;;

	for (auto& data : activeData)
	{
		for (const std::shared_ptr<ActorBase>& actor : data.second)
		{

			// 敵
			const auto& enemys = activeData.find(ActorType::ENEMY);

			// 中身が入っているか確認
			if (enemys == activeData.end())continue;

			for (const std::shared_ptr<ActorBase>& enemy : enemys->second)
			{
				// trueだったらこの敵をロックオンする
				if (enemy->GetIsLockOn())
				{
					dir = VSub(enemy->GetPos(), transform_.pos);
					dir = VNorm(dir);
					isLockOn_ = true;
				}
			}

		}
	}

	// 方向を角度に変換する(XZ平面 Y軸)
	float angle = atan2f(dir.x, dir.z);

	// プレイヤーにカメラを追従するときはこっちに切り替える
	LazyRotation(cameraAngle.y + angle);

	// 攻撃処理
	ComboAttack(deltaTime);

	// コンボ中に次の攻撃がなかったら待機状態に戻す
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(STATE::IDLE);
	}

	// 衝突判定の更新
	ActorBase::CollisionUpdate();

	// 状態ごとの更新
	stateUpdate_();

	// アニメーション再生
	animationController_->Update(deltaTime);

	// モデル情報を更新
	transform_.Update();

}

bool Player::GetAttackState()
{

	for (const auto state : attackState_)
	{
		if (state_ == state)
		{
			return true;
		}
	}

	return false;

}

void Player::Move()
{

	// 入力方向
	VECTOR dir = inputController_->Dir();

	if (isLockOn_)
	{
		dir = inputController_->LockOnDir(transform_);
	}

	//// カメラの角度
	//VECTOR cameraAngle = SceneManager::GetInstance().GetCamera().lock()->GetAngle();

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

	//// 方向を角度に変換する(XZ平面 Y軸)
	//float angle = atan2f(dir_.x, dir_.z);

	//// プレイヤーにカメラを追従するときはこっちに切り替える
	//LazyRotation(cameraAngle.y + angle);

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
	transform_.pos = VAdd(transform_.pos, VScale(dir_, ATTACK_MOVE_POW));

}

void Player::ChangeStraight()
{

	stateUpdate_ = std::bind(&Player::UpdateStraight, this);

	// 少し前に移動
	transform_.pos = VAdd(transform_.pos, VScale(dir_, ATTACK_MOVE_POW));

}

void Player::ChangeKick()
{
	stateUpdate_ = std::bind(&Player::UpdateKick, this);
}

void Player::ChangeUpper()
{

	stateUpdate_ = std::bind(&Player::UpdateUpper, this);

	// 少し前に移動
	transform_.pos = VAdd(transform_.pos, VScale(dir_, ATTACK_MOVE_POW));

}

void Player::UpdateIdle(void)
{
}

void Player::UpdateRun(void)
{
	// 移動処理
	transform_.pos = VAdd(transform_.pos, VScale(dir_, ATTACK_MOVE_POW));
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
