#include <memory>
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
	JAB_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::JAB) - 1]["ATTACK_START_FRAME"]),
	JAB_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::JAB) - 1]["ATTACK_END_FRAME"]),
	JAB_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::JAB) - 1]["DAMAGE"]),
	STRAIGHT_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::STRAIGHT) - 1]["ATTACK_START_FRAME"]),
	STRAIGHT_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::STRAIGHT) - 1]["ATTACK_END_FRAME"]),
	STRAIGHT_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::STRAIGHT) - 1]["DAMAGE"]),
	HOOK_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::HOOK) - 1]["ATTACK_START_FRAME"]),
	HOOK_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::HOOK) - 1]["ATTACK_END_FRAME"]),
	HOOK_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::HOOK) - 1]["DAMAGE"]),
	LEFT_KICK_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::LEFT_KICK) - 1]["ATTACK_START_FRAME"]),
	LEFT_KICK_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::LEFT_KICK) - 1]["ATTACK_END_FRAME"]),
	LEFT_KICK_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::LEFT_KICK) - 1]["DAMAGE"]),
	RIGHT_KICK_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::RIGHT_KICK) - 1]["ATTACK_START_FRAME"]),
	RIGHT_KICK_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::RIGHT_KICK) - 1]["ATTACK_END_FRAME"]),
	RIGHT_KICK_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::RIGHT_KICK) - 1]["DAMAGE"]),
	UPPER_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::UPPER) - 1]["ATTACK_START_FRAME"]),
	UPPER_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::UPPER) - 1]["ATTACK_END_FRAME"]),
	UPPER_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::UPPER) - 1]["DAMAGE"])
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
	stateChange_.emplace(PlayerState::IDLE, std::bind(&Player::ChangeIdle, this));
	stateChange_.emplace(PlayerState::RUN, std::bind(&Player::ChangeRun, this));
	stateChange_.emplace(PlayerState::JAB, std::bind(&Player::ChangeJab, this));
	stateChange_.emplace(PlayerState::STRAIGHT, std::bind(&Player::ChangeStraight, this));
	stateChange_.emplace(PlayerState::HOOK, std::bind(&Player::ChangeHook, this));
	stateChange_.emplace(PlayerState::LEFT_KICK, std::bind(&Player::ChangeLeftKick, this));
	stateChange_.emplace(PlayerState::RIGHT_KICK, std::bind(&Player::ChangeRightKick, this));
	stateChange_.emplace(PlayerState::UPPER, std::bind(&Player::ChangeUpper, this));
	stateChange_.emplace(PlayerState::HIT_HEAD, std::bind(&Player::ChangeHitHead, this));
	stateChange_.emplace(PlayerState::HIT_BODY, std::bind(&Player::ChangeHitBody, this));

}

void Player::InitParameter()
{

	// 攻撃の入力を初期化
	for (int i = static_cast<int>(PlayerState::JAB); i < static_cast<int>(PlayerState::MAX); i++)
	{
		isCombo_.emplace(static_cast<PlayerState>(i), false);
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

	// 走るときの移動量
	RUN_MOVE_POW = jsonData_["RUN_MOVE_POW"];

	// アニメーション番号
	ANIM_INDEX = jsonData_["ANIM_INDEX"];

}

void Player::InitAnimation()
{

	// アニメーションコントローラの生成
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);

	// アニメーションコントローラーにアニメーションを追加
	for (int i = static_cast<int>(PlayerState::IDLE); i < static_cast<int>(PlayerState::MAX); ++i)
	{

		// ループ再生するアニメーションだけisLoopをtrueにする
		bool isLoop = i == static_cast<int>(PlayerState::IDLE) || i == static_cast<int>(PlayerState::RUN);
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
			ANIM_INDEX,

			// アニメーションの逆再生
			false
		);
	}

	// アニメーション再生するキー
	key_ = "IDLE";

	// 1個前のアニメーション
	preKey_ = key_;

	// 初期状態
	ChangeState(PlayerState::IDLE);

}

