#include <EffekseerForDXLib.h>
#include "../Utility/Utility.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "SceneManager.h" 
#include "../Application.h"
#include "../Object/Common/Transform.h"
#include "Camera.h"

Camera::Camera(void)
{

	mode_ = MODE::FIXED_POINT;
	pos_ = { 0.0f, 0.0f, 0.0f };
	targetPos_ = { 0.0f, 0.0f, 0.0f };
	angle_ = { 0.0f, 0.0f, 0.0f };
	lockOn_ = false;

	// カメラの初期設定
	SetDefault();

}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;
	case Camera::MODE::LOCKON:
		SetBeforeDrawLockOn();
		break;
	}

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

}

void Camera::SetBeforeDrawFixedPoint(void)
{
	pos_ = { 0.0f,5000.0f,-5000.0f };
}

void Camera::SetBeforeDrawFree(void)
{

	auto& ins = InputManager::GetInstance();

#pragma region 回転

	VECTOR axisDeg = Utility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_UP)) { axisDeg.x += -1.0f; }
	if (ins.IsNew(KEY_INPUT_DOWN)) { axisDeg.x += 1.0f; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { axisDeg.y += -1.0f; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { axisDeg.y += 1.0f; }


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
	targetPos_ = VAdd(pos_, localPos);

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

void Camera::SetBeforeDrawFollow(void)
{

	auto& ins = InputManager::GetInstance();

	auto& sce = SceneManager::GetInstance();

	// マウスでの操作
	if (!SceneManager::GetInstance().GetGamePad())
	{
		KeybordController();
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

void Camera::SetBeforeDrawLockOn(void)
{

	// 同期先の位置
	VECTOR playerPos = playerTransform_->pos;

	// 敵の位置
	VECTOR enemyPos = { 0.0f,1000.0f,5000.0f };

	if (lockOn_)
	{
		enemyPos = enemyTransform_->pos;
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


void Camera::Draw(void)
{
}

void Camera::Release(void)
{
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

VECTOR Camera::GetAngle(void) const
{
	return angle_;
}

void Camera::SetLazyAngles(const VECTOR angles)
{

	lazyGoalRotY_ = Quaternion::Euler(0.0f, angles.y, 0.0f);
	isLazy_ = true;

}

VECTOR Camera::GetTargetPos(void) const
{
	return targetPos_;
}

void Camera::SetPlayer(const Transform* follow)
{
	playerTransform_ = follow;
}

void Camera::SetEnemy(const Transform* follow)
{
	enemyTransform_ = follow;
}

void Camera::SetLockOn(const bool lockOn)
{
	lockOn_ = lockOn;
}

void Camera::ChangeMode(MODE mode)
{

	// カメラモードの変更
	mode_ = mode;

	// 変更時の初期化処理
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		// カメラの初期設定
		SetDefault();
		pos_ = { 0.0f,200.0f,-500.0f };
		targetPos_ = { 0.0f,150.0f,0.0f };
		break;
	case Camera::MODE::FREE:
		break;
	case Camera::MODE::FOLLOW:
		angle_ = { 0.0f,0.0f,0.0f };
		lockOnAngles_ = { 0.0f, 0.0f, 0.0f };
		break;
	case Camera::MODE::LOCKON:
		angle_ = { 0.0f,0.0f,0.0f };
		lockOnAngles_ = { 0.0f, 0.0f, 0.0f };
		break;
	}

}

Quaternion Camera::GetRotY(void) const
{
	return rotY_;
}

bool Camera::GetLockOn(void)
{
	return lockOn_;
}

Camera::MODE Camera::GetMode(void)
{
	return mode_;
}

void Camera::AddLockOnAnglesY(float rad)
{
	lockOnAngles_.y += rad;
}

void Camera::SetStageID(const int modelId)
{
	stageId_ = modelId;
}

void Camera::SetDefault(void)
{

	// カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	// 注視点
	targetPos_ = VAdd(pos_, LOCAL_P2T_POS);

	// カメラの上方向
	cameraUp_ = { 0.0f, 1.0f, 0.0f };

	// カメラはX軸に傾いているが、
	// この傾いた状態を角度ゼロ、傾き無しとする
	rotY_ = Quaternion::Identity();
	rotXY_ = Quaternion::Identity();

	// ロックオン
	lockOn_ = false;

}

void Camera::SetTargetPosFollowForward(void)
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

void Camera::KeybordController(void)
{

	auto& ins = InputManager::GetInstance();

	// 回転
//-------------------------------------
	VECTOR axisDeg = Utility::VECTOR_ZERO;

	// 画面の中心位置
	Vector2 center = { Application::SCREEN_SIZE_X / 2,Application::SCREEN_SIZE_Y / 2 };

	// キーボード操作
	if (ins.IsNew(KEY_INPUT_UP)) { axisDeg.x += 1.0f; }
	if (ins.IsNew(KEY_INPUT_DOWN)) { axisDeg.x += -1.0f; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { axisDeg.y += -1.0f; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { axisDeg.y += 1.0f; }

	if (ins.IsNew(KEY_INPUT_RIGHT))
	{
		angle_.y += Utility::Deg2RadF(axisDeg.y);
	}
	if (ins.IsNew(KEY_INPUT_LEFT))
	{
		angle_.y += Utility::Deg2RadF(axisDeg.y);
	}
	if (ins.IsNew(KEY_INPUT_UP) && Utility::Rad2DegF(angle_.x) <= 40.0f)
	{
		angle_.x += Utility::Deg2RadF(axisDeg.x);
	}
	if (ins.IsNew(KEY_INPUT_DOWN) && Utility::Rad2DegF(angle_.x) >= -15.0f)
	{
		angle_.x += Utility::Deg2RadF(axisDeg.x);
	}

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

	//// マウスカーソルを非表示にする
	//SetMouseDispFlag(false);

	//// 回転
	////-------------------------------------
	//VECTOR axisDeg = Utility::VECTOR_ZERO;

	//// マウス回転量
	//float rotPow = 4.0f;
	////float rotPow = 0.0f;

	//// マウス位置
	//Vector2 mousePos;

	//// 画面の中心位置
	//Vector2 center = { Application::SCREEN_SIZE_X / 2,Application::SCREEN_SIZE_Y / 2 };

	//// マウス位置の取得
	//GetMousePoint(&mousePos.x, &mousePos.y);

	//// カメラ回転の計算(マウスカーソル位置と画面の中心の差分を計算し、回転量/FPSを乗算する)
	//// これが回転量
	//rotPowY_ = float(std::clamp(mousePos.x - center.x, -120, 120)) * rotPow / GetFPS();	// マウス横移動
	//rotPowX_ = float(std::clamp(mousePos.y - center.y, -120, 120)) * rotPow / GetFPS();	// マウス縦移動

	//// カメラ位置を中心にセット
	//SetMousePoint(center.x, center.y);

	//if (center.x <= mousePos.x) { axisDeg.y += rotPowY_; }
	//if (center.x >= mousePos.x) { axisDeg.y += rotPowY_; }

	//if (center.y >= mousePos.y && Utility::Rad2DegF(angle_.x) >= -30.0f)
	//{
	//	axisDeg.x += rotPowX_;
	//}
	//if (center.y <= mousePos.y && Utility::Rad2DegF(angle_.x) <= 10.0f)
	//{
	//	axisDeg.x += rotPowX_;
	//}

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

void Camera::KeybordLockOnContoroller(void)
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

void Camera::GamePadController(void)
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
	if (pad.AKeyRZ < 0 && Utility::Rad2DegF(angle_.x) >= -30.0f)
	{
		axisDeg.x = pad.AKeyRZ;
		// 方向を正規化
		axisDeg = VNorm(axisDeg);
		axisDeg = VScale(axisDeg, 3.0f);
	}
	// 下方向
	if (pad.AKeyRZ > 0 && Utility::Rad2DegF(angle_.x) <= 30.0f)
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

void Camera::GamePadLockOnContoroller(void)
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

void Camera::CheckStageCollision(void)
{

	// 球体との衝突判定
	auto hits = MV1CollCheck_Sphere(
		stageId_, -1, pos_, 20.0f);

	if (hits.HitNum > 0)
	{
		pHit_ = true;
	}
	else
	{
		pHit_ = false;
	}

	// 検出した地面ポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(hits);

}

void Camera::LazyRotation(void)
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