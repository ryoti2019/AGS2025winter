#include <EffekseerForDXLib.h>
#include "../Lib/ImGui/imgui.h"
#include "../Utility/Utility.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "SceneManager.h" 
#include "../Application.h"
#include "../Scene/GameScene.h"
#include "../Object/Common/Transform.h"
#include "Camera.h"

Camera::Camera()
{

	pos_ = { 0.0f,0.0f,0.0f };
	targetPos_ = { 0.0f, 0.0f, 0.0f };
	movePow_ = { 0.0f,0.0f,0.0f };
	angle_ = { 0.0f, Utility::Deg2RadF(90.0f), 0.0f};
	lockOn_ = false;
	specialMoveCnt_ = 0.0f;

	// カメラの初期設定
	SetDefault();

	// 関数ポインタの初期化
	InitFunctionPointer();

	ChangeMode(MODE::FIXED_POINT);

}

Camera::~Camera()
{
}

void Camera::Init()
{
}

void Camera::InitFunctionPointer()
{

	//関数ポインタの初期化
	modeChange_.emplace(MODE::FIXED_POINT, std::bind(&Camera::ChangeFixedPoint, this));
	modeChange_.emplace(MODE::FREE, std::bind(&Camera::ChangeFree, this));
	modeChange_.emplace(MODE::TITLE, std::bind(&Camera::ChangeTitle, this));
	modeChange_.emplace(MODE::FOLLOW, std::bind(&Camera::ChangeFollow, this));
	modeChange_.emplace(MODE::LOCKON, std::bind(&Camera::ChangeLockOn, this));
	modeChange_.emplace(MODE::SPECIAL, std::bind(&Camera::ChangeSpecial, this));
	modeChange_.emplace(MODE::APPEARANCE, std::bind(&Camera::ChangeAppearance, this));
	modeChange_.emplace(MODE::GAME_CLEAR, std::bind(&Camera::ChangeGameClear, this));

}

void Camera::Update()
{

	// ImGuiのデバッグ描画の更新
	//UpdateDebugImGui();

}

void Camera::SetBeforeDraw(const float deltaTime)
{

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	// モードごとの更新
	modeDraw_(deltaTime);

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

}

void Camera::ChangeMode(const MODE& mode)
{

	// カメラモードの変更
	mode_ = mode;

	// 関数ポインタの遷移
	modeChange_[mode_]();

}

void Camera::ChangeFixedPoint()
{
	modeDraw_ = std::bind(&Camera::SetBeforeDrawFixedPoint, this, std::placeholders::_1);

	// カメラの初期設定
	SetDefault();
	pos_ = { 0.0f,200.0f,-500.0f };
	targetPos_ = { 0.0f,150.0f,0.0f };

}

void Camera::ChangeFree()
{
	modeDraw_ = std::bind(&Camera::SetBeforeDrawFree, this, std::placeholders::_1);
}

void Camera::ChangeTitle()
{

	modeDraw_ = std::bind(&Camera::SetBeforeDrawTitle, this, std::placeholders::_1);

	targetPos_ = VAdd(playerTransform_->pos, playerTransform_->quaRot.PosAxis({-1200.0f, 1000.0f, 0.0f}));

	pos_ = VAdd(playerTransform_->pos, playerTransform_->quaRot.PosAxis({ -1200.0f,800.0f,2500.0f }));

}

void Camera::ChangeFollow()
{

	modeDraw_ = std::bind(&Camera::SetBeforeDrawFollow, this, std::placeholders::_1);

	targetPos_ = VAdd(playerTransform_->pos, playerTransform_->quaRot.PosAxis(LOCAL_P2T_POS));

	angle_.y = playerTransform_->quaRot.ToEuler().y;
	rotY_ = playerTransform_->quaRot;
	rotXY_ = playerTransform_->quaRot;
	lockOnAngles_ = { 0.0f, 0.0f, 0.0f };
}

void Camera::ChangeLockOn()
{

	modeDraw_ = std::bind(&Camera::SetBeforeDrawLockOn, this, std::placeholders::_1);

	angle_ = { 0.0f,0.0f,0.0f };
	lockOnAngles_ = { 0.0f, 0.0f, 0.0f };

}