void Player::Update(const float deltaTime)
{

	// ImGuiのデバッグ描画の更新
	UpdateDebugImGui();

	// 移動処理
	Move();

	// 攻撃処理
	Attack();

	// 重力
	Gravity(gravityScale_);

	// 入力受付時間をカウント
	acceptCnt_++;

	// 状態ごとの更新
	stateUpdate_();

	// アニメーション再生
	animationController_->Update(deltaTime);

	// モデル情報を更新
	transform_.Update();

	// 衝突判定の更新
	ActorBase::CollisionUpdate();
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

void Player::AttackHit(const int damage, const int state)
{

	// どのアニメーションかチェックする
	AttackHitCheck(state);

	// HPを減らす
	SubHp(damage);
	
	// アニメーションの再生時間をリセットする
	animationController_->ResetStepAnim();

}

void Player::AttackHitCheck(const int state)
{

	// 頭にヒットするアニメーションかチェック
	for (const auto hitState : hitHeadState_)
	{
		if (hitState == static_cast<EnemyState>(state))
		{
			ChangeState(PlayerState::HIT_HEAD);
			return;
		}
	}

	// 体にヒットするアニメーションかチェック
	for (const auto hitState : hitBodyState_)
	{
		if (hitState == static_cast<EnemyState>(state))
		{
			ChangeState(PlayerState::HIT_BODY);
			return;
		}
	}

}

void Player::Move()
{

	// ヒット中は行動できない
	for (const auto hitState : hitState_)
	{
		if (hitState == state_)
		{
			return;
		}
	}

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

			ChangeState(PlayerState::RUN);
		}
		// 入力していなければ待機状態にする
		else if (Utility::EqualsVZero(dir))
		{
			ChangeState(PlayerState::IDLE);
		}
	}

}

void Player::Attack()
{

	// ヒット中は行動できない
	for (const auto hitState : hitState_)
	{
		if (hitState == state_)
		{
			return;
		}
	}

	// 攻撃の先行入力
	if (inputController_->ComboAttack())
	{
		// コンボの先行入力の処理
		for (int i = static_cast<int>(PlayerState::JAB); i <= static_cast<int>(PlayerState::RIGHT_KICK); i++)
		{

			// 今から判断するものを探す
			auto isCombo = isCombo_.find(static_cast<PlayerState>(i));

			// falseであればtrueにして先行入力を保持する
			if (!isCombo->second)
			{
				isCombo->second = true;
				break;
			}
		}

		// ジャブに遷移
		if (isCombo_.at(PlayerState::JAB) && !isCombo_.at(PlayerState::STRAIGHT))
		{
			ChangeState(PlayerState::JAB);
		}

	}

	// アッパーに遷移
	if (inputController_->Upper() && state_ != PlayerState::UPPER)
	{
		ChangeState(PlayerState::UPPER);
	}

	//コンボ中か判定
	if (!GetComboState())return;

	// 次の入力がなければコンボをキャンセルする
	for(int i = static_cast<int>(PlayerState::JAB); i < static_cast<int>(PlayerState::RIGHT_KICK); i++)
	{

		// 次の状態の入力を見るための計算
		const int nextState = static_cast<int>(state_) + 1;

		// falseだったらコンボをキャンセル
		if (!isCombo_.at(static_cast<PlayerState>(nextState)))
		{
			// 待機状態に遷移
			if (animationController_->IsEndPlayAnimation())
			{
				ChangeState(PlayerState::IDLE);
				return;
			}
		}

	}

}

void Player::ChangeState(PlayerState state)
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

	// 重力を通常状態に戻す
	gravityScale_ = 1.0f;

}

void Player::ChangeRun(void)
{
	stateUpdate_ = std::bind(&Player::UpdateRun, this);
}

void Player::ChangeJab()
{

	stateUpdate_ = std::bind(&Player::UpdateJab, this);

	// 入力受付時間をリセット
	acceptCnt_ = 0.0f;

	// 攻撃が当たっているかをリセットする
	isAttackHit_ = false;

	// ダメージ量
	damage_ = JAB_DAMAGE;

	// スピード
	speed_ = ATTACK_MOVE_POW;

	// どれだけ進むか計算
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, speed_));

}

