#include "../Lib/ImGui/imgui.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "TitlePlayer.h"

TitlePlayer::TitlePlayer(const VECTOR& pos, const json& data)
	:
	ActorBase(pos, data)
{

	// 機能の初期化
	InitFunction();

	// モデルID
	modelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::MODEL_PLAYER)]);

	// 共通部分は基底クラスで初期化
	ActorBase::Init(pos);

	// パラメータの初期化
	InitParameter();

	// アニメーションの初期化
	InitAnimation();

}

void TitlePlayer::Init(const VECTOR& pos)
{

	// 描画用のコンポーネント
	drawComponent_ = std::make_unique<DrawComponent>(std::static_pointer_cast<TitlePlayer>(GetThis()));

}

void TitlePlayer::Update(const float deltaTime)
{

	// ImGuiのデバッグ描画の更新
	UpdateDebugImGui();

	// モデル情報を更新
	transform_->Update();

	// アニメーション再生
	animationController_->Update(deltaTime);

}

void TitlePlayer::InitFunction()
{
	// カメラを生成
	std::weak_ptr<Camera> camera = SceneManager::GetInstance().GetCamera();

	// カメラのターゲットをプレイヤーに設定
	camera.lock()->SetPlayer(transform_);
}

void TitlePlayer::InitParameter()
{

	// アニメーション番号
	ANIM_INDEX = jsonData_["ANIM_INDEX"];

}

void TitlePlayer::InitAnimation()
{

	// アニメーションコントローラの生成
	animationController_ = std::make_unique<AnimationController>(transform_->modelId);

	// アニメーションコントローラーにアニメーションを追加
	for (int i = static_cast<int>(PlayerState::IDLE); i < static_cast<int>(PlayerState::MAX); ++i)
	{

		// ループ再生するアニメーションだけisLoopをtrueにする
		bool isLoop = i == static_cast<int>(PlayerState::IDLE) || i == static_cast<int>(PlayerState::RUN) || i == static_cast<int>(PlayerState::TITLE_IDLE);
		animationController_->Add(

			// アニメーションの名前
			jsonData_["ANIM"][i - 1]["NAME"],

			// アニメーションが始まる時間
			0.0f,

			// アニメーションスピード
			jsonData_["ANIM"][i - 1]["SPEED"],

			// アニメーションハンドル
			resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::MODEL_PLAYER) + i]),

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
	ChangeState(PlayerState::TITLE_IDLE);

}

void TitlePlayer::UpdateDebugImGui()
{

	// ウィンドウタイトル&開始処理
	ImGui::Begin("Player");

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

void TitlePlayer::ChangeState(const PlayerState state)
{

	// 状態遷移
	state_ = state;

	// 前のアニメーションを保存
	preKey_ = key_;

	// 新しいアニメーションを保存
	key_ = ANIM_DATA_KEY[static_cast<int>(state)];

	// アニメーションコントローラー側のアニメーション遷移
	animationController_->ChangeAnimation(key_);

}
