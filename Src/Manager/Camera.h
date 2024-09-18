#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"

class Transform;
class Player;

class Camera
{

public:

	// カメラの高さ
	static constexpr float HEIGHT = 250.0f;

	// 追従対象からカメラまでの距離
	static constexpr float DIS_FOLLOW2CAMERA = 150.0f;

	// 追従対象から注視点までの距離
	static constexpr float DIS_FOLLOW2TARGET = 350.0f;

	// カメラスピード：NEAR
	static constexpr float SPEED = 20.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 40.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_FAR = 15000.0f;

	// カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 500.0f, -500.0f };

	// プレイヤーからカメラの位置までの相対座標
	static constexpr VECTOR LOCAL_P2C_POS = { 0.0f, 500.0f,-400.0f };

	// 敵からカメラの位置までの相対座標
	static constexpr VECTOR LOCAL_E2C_POS = { 0.0f, 500.0f,-400.0f };

	// プレイヤーから注視点までの相対座標
	static constexpr VECTOR LOCAL_P2T_POS = { 0.0f,0.0f,500.0f };

	static constexpr VECTOR LOCAL_POS = { 0.0f,300.0f,-500.0f };

	// 敵から注視点までの相対座標
	static constexpr VECTOR LOCAL_E2T_POS = { 0.0f,0.0f,500.0f };

	static constexpr VECTOR LOCAL_LOCK_ON_F2C_POS = { 0.0f,300.0f,-500.0f };


	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,	// 定点カメラ
		FREE,			// フリーモード
		FOLLOW,			// 追従モード
		LOCKON,			// ロックオンモード
	};

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);
	void SetBeforeDrawLockOn(void);
	void Draw(void);
	void Release(void);

	VECTOR GetPos(void) const;

	VECTOR GetAngles(void) const;

	// プレイヤーが向いている角度
	void SetLazyAngles(const VECTOR angles);

	VECTOR GetTargetPos(void) const;

	void SetBeforeDrawFollow(void);
	
	// 追従対象の設定
	void SetPlayer(const Transform* follow);

	// 追従対象の設定
	void SetEnemy(const Transform* follow);

	// ロックオンのフラグを変える
	void ChangeLockOnFlag(void);

	// カメラモードの変更
	void ChangeMode(MODE mode);

	Quaternion GetRotY(void) const;

	// 遅延回転
	void LazyRotation(void);

	// ロックオンの取得
	bool GetLockOn(void);

	Camera::MODE GetMode(void);

	//	ロックオン中のY軸の角度を足す
	void AddLockOnAnglesY(float rad);

	// ステージのモデルIDを設定
	void SetStageID(const int modelId);

private:

	// ステージのID
	int stageId_;

	// ロックオンのフラグ
	bool lockOn_;

	// カメラモード
	MODE mode_;

	// カメラの位置
	VECTOR pos_;

	// 移動後座標
	VECTOR movedPos_;

	// カメラの注視点
	VECTOR targetPos_;

	// カメラの上方向
	VECTOR cameraUp_;

	// カメラの回転
	Quaternion rotXY_;
	Quaternion rotY_;

	// Y軸のゴールの角度
	Quaternion lazyGoalRotY_;

	// SetLazyAngleに値が入ったらtrue
	bool isLazy_ = false;

	// カメラの回転
	VECTOR angle_;

	Quaternion lockOnLook_;

	// ロックオン専用コントローラ制御角
	VECTOR lockOnAngles_;

	VECTOR angles_;

	bool isNearLockOnTarget_;

	VECTOR goalCameraPos_;

	// マウスの横移動
	float rotPowY_;

	// マウスの縦移動
	float rotPowX_;

	// 追従対象からカメラまでの相対座標	
	VECTOR relativeCPos_;

	// ポリゴンと当たっているか
	bool pHit_;

	// 操作説明をスキップさせないようにするカウンタ
	float operationCnt_;

	// 決定音
	int musicDecisionId_;

	// カメラを初期位置に戻す
	void SetDefault(void);

	// 注視点をキャラクター前方位置に設定
	void SetTargetPosFollowForward(void);

	// 追従対象
	const Transform* playerTransform_;
	const Transform* enemyTransform_;

	// キーボードの操作
	void KeybordContoroller(void);
	void KeybordLockOnContoroller(void);

	// ゲームパッドの操作
	void GamePadController(void);
	void GamePadLockOnContoroller(void);

	// ステージとの当たり判定
	void CollisionStage(void);

};