void Player::ChangeStraight()
{

	stateUpdate_ = std::bind(&Player::UpdateStraight, this);

	// 入力受付時間をリセット
	acceptCnt_ = 0.0f;

	// 攻撃が当たっているかをリセットする
	isAttackHit_ = false;

	// ダメージ量
	damage_ = STRAIGHT_DAMAGE;

	// スピード
	speed_ = ATTACK_MOVE_POW;

	// どれだけ進むか計算
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, speed_));

}

void Player::ChangeHook()
{

	stateUpdate_ = std::bind(&Player::UpdateHook, this);

	// 入力受付時間をリセット
	acceptCnt_ = 0.0f;

	// 攻撃が当たっているかをリセットする
	isAttackHit_ = false;

	// ダメージ量
	damage_ = HOOK_DAMAGE;

	// スピード
	speed_ = ATTACK_MOVE_POW;

	// どれだけ進むか計算
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, speed_));

}

void Player::ChangeLeftKick()
{

	stateUpdate_ = std::bind(&Player::UpdateLeftKick, this);

	// 入力受付時間をリセット
	acceptCnt_ = 0.0f;

	// 攻撃が当たっているかをリセットする
	isAttackHit_ = false;

	// ダメージ量
	damage_ = LEFT_KICK_DAMAGE;

	// スピード
	speed_ = ATTACK_MOVE_POW;

	// どれだけ進むか計算
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, speed_));

}

void Player::ChangeRightKick()
{

	stateUpdate_ = std::bind(&Player::UpdateRightKick, this);

	// 入力受付時間をリセット
	acceptCnt_ = 0.0f;

	// 攻撃が当たっているかをリセットする
	isAttackHit_ = false;

	// ダメージ量
	damage_ = RIGHT_KICK_DAMAGE;

	// スピード
	speed_ = ATTACK_MOVE_POW;

	// どれだけ進むか計算
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, speed_));

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

	// ダメージ量
	damage_ = UPPER_DAMAGE;

	// スピード
	speed_ = ATTACK_MOVE_POW;

	// どれだけ進むか計算
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, speed_));

}

void Player::ChangeHitHead()
{

	stateUpdate_ = std::bind(&Player::UpdateHitHead, this);

	// プレイヤーの方向を求める
	VECTOR vec = VSub(targetPos_, transform_.pos);

	// 正規化
	vec = VNorm(vec);

	// プレイヤーの方向と逆方向のベクトル
	vec = { -vec.x, vec.y,-vec.z };

	// スピード
	speed_ = ATTACK_MOVE_POW;

	// どれだけ進むか計算
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, speed_));

}

void Player::ChangeHitBody()
{

	stateUpdate_ = std::bind(&Player::UpdateHitBody, this);

	// プレイヤーの方向を求める
	VECTOR vec = VSub(targetPos_, transform_.pos);

	// 正規化
	vec = VNorm(vec);

	// プレイヤーの方向と逆方向のベクトル
	vec = { -vec.x, vec.y,-vec.z };

	// スピード
	speed_ = ATTACK_MOVE_POW;

	// どれだけ進むか計算
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, speed_));

}

void Player::UpdateIdle(void)
{

	// 攻撃の入力を初期化
	for (int i = static_cast<int>(PlayerState::JAB); i < static_cast<int>(PlayerState::MAX); i++)
	{
		isCombo_.at(static_cast<PlayerState>(i)) = false;
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

	// スピード
	speed_ = RUN_MOVE_POW;

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
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(PlayerState::STRAIGHT))
	{
		ChangeState(PlayerState::STRAIGHT);
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
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(PlayerState::HOOK))
	{
		ChangeState(PlayerState::HOOK);
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
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(PlayerState::LEFT_KICK))
	{
		ChangeState(PlayerState::LEFT_KICK);
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
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(PlayerState::RIGHT_KICK))
	{
		ChangeState(PlayerState::RIGHT_KICK);
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
		ChangeState(PlayerState::IDLE);
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
		ChangeState(PlayerState::IDLE);
	}

}

void Player::UpdateHitHead()
{

	// 待機状態に遷移
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);
	}

}

void Player::UpdateHitBody()
{

	// 待機状態に遷移
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);
	}

}
