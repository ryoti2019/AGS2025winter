#include <memory>
//#include <ranges>
#include "../Lib/ImGui/imgui.h"
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
	ActorBase(pos, data),
	JAB_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(STATE::JAB) - 1]["ATTACK_START_FRAME"]),
	JAB_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(STATE::JAB) - 1]["ATTACK_END_FRAME"]),
	STRAIGHT_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(STATE::STRAIGHT) - 1]["ATTACK_START_FRAME"]),
	STRAIGHT_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(STATE::STRAIGHT) - 1]["ATTACK_END_FRAME"]),
	HOOK_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(STATE::HOOK) - 1]["ATTACK_START_FRAME"]),
	HOOK_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(STATE::HOOK) - 1]["ATTACK_END_FRAME"]),
	LEFT_KICK_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(STATE::LEFT_KICK) - 1]["ATTACK_START_FRAME"]),
	LEFT_KICK_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(STATE::LEFT_KICK) - 1]["ATTACK_END_FRAME"]),
	RIGHT_KICK_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(STATE::RIGHT_KICK) - 1]["ATTACK_START_FRAME"]),
	RIGHT_KICK_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(STATE::RIGHT_KICK) - 1]["ATTACK_END_FRAME"]),
	UPPER_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(STATE::UPPER) - 1]["ATTACK_START_FRAME"]),
	UPPER_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(STATE::UPPER) - 1]["ATTACK_END_FRAME"])
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
	stateChange_.emplace(STATE::HOOK, std::bind(&Player::ChangeHook, this));
	stateChange_.emplace(STATE::LEFT_KICK, std::bind(&Player::ChangeLeftKick, this));
	stateChange_.emplace(STATE::RIGHT_KICK, std::bind(&Player::ChangeRightKick, this));
	stateChange_.emplace(STATE::UPPER, std::bind(&Player::ChangeUpper, this));
	stateChange_.emplace(STATE::HIT, std::bind(&Player::ChangeHit, this));

}

void Player::InitParameter()
{

	// 攻撃の入力を初期化
	for (int i = static_cast<int>(STATE::JAB); i < static_cast<int>(STATE::MAX); i++)
	{
		isCombo_.emplace(static_cast<STATE>(i), false);
	}

	// 動く方向
	moveDir_ = { 0.0f,0.0f,1.0f };

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

	// 右手に攻撃判定があるかどうか
	collisionData_.isRightHandAttack = false;

	// 左手に攻撃判定があるかどうか
	collisionData_.isLeftHandAttack = false;

	// 右足に攻撃判定があるかどうか
	collisionData_.isRightFootAttack = false;

	// 左足に攻撃判定があるかどうか
	collisionData_.isLeftFootAttack = false;

	// 入力カウンタの初期化
	acceptCnt_ = 0.0f;

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

	// ImGuiのデバッグ描画の更新
	UpdateDebugImGui();

	// 移動処理
	Move();

	// 攻撃処理
	Attack();

	// 入力受付時間をカウント
	acceptCnt_++;

	// 衝突判定の更新
	ActorBase::CollisionUpdate();

	// 状態ごとの更新
	stateUpdate_();

	// アニメーション再生
	animationController_->Update(deltaTime);

	// モデル情報を更新
	transform_.Update();

}

void Player::UpdateDebugImGui()
{

	// ウィンドウタイトル&開始処理
	ImGui::Begin("Player");

	// 大きさ
	ImGui::Text("scale");
	ImGui::InputFloat("Scl", &scl_);

	// 角度
	VECTOR rotDeg = VECTOR();
	rotDeg.x = Utility::Rad2DegF(transform_.quaRot.x);
	rotDeg.y = Utility::Rad2DegF(transform_.quaRot.y);
	rotDeg.z = Utility::Rad2DegF(transform_.quaRot.z);
	ImGui::Text("angle(deg)");
	ImGui::SliderFloat("RotX", &rotDeg.x, -90.0f, 90.0f);
	ImGui::SliderFloat("RotY", &rotDeg.y, -90.0f, 90.0f);
	ImGui::SliderFloat("RotZ", &rotDeg.z, -90.0f, 90.0f);
	transform_.quaRot.x = Utility::Deg2RadF(rotDeg.x);
	transform_.quaRot.y = Utility::Deg2RadF(rotDeg.y);
	transform_.quaRot.z = Utility::Deg2RadF(rotDeg.z);

	// 位置
	ImGui::Text("position");
	// 構造体の先頭ポインタを渡し、xyzと連続したメモリ配置へアクセス
	ImGui::InputFloat3("Pos", &transform_.pos.x);
	// 終了処理
	ImGui::End();

}

bool Player::GetAttackState()
{

	// 攻撃の状態か判定
	for (const auto state : attackState_)
	{
		if (state_ == state)
		{
			return true;
		}
	}

	return false;

}