void Camera::ChangeSpecial()
{

	modeDraw_ = std::bind(&Camera::SetBeforeDrawSpecial, this, std::placeholders::_1);

	movePow_ = { 0.0f,0.0f,0.0f };

	movedPos_ = { 0.0f,0.0f,0.0f };

	pos_ = VAdd(playerTransform_->pos, { 0.0f,1500.0f,0.0f });

	// 回転している
	targetPos_ = VAdd(playerTransform_->pos, playerTransform_->quaRot.PosAxis({ 0.0f,1000.0f,0.0f }));

	specialMoveCnt_ = 0.0f;

}

void Camera::ChangeAppearance()
{

	modeDraw_ = std::bind(&Camera::SetBeforeDrawAppearance, this, std::placeholders::_1);

	movePow_ = { 0.0f,0.0f,0.0f };

	movedPos_ = { 0.0f,0.0f,0.0f };

	// プレイヤーから見たカメラのローカル座標
	VECTOR localRotPos = bossTransform_->quaRot.PosAxis({ 500.0f,2000.0f,-2000.0f });

	// カメラが何秒移動したか計るカウンタ
	elapsedTime_ = 0.0f;

	// ボスの登場シーンの1つ目のカメラの動きのフラグ
	isBossAppearanceCameraMove1_ = true;

	// ボスの登場シーンの2つ目のカメラの動きのフラグ
	isBossAppearanceCameraMove2_ = false;

	// ボスの登場シーンの3つ目のカメラの動きのフラグ
	isBossAppearanceCameraMove3_ = false;

	// ボスの登場シーンが終わったかのフラグ
	isEndBossAppearanceScene_ = false;

	// カメラの座標を設定
	pos_ = VAdd(bossTransform_->pos, localRotPos);

	// 回転している
	targetPos_ = VAdd(bossTransform_->pos, bossTransform_->quaRot.PosAxis({ 0.0f,1000.0f,0.0f }));
	
}

void Camera::ChangeGameClear()
{

	modeDraw_ = std::bind(&Camera::SetBeforeDrawGameClear, this, std::placeholders::_1);

	targetPos_ = VAdd(playerTransform_->pos, playerTransform_->quaRot.PosAxis({ -1200.0f, 1000.0f, 0.0f }));

	pos_ = VAdd(playerTransform_->pos, playerTransform_->quaRot.PosAxis({ -1200.0f,800.0f,2500.0f }));

}

void Camera::SetBeforeDrawFixedPoint(const float deltaTime)
{
	pos_ = { 0.0f,5000.0f,-5000.0f };
}

void Camera::SetBeforeDrawFree(const float deltaTime)
{

	auto& ins = InputManager::GetInstance();

#pragma region 回転

	VECTOR axisDeg = Utility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_UP)) { axisDeg.x += -0.01f; }
	if (ins.IsNew(KEY_INPUT_DOWN)) { axisDeg.x += 0.01f; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { axisDeg.y += -0.01f; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { axisDeg.y += 0.01f; }


	if (!Utility::EqualsVZero(axisDeg))
	{
		// カメラを回転させる
		angle_.x += Utility::Deg2RadF(axisDeg.x);
		angle_.y += Utility::Deg2RadF(axisDeg.y);
	}


	rotY_ = Quaternion::AngleAxis(angle_.y, Utility::AXIS_Y);
	rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, Utility::AXIS_X));

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	VECTOR localPos = rotXY_.PosAxis(LOCAL_P2T_POS);
	targetPos_ = VAdd(bossTransform_->pos, localPos);

	//// カメラ位置
	//localPos = rotXY_.PosAxis(LOCAL_P2C_POS);
	//pos_ = VAdd(playerTransform_->pos, localPos);

	// カメラの上方向
	cameraUp_ = rotXY_.GetUp();

#pragma endregion

