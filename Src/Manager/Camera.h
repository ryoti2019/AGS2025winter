#pragma once
#include <functional>
#include <DxLib.h>
#include "../Common/Quaternion.h"
#include "../Object/Common/Transform.h"

class Player;

class Camera
{

public:

	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,	// 定点カメラ
		FREE,			// フリーモード
		TITLE,			// タイトルシーンモード
		FOLLOW,			// 追従モード
		SPECIAL,		// 必殺技モード
		APPEARANCE,		// ボスの登場モード
		GAME_CLEAR,		// ゲームクリアモード
		GAME_OVER		// ゲームオーバーモード
	};

	Camera();
	~Camera();

	void Init();
	void Update();
	void SetBeforeDraw(const float deltaTime);
	void Draw();
	void Release();

	// プレイヤーが向いている角度
	void SetLazyAngles(const VECTOR angles);

	// 追従対象の設定
	void SetPlayer(const std::shared_ptr<Transform>& follow) { playerTransform_ = follow; }

	// 追従対象の設定
	void SetBoss(const std::shared_ptr<Transform>& follow) { bossTransform_ = follow; }

	// カメラモードの変更
	void ChangeMode(const MODE& mode);

	// ステージのモデルIDを設定
	void SetStageID(const int modelId);

	// モードを取得
	const Camera::MODE& GetMode()const { return mode_; }

	// カメラ座標を取得
	const VECTOR& GetPos()const { return pos_; }

	// カメラ座標を設定
	void SetPos(const VECTOR& pos) { pos_ = pos; }

	// 注視点を取得
	const VECTOR& GetTargetPos()const { return targetPos_; }

	// 角度を取得
	const VECTOR& GetAngle()const { return angle_; }

	// ボスの登場シーンの1つ目のカメラの動きのフラグを取得
	const bool GetIsBossAppearanceCameraMove1() { return isBossAppearanceCameraMove1_; }

	// ボスの登場シーンの2つ目のカメラの動きのフラグを取得
	const bool GetIsBossAppearanceCameraMove2() { return isBossAppearanceCameraMove2_; }

	// ボスの登場シーンの3つ目のカメラの動きのフラグを取得
	const bool GetIsBossAppearanceCameraMove3() { return isBossAppearanceCameraMove3_; }

	// ボスシーンが終わったかのフラグ
	const bool GetIsEndBossAppearanceScene() { return isEndBossAppearanceScene_; }

	// カメラが何秒移動したか計るカウンタ
	const float GetElapsedTime()const { return elapsedTime_; }