bool Player::GetHitState()
{

	// 攻撃を受けている状態か判定
	for (const auto state : hitState_)
	{
		if (state_ == state)
		{
			return true;
		}
	}

	return false;

}

bool Player::GetComboState()
{

	// コンボ中か判定
	for (const auto state : comboState_)
	{
		if (state_ == state)
		{
			return true;
		}
	}

	return false;

}

void Player::AttackHit()
{

	SubHp(10);
	ChangeState(STATE::HIT);

}

void Player::AttackHitFly()
{
}

void Player::Move()
{

	// ヒット中は行動できない
	if (state_ == STATE::HIT)return;

	// 入力方向
	VECTOR dir = inputController_->Dir();

	// 攻撃中は移動できない
	if (!GetAttackState())
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

}

void Player::Attack()
{

	// ヒット中は行動できない
	// 攻撃の先行入力
	if (inputController_->ComboAttack())
	{
		// コンボの先行入力の処理
		for (auto& data : isCombo_)
		{
			if (!data.second)
			{
				data.second = true;
				break;
			}
		}

		// ジャブに遷移
		if (isCombo_.at(STATE::JAB) && !isCombo_.at(STATE::STRAIGHT))
		{
			ChangeState(STATE::JAB);
		}

	}

	// アッパーに遷移
	if (inputController_->Upper() && !animationController_->IsPlayNowAnimation())
	{
		ChangeState(STATE::UPPER);
	}


	//コンボ中か判定
	if (!GetComboState())return;

	// 次の入力がなければコンボをキャンセルする
	for(int i = static_cast<int>(STATE::JAB); i < static_cast<int>(STATE::RIGHT_KICK); i++)
	{

		// 次の状態の入力を見るための計算
		const int nextState = static_cast<int>(state_) + 1;

		// falseだったらコンボをキャンセル
		if (!isCombo_.at(static_cast<STATE>(nextState)))
		{
			// 待機状態に遷移
			if (animationController_->IsEndPlayAnimation())
			{
				ChangeState(STATE::IDLE);
				return;
			}
		}

	}

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

	// 右手の攻撃判定をなくす
	collisionData_.isRightHandAttack = false;

	// 左手の攻撃判定をなくす
	collisionData_.isLeftHandAttack = false;

	// 右足の攻撃判定をなくす
	collisionData_.isRightFootAttack = false;

	// 左足の攻撃判定をなくす
	collisionData_.isLeftFootAttack = false;

	// 攻撃が当たっているかをリセットする
	isAttackHit_ = false;

}

void Player::ChangeRun(void)
{
	stateUpdate_ = std::bind(&Player::UpdateRun, this);
}

void Player::ChangeJab()
{

	stateUpdate_ = std::bind(&Player::UpdateJab, this);

	// どれだけ進むか計算
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// 入力受付時間をリセット
	acceptCnt_ = 0.0f;

	// 攻撃が当たっているかをリセットする
	isAttackHit_ = false;

}

void Player::ChangeStraight()
{

	stateUpdate_ = std::bind(&Player::UpdateStraight, this);

	// どれだけ進むか計算
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// 入力受付時間をリセット
	acceptCnt_ = 0.0f;

	// 攻撃が当たっているかをリセットする
	isAttackHit_ = false;

}

void Player::ChangeHook()
{

	stateUpdate_ = std::bind(&Player::UpdateHook, this);

	// どれだけ進むか計算
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// 入力受付時間をリセット
	acceptCnt_ = 0.0f;

	// 攻撃が当たっているかをリセットする
	isAttackHit_ = false;

}

void Player::ChangeLeftKick()
{

	stateUpdate_ = std::bind(&Player::UpdateLeftKick, this);

	// どれだけ進むか計算
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// 入力受付時間をリセット
	acceptCnt_ = 0.0f;

	// 攻撃が当たっているかをリセットする
	isAttackHit_ = false;

}

void Player::ChangeRightKick()
{

	stateUpdate_ = std::bind(&Player::UpdateRightKick, this);

	// どれだけ進むか計算
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// 入力受付時間をリセット
	acceptCnt_ = 0.0f;

	// 攻撃が当たっているかをリセットする
	isAttackHit_ = false;

}

void Player::ChangeUpper()
{

	stateUpdate_ = std::bind(&Player::UpdateUpper, this);

	// どれだけ進むか計算
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// 入力受付時間をリセット
	acceptCnt_ = 0.0f;

	// 攻撃が当たっているかをリセットする
	isAttackHit_ = false;

}

void Player::ChangeHit()
{

	stateUpdate_ = std::bind(&Player::UpdateHit, this);

	// プレイヤーの方向を求める
	VECTOR vec = VSub(targetPos_, transform_.pos);

	// 正規化
	vec = VNorm(vec);

	// プレイヤーの方向と逆方向のベクトル
	vec = { -vec.x, vec.y,-vec.z };

	// 移動量
	movePow_ = VAdd(transform_.pos, VScale(vec, ATTACK_MOVE_POW));

}

