#include "../Lib/ImGui/imgui.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "BossAppearanceBoss.h"

BossAppearanceBoss::BossAppearanceBoss(const VECTOR& pos, const json& data)
	:
	EnemyBase(pos, data),
	isLand_(false)
{

	// 機能の初期化
	InitComponent();

	// モデルID
	modelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::MODEL_BOSS)]);

	// 関数ポインタの初期化
	InitFunctionPointer();

	// 共通部分は基底クラスで初期化
	ActorBase::Init(pos);

	// パラメータの初期化
	InitParameter();

	// アニメーションの初期化
	InitAnimation();

}

void BossAppearanceBoss::Init(const VECTOR& pos)
{

	// 描画用のコンポーネント
	drawComponent_ = std::make_unique<DrawComponent>(std::static_pointer_cast<BossAppearanceBoss>(GetThis()));

}

void BossAppearanceBoss::InitComponent()
{

	// カメラを生成
	std::weak_ptr<Camera> camera = SceneManager::GetInstance().GetCamera();

	// カメラのターゲットをプレイヤーに設定
	camera.lock()->SetBoss(transform_);

}

void BossAppearanceBoss::InitParameter()
{

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
	collisionData_.rightHand = MV1SearchFrame(transform_->modelId, RIGHT_HAND_FRAME.c_str());

	// 左手のフレーム番号を取得
	collisionData_.leftHand = MV1SearchFrame(transform_->modelId, LEFT_HAND_FRAME.c_str());

	// 右足のフレーム番号を取得
	collisionData_.rightFoot = MV1SearchFrame(transform_->modelId, RIGHT_FOOT_FRAME.c_str());

	// 左足のフレーム番号を取得
	collisionData_.leftFoot = MV1SearchFrame(transform_->modelId, LEFT_FOOT_FRAME.c_str());

	// 体のフレーム番号を取得
	collisionData_.body = MV1SearchFrame(transform_->modelId, BODY_FRAME.c_str());

	// 手足の衝突判定の半径
	collisionData_.handAndFootCollisionRadius = HAND_AND_FOOT_COLLISION_RADIUS;

	// 体の衝突判定の半径
	collisionData_.bodyCollisionRadius = BODY_COLLISION_RADIUS;

	// アニメーション番号
	ANIM_INDEX = jsonData_["ANIM_INDEX"];

}

void BossAppearanceBoss::InitAnimation()
{

	// アニメーションコントローラーの生成
	animationController_ = std::make_unique<AnimationController>(transform_->modelId);

	// アニメーションコントローラーにアニメーションを追加
	for (int i = static_cast<int>(BossState::IDLE); i < static_cast<int>(BossState::MAX); ++i)
	{

		// ループ再生するアニメーションだけisLoopをtrueにする
		bool isLoop = i == static_cast<int>(BossState::IDLE) || i == static_cast<int>(BossState::RUN) || i == static_cast<int>(BossState::JUMP_IDLE);
		animationController_->Add(

			// アニメーションの名前
			jsonData_["ANIM"][i - 1]["NAME"],

			// アニメーションが始まる時間
			0.0f,

			// アニメーションスピード
			jsonData_["ANIM"][i - 1]["SPEED"],

			// アニメーションハンドル
			resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::MODEL_BOSS) + i]).handleId_,

			// アニメーションのループ再生
			isLoop,

			// アニメーション番号
			ANIM_INDEX,

			// アニメーションの逆再生
			false
		);
	}

	// アニメーション再生するキー
	key_ = "";

	// 1個前のアニメーション
	preKey_ = "";

	// 初期状態
	ChangeState(BossState::IDLE);

}

void BossAppearanceBoss::InitFunctionPointer()
{

	//関数ポインタの初期化
	stateChange_.emplace(BossState::IDLE, std::bind(&BossAppearanceBoss::ChangeIdle, this));
	stateChange_.emplace(BossState::JUMP_IDLE, std::bind(&BossAppearanceBoss::ChangeJumpIdle, this));
	stateChange_.emplace(BossState::LAND, std::bind(&BossAppearanceBoss::ChangeLand, this));

}