#pragma region 移動

	// 移動
	//--------------------------------------
	VECTOR moveDir = Utility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_Y)) { moveDir = Utility::DIR_F; }
	if (ins.IsNew(KEY_INPUT_G)) { moveDir = Utility::DIR_L; }
	if (ins.IsNew(KEY_INPUT_H)) { moveDir = Utility::DIR_B; }
	if (ins.IsNew(KEY_INPUT_J)) { moveDir = Utility::DIR_R; }
	if (ins.IsNew(KEY_INPUT_T)) { moveDir = Utility::DIR_U; }
	if (ins.IsNew(KEY_INPUT_U)) { moveDir = Utility::DIR_D; }
	//---------------------------------------

	if (!Utility::EqualsVZero(moveDir))
	{

		// 押下された移動ボタンの方向に移動
		VECTOR direction = VNorm(rotY_.PosAxis(moveDir)); // 回転させる必要がある

		// 移動量(方向 * スピード)
		VECTOR movePow = VScale(direction, SPEED);

		// 移動(座標 + 移動量)
		// カメラ位置とカメラ注視点
		pos_ = VAdd(pos_, movePow);
		targetPos_ = VAdd(targetPos_, movePow);

	}

#pragma endregion

}

void Camera::SetBeforeDrawTitle(const float deltaTime)
{
}

void Camera::SetBeforeDrawFollow(const float deltaTime)
{

	auto& ins = InputManager::GetInstance();

	auto& sce = SceneManager::GetInstance();

	// 基底クラスから使いたい型へキャストする
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	if ( gameScene && gameScene->GetIsViewUserGuide())
	{

		// 追従対象からカメラまでの相対座標
		VECTOR relativeCPos = rotXY_.PosAxis(LOCAL_P2C_POS);

		// カメラ座標
		pos_ = VAdd(playerTransform_->pos, relativeCPos);
		return;
	
	}

	// マウスでの操作
	if (!SceneManager::GetInstance().GetGamePad())
	{
		KeyboardController();
	}

	// ゲームパッドでの操作
	if (SceneManager::GetInstance().GetGamePad())
	{
		GamePadController();
	}

	//// Qキーを押したらtrueになる
	//if (isLazy_)
	//{
	//	LazyRotation();
	//}

	//// ステージの衝突判定
	//CollisionStage();

}

void Camera::SetBeforeDrawLockOn(const float deltaTime)
{

	// 同期先の位置
	VECTOR playerPos = playerTransform_->pos;

	// 敵の位置
	VECTOR enemyPos = { 0.0f,1000.0f,5000.0f };

	if (lockOn_)
	{
		enemyPos = bossTransform_->pos;
		enemyPos = VAdd(enemyPos, { 0.0f,1000.0f,0.0f });
	}

	// プレイヤーと敵の中間地点を設定
	VECTOR centerPos = VAdd(playerPos, enemyPos);

	// プレイヤーとロックオン対象の距離を計算
	float distance = VSize(VSub(playerPos, enemyPos));

	centerPos = VScale(centerPos, 0.5f);

	VECTOR pos = rotY_.PosAxis(LOCAL_P2C_POS);
	
	// カメラの位置
	pos_ = VAdd(playerPos, pos);

	// 注視点を設定
	targetPos_ = centerPos;

	// カメラの上方向
	cameraUp_ = Utility::DIR_U;

}

void Camera::SetBeforeDrawSpecial(const float deltaTime)
{

	auto& ins = InputManager::GetInstance();

	// 回転
	//-------------------------------------

	// 必殺技時のカメラの移動する時間を計算
	specialMoveCnt_ += deltaTime;

	// カメラのオフセット
	VECTOR pow = { 20.0f,0.02f,20.0f };

	// カメラが動ける時間まで移動させる
	if (SPECIAL_MOVE_MAX_TIME >= specialMoveCnt_)
	{

		// プレイヤーから見たカメラのローカル座標
		VECTOR localRotPos = playerTransform_->quaRot.PosAxis({ 500.0f,2000.0f,500.0f });

		// カメラの座標を設定
		pos_ = VAdd(playerTransform_->pos, localRotPos);

		// カメラの移動量を設定
		movePow_ = VAdd(movePow_,playerTransform_->quaRot.PosAxis(pow));

		// カメラ座標を移動させる
		pos_ = VAdd(pos_, movePow_);

	}

	// カメラの上方向
	cameraUp_ = Utility::DIR_U;

}

