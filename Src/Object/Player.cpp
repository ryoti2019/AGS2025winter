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
	JAB_MOVE_START_FRAME(data["ATTACK_MOVE_FRAME"][static_cast<int>(ATTACK_STATE::JAB)]["START"]),
	JAB_MOVE_END_FRAME(data["ATTACK_MOVE_FRAME"][static_cast<int>(ATTACK_STATE::JAB)]["END"]),
	STRAIGHT_MOVE_START_FRAME(data["ATTACK_MOVE_FRAME"][static_cast<int>(ATTACK_STATE::STRAIGHT)]["START"]),
	STRAIGHT_MOVE_END_FRAME(data["ATTACK_MOVE_FRAME"][static_cast<int>(ATTACK_STATE::STRAIGHT)]["END"]),
	HOOK_MOVE_START_FRAME(data["ATTACK_MOVE_FRAME"][static_cast<int>(ATTACK_STATE::HOOK)]["START"]),
	HOOK_MOVE_END_FRAME(data["ATTACK_MOVE_FRAME"][static_cast<int>(ATTACK_STATE::HOOK)]["END"]),
	LEFT_KICK_MOVE_START_FRAME(data["ATTACK_MOVE_FRAME"][static_cast<int>(ATTACK_STATE::LEFT_KICK)]["START"]),
	LEFT_KICK_MOVE_END_FRAME(data["ATTACK_MOVE_FRAME"][static_cast<int>(ATTACK_STATE::LEFT_KICK)]["END"]),
	RIGHT_KICK_MOVE_START_FRAME(data["ATTACK_MOVE_FRAME"][static_cast<int>(ATTACK_STATE::RIGHT_KICK)]["START"]),
	RIGHT_KICK_MOVE_END_FRAME(data["ATTACK_MOVE_FRAME"][static_cast<int>(ATTACK_STATE::RIGHT_KICK)]["END"])
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
	ComboAttack(deltaTime);

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

	for (const auto state : attackState_)
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

	for (const auto state : comboState_)
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

void Player::ComboAttack(const float deltaTime)
{

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

}

void Player::ChangeStraight()
{

	stateUpdate_ = std::bind(&Player::UpdateStraight, this);

	// どれだけ進むか計算
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// 入力受付時間をリセット
	acceptCnt_ = 0.0f;

}

void Player::ChangeHook()
{

	stateUpdate_ = std::bind(&Player::UpdateHook, this);

	// どれだけ進むか計算
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// 入力受付時間をリセット
	acceptCnt_ = 0.0f;

}

void Player::ChangeLeftKick()
{

	stateUpdate_ = std::bind(&Player::UpdateLeftKick, this);

	// どれだけ進むか計算
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// 入力受付時間をリセット
	acceptCnt_ = 0.0f;

}

void Player::ChangeRightKick()
{

	stateUpdate_ = std::bind(&Player::UpdateRightKick, this);

	// どれだけ進むか計算
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// 入力受付時間をリセット
	acceptCnt_ = 0.0f;

}

void Player::ChangeUpper()
{

	stateUpdate_ = std::bind(&Player::UpdateUpper, this);

	// どれだけ進むか計算
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// 入力受付時間をリセット
	acceptCnt_ = 0.0f;

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

	// 待機状態に遷移
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(STATE::IDLE);
	}

}

void Player::UpdateUpper()
{
}
