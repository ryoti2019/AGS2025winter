#pragma once
#include <functional>
#include <DxLib.h>
#include "../Common/Quaternion.h"
#include "../Object/Common/Transform.h"

class Player;

class Camera
{

public:

	// カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 0.0f, 0.0f };

	// プレイヤーからカメラの位置までの相対座標
	static constexpr VECTOR LOCAL_P2C_POS = { 0.0f, 1500.0f,-3000.0f };

	// プレイヤーから注視点までの相対座標
	static constexpr VECTOR LOCAL_P2T_POS = { 0.0f,800.0f,0.0f };

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

	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,	// 定点カメラ
		FREE,			// フリーモード
		TITLE,			// タイトルシーンモード
		FOLLOW,			// 追従モード
		LOCKON,			// ロックオンモード
		SPECIAL,		// 必殺技モード
		APPEARANCE,		// ボスの登場モード
		GAME_CLEAR		// ゲームクリアシーン
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

	// ロックオンの設定
	void SetLockOn(const bool lockOn);

	// カメラモードの変更
	void ChangeMode(const MODE& mode);

	// 遅延回転
	void LazyRotation();

	//	ロックオン中のY軸の角度を足す
	void AddLockOnAnglesY(float rad);

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

	// ボスの登場シーンの2つ目のカメラの動きのフラグを取得
	const bool GetIsBossAppearanceCameraMove3() { return isBossAppearanceCameraMove3_; }

	// ボスシーンが終わったかのフラグ
	const bool GetIsEndBossAppearanceScene() { return isEndBossAppearanceScene_; }

private:

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

	// ロックオン専用コントローラ制御角
	VECTOR lockOnAngles_;

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

	Quaternion lockOnLook_;

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
	bool isLazy_ = false;

	// ロックオンのフラグ
	bool lockOn_;

	bool isNearLockOnTarget_;

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
	void ChangeLockOn();
	void ChangeSpecial();
	void ChangeAppearance();
	void ChangeGameClear();

	// カメラモードの更新
	std::function<void(const float deltaTime)> modeDraw_;
	void SetBeforeDrawFixedPoint(const float deltaTime);
	void SetBeforeDrawFree(const float deltaTime);
	void SetBeforeDrawTitle(const float deltaTime);
	void SetBeforeDrawFollow(const float deltaTime);
	void SetBeforeDrawLockOn(const float deltaTime);
	void SetBeforeDrawSpecial(const float deltaTime);
	void SetBeforeDrawAppearance(const float deltaTime);
	void SetBeforeDrawGameClear(const float deltaTime);

	// 関数ポインタの初期化
	void InitFunctionPointer();

	// カメラを初期位置に戻す
	void SetDefault();

	// 注視点をキャラクター前方位置に設定
	void SetTargetPosFollowForward();

	// キーボードの操作
	void KeyboardController();
	void KeyboardLockOnController();

	// ゲームパッドの操作
	void GamePadController();
	void GamePadLockOnController();

	// ImGuiのデバッグ描画の更新
	void UpdateDebugImGui();

};