private:

	// カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 0.0f, 0.0f };

	// プレイヤーからカメラの位置までの相対座標
	static constexpr VECTOR LOCAL_P2C_POS = { 0.0f, 1500.0f,-3000.0f };

	// プレイヤー追従時のカメラのローカル注視点
	static constexpr VECTOR FOLLOW_CAMERA_LOCAL_TARGET_POS = { 0.0f,800.0f,0.0f };

	// 敵からカメラの位置までの相対座標
	static constexpr VECTOR LOCAL_E2C_POS = { 0.0f, 500.0f,-400.0f };

	// 敵から注視点までの相対座標
	static constexpr VECTOR LOCAL_E2T_POS = { 0.0f,0.0f,500.0f };

	// カメラの高さ
	static constexpr float HEIGHT = 250.0f;

	// 追従対象からカメラまでの距離
	static constexpr float DIS_FOLLOW2CAMERA = 150.0f;

	// 追従対象から注視点までの距離
	static constexpr float DIS_FOLLOW2TARGET = 350.0f;

	// カメラスピード
	static constexpr float SPEED = 100.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 40.0f;

	// カメラクリップ：FAR
	static constexpr float CAMERA_FAR = 1000000.0f;

	// 必殺技時のカメラが動く時間
	static constexpr float SPECIAL_MOVE_MAX_TIME = 2.0f;

	// 当たり判定の半径
	static constexpr float COLLISION_RADIUS = 100.0f;

	// 最初の角度
	static constexpr float INIT_ANGLE = 90.0f;

	// 定点カメラの座標
	static constexpr VECTOR FIXED_POINT_CAMERA_POS = { 0.0f,200.0f,-500.0f };

	// 定点カメラの注視点
	static constexpr VECTOR FIXED_POINT_CAMERA_TARGET_POS = { 0.0f,150.0f,0.0f };

	// タイトルのカメラのローカル座標
	static constexpr VECTOR TITLE_CAMERA_LOCAL_POS = { -1200.0f,800.0f,2500.0f };

	// タイトルのカメラのローカル注視点
	static constexpr VECTOR TITLE_CAMERA_TARGET_LOCAL_POS = { -1200.0f, 1000.0f, 0.0f };

	// 必殺技のカメラの初期ローカル座標
	static constexpr VECTOR SPECIAL_CAMERA_INIT_LOCAL_POS = { 500.0f,2000.0f,500.0f };

	// 必殺技のカメラのローカル注視点
	static constexpr VECTOR SPECIAL_CAMERA_LOCAL_TARGET_POS = { 0.0f,1000.0f,0.0f };

	// ボスの登場シーンのカメラの初期ローカル座標
	static constexpr VECTOR BOSS_APPEARANCE_CAMERA_INIT_LOCAL_POS = { -1000.0f, 2000.0f, -1000.0f };

	// ボスの登場シーンのカメラの初期注視点
	static constexpr VECTOR BOSS_APPEARANCE_CAMERA_INIT_LOCAL_TARGET_POS = { 0.0f,-1000.0f,1000.0f };

	// ゲームクリアシーンのカメラのローカル座標
	static constexpr VECTOR GAME_CLEAR_CAMERA_LOCAL_POS = { -1200.0f,800.0f,2500.0f };

	// ゲームクリアシーンのカメラの注視点
	static constexpr VECTOR GAME_CLEAR_CAMERA_LOCAL_TRAGET_POS = { -1200.0f, 1000.0f, 0.0f };

	// フリーモードの時のカメラの回転量
	static constexpr float FREE_MODE_ROTATION_POW = 0.01f;

	// 必殺技のカメラの移動量
	static constexpr VECTOR SPECIAL_CAMERA_MOVE_POW = { 20.0f,0.02f,20.0f };

	// ボスの登場シーンカメラの動き2つ目のカメラのローカル座標
	static constexpr VECTOR BOSS_APPEARANCE_CAMERA2_LOCAL_POS = { 300.0f,100.0f,300.0f };

	// ボスの登場シーンカメラの動き2つ目の注視点のローカル座標
	static constexpr VECTOR BOSS_APPEARANCE_CAMERA2_LOCAL_TARGET_POS = { 10.0f,0.0f,0.0f };

	// ボスの登場シーンカメラの動き3つ目のカメラのローカル座標
	static constexpr VECTOR BOSS_APPEARANCE_CAMERA3_LOCAL_POS = { 2000.0f,2000.0f,-2000.0f };

	// ボスの登場シーンカメラの動き3つ目の注視点の座標
	static constexpr VECTOR BOSS_APPEARANCE_CAMERA3_TARGET_POS = { -10800.0f,-18000.0f,-140000.0f };

	// ボスの登場シーンカメラの動き3つ目のカメラのローカル座標2
	static constexpr VECTOR BOSS_APPEARANCE_CAMERA3_LOCAL_POS2 = { 0.0f ,1000.0f ,2000.0f };

	// ボスの登場シーンカメラの動き3つ目の注視点のローカル座標2
	static constexpr VECTOR BOSS_APPEARANCE_CAMERA3_TARGET_LOCAL_POS2 = { 0.0f ,1000.0f ,0.0f };

	// 必殺技のカメラ1つ目の移動量
	static constexpr float CAMERA_MOVE_POW = 10.0f;

	// 必殺技シーンの1つ目の動ける時間
	static constexpr float FIRST_CAMERA_TIME = 3.0f;

	// 必殺技シーンの2つ目の動ける時間
	static constexpr float SECOND_CAMERA_TIME = 2.0f;

	// 必殺技シーンの3つ目の動ける時間
	static constexpr float THIRD_CAMERA_TIME = 5.0f;

	// ボスバトルに遷移する時間
	static constexpr float BOSS_BATTLE_TRANSITION_TIME = 10.0f;

	// カメラが敵に近づいていく力
	static constexpr float CAMERA_APPROACH_FORCE = 0.05f;

	// ゲームオーバー時のカメラが回転量
	static constexpr float GAME_OVER_CAMERA_ROTATION_FORCE = 0.5f;

	// ボスの登場シーンカメラのローカル座標
	static constexpr VECTOR GAME_OVER_CAMERA_LOCAL_POS = { 0.0f,3000.0f,-2000.0f };

	// ゲームオーバー時のカメラの移動量
	static constexpr float GAME_OVER_CAMERA_MOVE_AMOUNT = 0.1f;

	// ゲームオーバー時の注視点の移動量
	static constexpr float GAME_OVER_TARGET_MOVE_AMOUNT = 0.1f;

	// マウスの回転量
	static constexpr float MOUSE_ROTATION_AMOUNT = 4.0f;

	// マウスカーソルの位置と画面の中心との差分の範囲制限値
	static constexpr int MOUSE_ROTATION_CLAMP = 120;

	// カメラの下方向の制限
	static constexpr float MOUSE_LOOK_MIN_ANGLE = -20.0f;

	// カメラの上方向の制限
	static constexpr float MOUSE_LOOK_MAX_ANGLE = 50.0f;

	// 追従時のカメラの移動量
	static constexpr float FOLLOW_CAMERA_MOVE_AMOUNT = 0.1f;

	// 追従時の注視点の移動量
	static constexpr float FOLLOW_TARGET_MOVE_AMOUNT = 0.1f;

	// ゲームパッドの回転量
	static constexpr float GAME_PAD_CAMERA_ROTATION_AMOUNT = 3.0f;

	// 追従対象
	std::shared_ptr<Transform> playerTransform_;
	std::shared_ptr<Transform> bossTransform_;

	// カメラモード
	MODE mode_;

	// カメラの位置
	VECTOR pos_;

	// 移動後座標
	VECTOR movedPos_;

	// カメラの移動量
	VECTOR movePow_;

	// カメラの注視点
	VECTOR targetPos_;

	// カメラの上方向
	VECTOR cameraUp_;

	VECTOR goalCameraPos_;

	// カメラの回転
	VECTOR angle_;

	// 追従対象からカメラまでの相対座標	
	VECTOR relativeCPos_;

	// カメラの回転
	Quaternion rotXY_;
	Quaternion rotY_;

	// Y軸のゴールの角度
	Quaternion lazyGoalRotY_;

	// ステージのID
	int stageId_;

	// 決定音
	int musicDecisionId_;

	// マウスの横移動
	float rotPowY_;

	// マウスの縦移動
	float rotPowX_;

	// 操作説明をスキップさせないようにするカウンタ
	float operationCnt_;

	// SetLazyAngleに値が入ったらtrue
	bool isLazy_;

	// ポリゴンと当たっているか
	bool pHit_;

	// 必殺技時のカメラの移動する時間
	float specialMoveCnt_;

	// カメラが何秒移動したか計るカウンタ
	float elapsedTime_;

	// ボスの登場シーンの1つ目のカメラの動きのフラグ
	bool isBossAppearanceCameraMove1_;

	// ボスの登場シーンの2つ目のカメラの動きのフラグ
	bool isBossAppearanceCameraMove2_;

	// ボスの登場シーンの3つ目のカメラの動きのフラグ
	bool isBossAppearanceCameraMove3_;

	// ボスの登場シーンが終わったかのフラグ
	bool isEndBossAppearanceScene_;

	// カメラモードの遷移
	std::unordered_map<MODE, std::function<void()>> modeChange_;
	void ChangeFixedPoint();
	void ChangeFree();
	void ChangeTitle();
	void ChangeFollow();
	void ChangeSpecial();
	void ChangeAppearance();
	void ChangeGameClear();
	void ChangeGameOver();

	// カメラモードの更新
	std::function<void(const float deltaTime)> modeDraw_;
	void SetBeforeDrawFixedPoint(const float deltaTime);
	void SetBeforeDrawFree(const float deltaTime);
	void SetBeforeDrawTitle(const float deltaTime);
	void SetBeforeDrawFollow(const float deltaTime);
	void SetBeforeDrawSpecial(const float deltaTime);
	void SetBeforeDrawAppearance(const float deltaTime);
	void SetBeforeDrawGameClear(const float deltaTime);
	void SetBeforeDrawGameOver(const float deltaTime);

	// 関数ポインタの初期化
	void InitFunctionPointer();

	// カメラを初期位置に戻す
	void SetDefault();

	// 注視点をキャラクター前方位置に設定
	void SetTargetPosFollowForward();

	// キーボードの操作
	void KeyboardController();

	// ゲームパッドの操作
	void GamePadController();

	// ImGuiのデバッグ描画の更新
	void UpdateDebugImGui();

};