void Camera::SetBeforeDrawAppearance(const float deltaTime)
{

	// 敵の背中側を通っていくカメラの動き
	if (isBossAppearanceCameraMove1_)
	{
		// 移動する力
		movePow_.x = 10.0f;

		// 移動後座標
		movedPos_.x += movePow_.x;

		// 敵から見たカメラのローカル座標
		VECTOR localRotPos = bossTransform_->quaRot.PosAxis({ -1000.0f + movedPos_.x ,2000.0f,-1000.0f });

		// カメラの座標を設定
		pos_ = VAdd(bossTransform_->pos, localRotPos);

		// 注視点の座標を設定
		targetPos_ = VAdd(pos_, { 0.0f,-1000.0f,1000.0f });

		// カメラが何秒動いたか計算
		elapsedTime_ += deltaTime;

		// カメラが一定秒数動いたらカメラの動きを変える
		if (elapsedTime_ >= 3.0f)
		{
			isBossAppearanceCameraMove1_ = false;
			isBossAppearanceCameraMove2_ = true;
			elapsedTime_ = 0.0f;
		}

	}
	// 敵の足元から少し顔が映るカメラの動き
	else if (isBossAppearanceCameraMove2_)
	{

		// 移動する力
		movePow_.y = 10.0f;

		// 移動後座標
		movedPos_.y += movePow_.y;

		// 敵から見たカメラのローカル座標
		VECTOR localRotPos = bossTransform_->quaRot.PosAxis({ 300.0f ,100.0f + movedPos_.y,300.0f });

		// カメラの座標を設定
		pos_ = VAdd(bossTransform_->pos, localRotPos);

		// 注視点の座標を設定
		targetPos_ = VAdd(bossTransform_->pos, bossTransform_->quaRot.PosAxis({ 10.0f,movedPos_.y,0.0f }));

		// カメラが何秒動いたか計算
		elapsedTime_ += deltaTime;

		// カメラが一定秒数動いたらカメラの動きを変える
		if (elapsedTime_ >= 2.0f)
		{
			isBossAppearanceCameraMove2_ = false;
			isBossAppearanceCameraMove3_ = true;
		}

	}
	// 敵が地面についた時のカメラの動き
	else if (isBossAppearanceCameraMove3_)
	{

		// 敵が地面に降りてきて待機状態になるまで
		if (elapsedTime_ < 5.0f)
		{

			// プレイヤーから見たカメラのローカル座標
			VECTOR localRotPos = playerTransform_->quaRot.PosAxis({ 2000.0f ,2000.0f ,-2000.0f });

			// カメラの座標を設定
			pos_ = VAdd(playerTransform_->pos, localRotPos);

			// 注視点の座標を設定
			targetPos_ = { -10800.0f,-18000.0f,-140000.0f };
		
		}
		// 敵が地面に降りてきて待機状態になるまで
		else if (elapsedTime_ >= 5.0f && elapsedTime_ < 12.0f)
		{

			// ボスから見たカメラのローカル座標
			VECTOR localRotCameraPos = bossTransform_->quaRot.PosAxis({ 0.0f ,1000.0f ,2000.0f });

			// ボスから見た注視点のローカル座標
			VECTOR localRotTargetPos = bossTransform_->quaRot.PosAxis({ 0.0f ,1000.0f ,0.0f });

			// 徐々に敵に近づけていく
			pos_ = Utility::Lerp(pos_, VAdd(bossTransform_->pos, localRotCameraPos),0.05f);

			// 注視点
			targetPos_ = VAdd(bossTransform_->pos, localRotTargetPos);

		}
		// ボスの登場シーンが終わったらゲームシーンに戻る
		else if (elapsedTime_ >= 10.0f)
		{
			isEndBossAppearanceScene_ = true;
		}

		// カメラが何秒動いたか計算
		elapsedTime_ += deltaTime;

	}


}

void Camera::SetBeforeDrawGameClear(const float deltaTime)
{
}

void Camera::Draw()
{
}

void Camera::Release()
{
}

void Camera::SetLazyAngles(const VECTOR angles)
{

	lazyGoalRotY_ = Quaternion::Euler(0.0f, angles.y, 0.0f);
	isLazy_ = true;

}