void Player::UpdateIdle(void)
{

	// 攻撃の入力を初期化
	for (int i = static_cast<int>(STATE::JAB); i < static_cast<int>(STATE::MAX); i++)
	{
		isCombo_.at(static_cast<STATE>(i)) = false;
	}

}

void Player::UpdateRun(void)
{

	// カメラの角度
	VECTOR cameraAngle = SceneManager::GetInstance().GetCamera().lock()->GetAngle();

	// Y軸の行列
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotY(cameraAngle.y));

	// 回転行列を使用して、ベクトルを回転させる
	moveDir_ = VTransform(dir_, mat);

	// 正規化
	moveDir_ = VNorm(moveDir_);

	// 移動量
	speed_ = 100.0f;

	// 移動量
	movePow_ = VScale(moveDir_, speed_);

	// 移動処理
	transform_.pos = VAdd(transform_.pos, movePow_);

	// 方向を角度に変換する(XZ平面 Y軸)
	float angle = atan2f(dir_.x, dir_.z);

	// プレイヤーにカメラを追従するときはこっちに切り替える
	LazyRotation(cameraAngle.y + angle);

}

void Player::UpdateJab()
{

	// 少し前にゆっくり移動
	transform_.pos = Utility::Lerp(transform_.pos, movePow_, 0.1f);

	// 攻撃判定があるフレーム
	if (JAB_ATTACK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= JAB_ATTACK_END_FRAME)
	{
		collisionData_.isLeftHandAttack = true;
	}
	else
	{
		collisionData_.isLeftHandAttack = false;
	}

	// ストレートに遷移
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(STATE::STRAIGHT))
	{
		ChangeState(STATE::STRAIGHT);
	}

}

void Player::UpdateStraight()
{

	// 少し前にゆっくり移動
	transform_.pos = Utility::Lerp(transform_.pos, movePow_, 0.1f);

	// 攻撃判定があるフレーム
	if (STRAIGHT_ATTACK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= STRAIGHT_ATTACK_END_FRAME)
	{
		collisionData_.isRightHandAttack = true;
	}
	else
	{
		collisionData_.isRightHandAttack = false;
	}

	// フックに遷移
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(STATE::HOOK))
	{
		ChangeState(STATE::HOOK);
	}

}

void Player::UpdateHook()
{

	// 少し前にゆっくり移動
	transform_.pos = Utility::Lerp(transform_.pos, movePow_, 0.1f);

	// 攻撃判定があるフレーム
	if (HOOK_ATTACK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= HOOK_ATTACK_END_FRAME)
	{
		collisionData_.isLeftHandAttack = true;
	}
	else
	{
		collisionData_.isLeftHandAttack = false;
	}

	// 左キックに遷移
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(STATE::LEFT_KICK))
	{
		ChangeState(STATE::LEFT_KICK);
	}

}

void Player::UpdateLeftKick()
{

	// 少し前にゆっくり移動
	transform_.pos = Utility::Lerp(transform_.pos, movePow_, 0.1f);

	// 攻撃判定があるフレーム
	if (LEFT_KICK_ATTACK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= LEFT_KICK_ATTACK_END_FRAME)
	{
		collisionData_.isLeftFootAttack = true;
	}
	else
	{
		collisionData_.isLeftFootAttack = false;
	}

	// 右キックに遷移
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(STATE::RIGHT_KICK))
	{
		ChangeState(STATE::RIGHT_KICK);
	}

}

void Player::UpdateRightKick()
{

	// 少し前にゆっくり移動
	transform_.pos = Utility::Lerp(transform_.pos, movePow_, 0.1f);

	// 攻撃判定があるフレーム
	if (RIGHT_KICK_ATTACK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= RIGHT_KICK_ATTACK_END_FRAME)
	{
		collisionData_.isRightFootAttack = true;
	}
	else
	{
		collisionData_.isRightFootAttack = false;
	}

	// 待機状態に遷移
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(STATE::IDLE);
	}

}

void Player::UpdateUpper()
{

	// 少し前にゆっくり移動
	transform_.pos = Utility::Lerp(transform_.pos, movePow_, 0.1f);

	// 攻撃判定があるフレーム
	if (UPPER_ATTACK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= UPPER_ATTACK_END_FRAME)
	{
		collisionData_.isRightHandAttack = true;
	}
	else
	{
		collisionData_.isRightHandAttack = false;
	}

	// 待機状態に遷移
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(STATE::IDLE);
	}

}

void Player::UpdateHit()
{

	// 待機状態に遷移
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(STATE::IDLE);
	}

}
