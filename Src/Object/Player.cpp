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
	stateChange_.emplace(STATE::HOOK, std::bind(&Player::ChangeHook, this));
	stateChange_.emplace(STATE::LEFT_KICK, std::bind(&Player::ChangeLeftKick, this));
	stateChange_.emplace(STATE::RIGHT_KICK, std::bind(&Player::ChangeRightKick, this));
	stateChange_.emplace(STATE::UPPER, std::bind(&Player::ChangeUpper, this));
}

void Player::InitParameter()
{

	// ジャブ
	jab_ = false;

	// ストレート
	straight_ = false;

	// フック
	hook_ = false;

	// 左キック
	leftKick_ = false;

	// 右キック
	rightKick_ = false;

	// アッパー
	upper_ = false;

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

	ANIM_ACCEPT_TIME data;
	for (int i = static_cast<int>(STATE::JAB); i < static_cast<int>(STATE::MAX); ++i)
	{

		// アニメーションの状態
		data.key = static_cast<STATE>(i);

		// アニメーションの受付開始時間
		data.animAcceptStartTime = jsonData_["ANIM"][i-1]["ACCEPT_START"];

		// アニメーションの受付終了時間
		data.animAcceptEndTime = jsonData_["ANIM"][i - 1]["ACCEPT_END"];

		// アニメーションの受付時間を追加
		animAcceptTime_.emplace_back(data);

	}

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

	// コンボ中に次の攻撃がなかったら待機状態に戻す
	for (const auto& stateData : animAcceptTime_)
	{
		if (state_ != stateData.key)continue;
		if (acceptCnt_ >= stateData.animAcceptEndTime)
		{
			ChangeState(STATE::IDLE);
			acceptCnt_ = 0.0f;
		}
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
		// フック
		else if (state_ == STATE::STRAIGHT)
		{
			hook_ = true;
		}
		// 左キック
		else if (state_ == STATE::HOOK)
		{
			leftKick_ = true;
		}
		// 右キック
		else if (state_ == STATE::LEFT_KICK)
		{
			rightKick_ = true;
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
	transform_.pos = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// 入力受付時間をリセット
	acceptCnt_ = 0.0f;

}

void Player::ChangeStraight()
{

	stateUpdate_ = std::bind(&Player::UpdateStraight, this);

	// 少し前に移動
	transform_.pos = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// 入力受付時間をリセット
	acceptCnt_ = 0.0f;

}

void Player::ChangeHook()
{

	stateUpdate_ = std::bind(&Player::UpdateHook, this);

	// 少し前に移動
	transform_.pos = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// 入力受付時間をリセット
	acceptCnt_ = 0.0f;

}

void Player::ChangeLeftKick()
{

	stateUpdate_ = std::bind(&Player::UpdateLeftKick, this);

	// 少し前に移動
	transform_.pos = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// 入力受付時間をリセット
	acceptCnt_ = 0.0f;

}

void Player::ChangeRightKick()
{

	stateUpdate_ = std::bind(&Player::UpdateRightKick, this);

	// 少し前に移動
	transform_.pos = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// 入力受付時間をリセット
	acceptCnt_ = 0.0f;

}

void Player::ChangeUpper()
{

	stateUpdate_ = std::bind(&Player::UpdateUpper, this);

	// 少し前に移動
	transform_.pos = VAdd(transform_.pos, VScale(moveDir_, ATTACK_MOVE_POW));

	// 入力受付時間をリセット
	acceptCnt_ = 0.0f;

}

void Player::UpdateIdle(void)
{
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
	// ストレートに遷移
	if (animationController_->IsPreEndPlayAnimation() && straight_)
	{
		straight_ = false;
		ChangeState(STATE::STRAIGHT);
	}
}

void Player::UpdateStraight()
{
	// フックに遷移
	if (animationController_->IsPreEndPlayAnimation() && hook_)
	{
		hook_ = false;
		ChangeState(STATE::HOOK);
	}
}

void Player::UpdateHook()
{
	// 左キックに遷移
	if (animationController_->IsPreEndPlayAnimation() && leftKick_)
	{
		leftKick_ = false;
		ChangeState(STATE::LEFT_KICK);
	}
}

void Player::UpdateLeftKick()
{
	// 右キックに遷移
	if (animationController_->IsPreEndPlayAnimation() && rightKick_)
	{
		rightKick_ = false;
		ChangeState(STATE::RIGHT_KICK);
	}
}

void Player::UpdateRightKick()
{
}

void Player::UpdateUpper()
{
}