void Camera::SetLockOn(const bool lockOn)
{
	lockOn_ = lockOn;
}

void Camera::AddLockOnAnglesY(float rad)
{
	lockOnAngles_.y += rad;
}

void Camera::SetStageID(const int modelId)
{
	stageId_ = modelId;
}

void Camera::SetDefault()
{

	// カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	// 注視点
	targetPos_ = VAdd(pos_, LOCAL_P2T_POS);

	// カメラの上方向
	cameraUp_ = { 0.0f, 1.0f, 0.0f };

	// カメラはX軸に傾いているが、
	// この傾いた状態を角度ゼロ、傾き無しとする
	rotY_ = Quaternion::AngleAxis(angle_.y, Utility::AXIS_Y);
	rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, Utility::AXIS_X));

	// ロックオン
	lockOn_ = false;

}

void Camera::SetTargetPosFollowForward()
{

	// カメラ位置(プレイヤーからの相対座標で制御)
	VECTOR localCameraPos = { 0.0f, HEIGHT, -DIS_FOLLOW2CAMERA };
	VECTOR localTargetPos = { 0.0f, 0.0f, DIS_FOLLOW2TARGET };

	// 注視点の移動
	VECTOR followPos = { 0.0f,0.0f,0.0f };

	// カメラの回転行列を作成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotX(rotXY_.x));
	mat = MMult(mat, MGetRotY(rotXY_.y));

	VECTOR localRotPos;

	// 注視点の移動
	localRotPos = VTransform(localTargetPos, mat);
	targetPos_ = VAdd(followPos, localRotPos);

	// カメラの移動
	localRotPos = VTransform(localCameraPos, mat);
	movedPos_ = VAdd(followPos, localRotPos);

}