void BossAppearanceBoss::Update(const float deltaTime)
{

	// ImGuiのデバッグ描画の更新
	//UpdateDebugImGui();

	// 衝突判定の更新
	ActorBase::CollisionUpdate();

	// 状態ごとの更新
	stateUpdate_(deltaTime);

	// 重力
	Gravity(gravityScale_);

	// モデル情報を更新
	transform_->Update();

	// アニメーション再生
	animationController_->Update(deltaTime);

}

void BossAppearanceBoss::ChangeState(const BossState state)
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

void BossAppearanceBoss::ChangeIdle()
{
	stateUpdate_ = std::bind(&BossAppearanceBoss::UpdateIdle, this, std::placeholders::_1);
}

void BossAppearanceBoss::ChangeJumpIdle()
{

	stateUpdate_ = std::bind(&BossAppearanceBoss::UpdateJumpIdle, this, std::placeholders::_1);

	// 座標を設定
	transform_->pos = { -10800.0f,11000.0f,-140000.0f };

	// 衝突判定の更新
	ActorBase::CollisionUpdate();

	gravityScale_ = 1.0f;

}

void BossAppearanceBoss::ChangeLand()
{
	stateUpdate_ = std::bind(&BossAppearanceBoss::UpdateLand, this, std::placeholders::_1);
}

void BossAppearanceBoss::UpdateIdle(const float deltaTime)
{

	// カメラを生成
	std::weak_ptr<Camera> camera = SceneManager::GetInstance().GetCamera();

	// カメラの1つ目と2つ目の動きが終わっているか
	if (!camera.lock()->GetIsBossAppearanceCameraMove1() && !camera.lock()->GetIsBossAppearanceCameraMove2() && !isLand_)
	{
		// ジャンプに遷移
		ChangeState(BossState::JUMP_IDLE);
	}

}

void BossAppearanceBoss::UpdateJumpIdle(const float deltaTime)
{

	// 床についているか
	if (isOnGround_)
	{
		// 起き上がりアニメーションに遷移
		ChangeState(BossState::LAND);
	}

}

void BossAppearanceBoss::UpdateLand(const float deltaTime)
{

	// アニメーション再生が終わっているか
	if (animationController_->IsEndPlayAnimation())
	{

		// 待機に遷移
		ChangeState(BossState::IDLE);

		// 着地した
		isLand_ = true;

	}

}

void BossAppearanceBoss::UpdateDebugImGui()
{

	// ウィンドウタイトル&開始処理
	ImGui::Begin("Boss");

	// 大きさ
	ImGui::Text("scale");
	ImGui::InputFloat("Scl", &scl_);

	// 角度
	VECTOR rotDeg = VECTOR();
	rotDeg.x = Utility::Rad2DegF(transform_->quaRot.x);
	rotDeg.y = Utility::Rad2DegF(transform_->quaRot.y);
	rotDeg.z = Utility::Rad2DegF(transform_->quaRot.z);
	ImGui::Text("angle(deg)");
	ImGui::SliderFloat("RotX", &rotDeg.x, -90.0f, 90.0f);
	ImGui::SliderFloat("RotY", &rotDeg.y, -90.0f, 90.0f);
	ImGui::SliderFloat("RotZ", &rotDeg.z, -90.0f, 90.0f);
	transform_->quaRot.x = Utility::Deg2RadF(rotDeg.x);
	transform_->quaRot.y = Utility::Deg2RadF(rotDeg.y);
	transform_->quaRot.z = Utility::Deg2RadF(rotDeg.z);

	// 位置
	ImGui::Text("position");
	// 構造体の先頭ポインタを渡し、xyzと連続したメモリ配置へアクセス
	ImGui::InputFloat3("Pos", &transform_->pos.x);
	// 終了処理
	ImGui::End();

}