void Camera::KeyboardController()
{

	auto& ins = InputManager::GetInstance();

	// 回転
	//-------------------------------------
	//VECTOR axisDeg = Utility::VECTOR_ZERO;

	//// 画面の中心位置
	//Vector2 center = { Application::SCREEN_SIZE_X / 2,Application::SCREEN_SIZE_Y / 2 };

	//// キーボード操作
	//if (ins.IsNew(KEY_INPUT_UP)) { axisDeg.x += -1.0f; }
	//if (ins.IsNew(KEY_INPUT_DOWN)) { axisDeg.x += 1.0f; }
	//if (ins.IsNew(KEY_INPUT_LEFT)) { axisDeg.y += -1.0f; }
	//if (ins.IsNew(KEY_INPUT_RIGHT)) { axisDeg.y += 1.0f; }

	//if (ins.IsNew(KEY_INPUT_RIGHT))
	//{
	//	angle_.y += Utility::Deg2RadF(axisDeg.y);
	//}
	//if (ins.IsNew(KEY_INPUT_LEFT))
	//{
	//	angle_.y += Utility::Deg2RadF(axisDeg.y);
	//}
	//if (ins.IsNew(KEY_INPUT_UP) && Utility::Rad2DegF(angle_.x) <= 40.0f)
	//{
	//	angle_.x += Utility::Deg2RadF(axisDeg.x);
	//}
	//if (ins.IsNew(KEY_INPUT_DOWN) && Utility::Rad2DegF(angle_.x) >= -15.0f)
	//{
	//	angle_.x += Utility::Deg2RadF(axisDeg.x);
	//}

	//// カメラを回転させる
	//if (!Utility::EqualsVZero(axisDeg))
	//{

	//	// X軸のカメラの移動制御
	//	angle_.x += Utility::Deg2RadF(axisDeg.x);
	//	angle_.y += Utility::Deg2RadF(axisDeg.y);

	//	rotY_ = Quaternion::AngleAxis(angle_.y, Utility::AXIS_Y);

	//	rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, Utility::AXIS_X));

	//}

	//// 追従対象の位置
	//VECTOR followPos = playerTransform_->pos;

	//// 追従対象から注視点までの相対座標を回転
	//VECTOR relativeTPos = rotY_.PosAxis(LOCAL_P2T_POS);

	//// 追従対象からカメラまでの相対座標
	//VECTOR relativeCPos = rotXY_.PosAxis(LOCAL_P2C_POS);

	//// カメラ座標をゆっくり移動させる
	//pos_ = Utility::Lerp(pos_, VAdd(followPos, relativeCPos), 0.1f);

	//// 注視点をゆっくり移動させる
	//targetPos_ = Utility::Lerp(targetPos_, VAdd(followPos, relativeTPos), 0.1f);

	//// カメラの上方向
	//cameraUp_ = Utility::DIR_U;

	// マウスカーソルを非表示にする
	SetMouseDispFlag(false);

	// 回転
	//-------------------------------------
	VECTOR axisDeg = Utility::VECTOR_ZERO;

	// マウス回転量
	float rotPow = 4.0f;
	//float rotPow = 0.0f;

	// マウス位置
	Vector2 mousePos;

	// 画面の中心位置
	Vector2 center = { Application::SCREEN_SIZE_X / 2,Application::SCREEN_SIZE_Y / 2 };

	// マウス位置の取得
	GetMousePoint(&mousePos.x, &mousePos.y);

	// カメラ回転の計算(マウスカーソル位置と画面の中心の差分を計算し、回転量/FPSを乗算する)
	// これが回転量
	rotPowY_ = float(std::clamp(mousePos.x - center.x, -120, 120)) * rotPow / GetFPS();	// マウス横移動
	rotPowX_ = float(std::clamp(mousePos.y - center.y, -120, 120)) * rotPow / GetFPS();	// マウス縦移動

	// カメラ位置を中心にセット
	SetMousePoint(center.x, center.y);

	// 右方向
	if (center.x <= mousePos.x) { axisDeg.y += rotPowY_; }
	
	// 左方向
	if (center.x >= mousePos.x) { axisDeg.y += rotPowY_; }

	// 下方向
	if (center.y >= mousePos.y && Utility::Rad2DegF(angle_.x) >= -20.0f)
	{
		axisDeg.x += rotPowX_;
	}

	// 上方向
	if (center.y <= mousePos.y && Utility::Rad2DegF(angle_.x) <= 50.0f)
	{
		axisDeg.x += rotPowX_;
	}

	if (!Utility::EqualsVZero(axisDeg))
	{

		// カメラを回転させる
		// X軸のカメラの移動制御
		angle_.x += Utility::Deg2RadF(axisDeg.x);
		angle_.y += Utility::Deg2RadF(axisDeg.y);

		rotY_ = Quaternion::AngleAxis(angle_.y, Utility::AXIS_Y);

		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, Utility::AXIS_X));

	}

	// 追従対象の位置
	VECTOR followPos = playerTransform_->pos;

	// 追従対象から注視点までの相対座標を回転
	VECTOR relativeTPos = rotY_.PosAxis(LOCAL_P2T_POS);

	// 追従対象からカメラまでの相対座標
	VECTOR relativeCPos = rotXY_.PosAxis(LOCAL_P2C_POS);

	// カメラ座標をゆっくり移動させる
	pos_ = Utility::Lerp(pos_, VAdd(followPos, relativeCPos), 0.1f);

	// 注視点をゆっくり移動させる
	targetPos_ = Utility::Lerp(targetPos_, VAdd(followPos, relativeTPos), 0.1f);

	// カメラの上方向
	cameraUp_ = Utility::DIR_U;

}

void Camera::KeyboardLockOnController()
{

	auto& ins = InputManager::GetInstance();

	// マウスカーソルを非表示にする
	SetMouseDispFlag(false);

	// 回転
	//-------------------------------------
	VECTOR axisDeg = Utility::VECTOR_ZERO;

	// マウス回転量
	float rotPow = 3.0f;

	// マウス位置
	Vector2 mousePos;

	// 画面の中心位置
	Vector2 center = { Application::SCREEN_SIZE_X / 2,Application::SCREEN_SIZE_Y / 2 };

	// マウス位置の取得
	GetMousePoint(&mousePos.x, &mousePos.y);

	// カメラ回転の計算(マウスカーソル位置と画面の中心の差分を計算し、回転量/FPSを乗算する)
	// これが回転量
	rotPowY_ = float(std::clamp(mousePos.x - center.x, -120, 120)) * rotPow / GetFPS();	// マウス横移動
	rotPowX_ = float(std::clamp(mousePos.y - center.y, -120, 120)) * rotPow / GetFPS();	// マウス縦移動

	// カメラ位置を中心にセット
	SetMousePoint(center.x, center.y);

	if (center.x <= mousePos.x) { axisDeg.y += rotPowY_; }
	if (center.x >= mousePos.x) { axisDeg.y += rotPowY_; }

	if (center.y >= mousePos.y && Utility::Rad2DegF(lockOnAngles_.x) >= -20.0f)
	{
		axisDeg.x += rotPowX_;
	}
	if (center.y <= mousePos.y && Utility::Rad2DegF(lockOnAngles_.x) <= 10.0f)
	{
		axisDeg.x += rotPowX_;
	}

	if (!Utility::EqualsVZero(axisDeg))
	{

		// カメラを回転させる
		// X軸のカメラの移動制御
		lockOnAngles_.x += Utility::Deg2RadF(axisDeg.x);
		lockOnAngles_.y += Utility::Deg2RadF(axisDeg.y);

		rotY_ = Quaternion::AngleAxis(lockOnAngles_.y, Utility::AXIS_Y);

		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(lockOnAngles_.x, Utility::AXIS_X));

	}

}

void Camera::GamePadController()
{

	auto& ins = InputManager::GetInstance();

	// 回転
	//-------------------------------------
	VECTOR axisDeg = Utility::VECTOR_ZERO;

	// ゲームパッドの番号を取得
	auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	// パッドの方向をdirに直す
	// 右方向
	if (pad.AKeyRX > 0)
	{
		axisDeg.y = pad.AKeyRX;
		// 方向を正規化
		axisDeg = VNorm(axisDeg);
		axisDeg = VScale(axisDeg, 3.0f);
	}

	// 左方向
	if (pad.AKeyRX < 0)
	{
		axisDeg.y = pad.AKeyRX;
		// 方向を正規化
		axisDeg = VNorm(axisDeg);
		axisDeg = VScale(axisDeg, 3.0f);
	}

	// 上方向
	if (pad.AKeyRZ > 0 && Utility::Rad2DegF(angle_.x) <= 50.0f)
	{
		axisDeg.x = pad.AKeyRZ;
		// 方向を正規化
		axisDeg = VNorm(axisDeg);
		axisDeg = VScale(axisDeg, 3.0f);
	}

	// 下方向
	if (pad.AKeyRZ < 0 && Utility::Rad2DegF(angle_.x) >= -20.0f)
	{
		axisDeg.x = pad.AKeyRZ;
		// 方向を正規化
		axisDeg = VNorm(axisDeg);
		axisDeg = VScale(axisDeg, 3.0f);
	}

	if (axisDeg.x != 0.0f || axisDeg.y != 0.0f)
	{
		// カメラを回転させる
		// X軸のカメラの移動制御
		angle_.x += Utility::Deg2RadF(axisDeg.x);
		angle_.y += Utility::Deg2RadF(axisDeg.y);

		rotY_ = Quaternion::AngleAxis(angle_.y, Utility::AXIS_Y);

		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, Utility::AXIS_X));
	}

	// 追従対象の位置
	VECTOR followPos = playerTransform_->pos;

	// 追従対象から注視点までの相対座標を回転
	VECTOR relativeTPos = rotY_.PosAxis(LOCAL_P2T_POS);

	// 追従対象からカメラまでの相対座標
	VECTOR relativeCPos = rotXY_.PosAxis(LOCAL_P2C_POS);

	// カメラ座標をゆっくり移動させる
	pos_ = Utility::Lerp(pos_, VAdd(followPos, relativeCPos), 0.1f);

	// 注視点をゆっくり移動させる
	targetPos_ = Utility::Lerp(targetPos_, VAdd(followPos, relativeTPos), 0.1f);

	// カメラの上方向
	cameraUp_ = Utility::DIR_U;

}

void Camera::GamePadLockOnController()
{

	auto& ins = InputManager::GetInstance();

	// 回転
	//-------------------------------------
	VECTOR axisDeg = Utility::VECTOR_ZERO;

	// ゲームパッドの番号を取得
	auto pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	// パッドの方向をdirに直す
	// 右方向
	if (pad.AKeyRX > 0)
	{
		axisDeg.y = pad.AKeyRX;
		// 方向を正規化
		axisDeg = VNorm(axisDeg);
		axisDeg = VScale(axisDeg, 3.0f);
	}
	// 左方向
	if (pad.AKeyRX < 0)
	{
		axisDeg.y = pad.AKeyRX;
		// 方向を正規化
		axisDeg = VNorm(axisDeg);
		axisDeg = VScale(axisDeg, 3.0f);
	}
	// 上方向
	if (pad.AKeyRZ < 0 && Utility::Rad2DegF(lockOnAngles_.x) <= 10.0f)
	{
		axisDeg.x = -pad.AKeyRZ;
		// 方向を正規化
		axisDeg = VNorm(axisDeg);
		axisDeg = VScale(axisDeg, 3.0f);
	}
	// 下方向
	if (pad.AKeyRZ > 0 && Utility::Rad2DegF(lockOnAngles_.x) >= -20.0f)
	{
		axisDeg.x = -pad.AKeyRZ;
		// 方向を正規化
		axisDeg = VNorm(axisDeg);
		axisDeg = VScale(axisDeg, 3.0f);
	}


	if (axisDeg.x != 0.0f || axisDeg.y != 0.0f)
	{
		// カメラを回転させる
		// X軸のカメラの移動制御
		lockOnAngles_.x += Utility::Deg2RadF(axisDeg.x);
		lockOnAngles_.y += Utility::Deg2RadF(axisDeg.y);

		rotY_ = Quaternion::AngleAxis(lockOnAngles_.y, Utility::AXIS_Y);

		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(lockOnAngles_.x, Utility::AXIS_X));
	}

}

void Camera::UpdateDebugImGui()
{

	// ウィンドウタイトル&開始処理
	ImGui::Begin("Camera");

	// 大きさ
	//ImGui::Text("scale");
	//ImGui::InputFloat("Scl", &scl_);

	// 角度
	VECTOR rotDeg = VECTOR();
	rotDeg.x = Utility::Rad2DegF(angle_.x);
	rotDeg.y = Utility::Rad2DegF(angle_.y);
	rotDeg.z = Utility::Rad2DegF(angle_.z);
	ImGui::Text("angle(deg)");
	ImGui::SliderFloat("RotX", &rotDeg.x, -90.0f, 90.0f);
	ImGui::SliderFloat("RotY", &rotDeg.y, -90.0f, 90.0f);
	ImGui::SliderFloat("RotZ", &rotDeg.z, -90.0f, 90.0f);
	angle_.x = Utility::Deg2RadF(rotDeg.x);
	angle_.y = Utility::Deg2RadF(rotDeg.y);
	angle_.z = Utility::Deg2RadF(rotDeg.z);

	// 位置
	ImGui::Text("position");
	// 構造体の先頭ポインタを渡し、xyzと連続したメモリ配置へアクセス
	ImGui::InputFloat3("Pos", &pos_.x);
	// 終了処理
	ImGui::End();

}

void Camera::LazyRotation()
{

	// プレイヤーが向いている方向にカメラを回転させる
	// 二つのクォータニオンの角度差
	// 回転中にマウス操作が発生したら回転をやめる
	if (Quaternion::Angle(rotY_, lazyGoalRotY_) > abs(2.0f) && abs(rotPowY_) >= 5.0f && abs(rotPowX_) >= 5.0f)
	{
		rotY_ = Quaternion::Slerp(rotY_, lazyGoalRotY_, 0.1f);
		angle_.y = rotY_.ToEuler().y;
		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, Utility::AXIS_X));
	}
	// 回転しない場合
	else
	{
		rotY_ = lazyGoalRotY_;
		isLazy_ = false;
		angle_.y = rotY_.ToEuler().y;
		rotXY_ = rotY_.Mult(Quaternion::AngleAxis(angle_.x, Utility::AXIS_X));
	}

}