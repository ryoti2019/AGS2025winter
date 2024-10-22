#pragma once
#include <DxLib.h>
#include <memory>
#include <vector>
#include "../Common/Vector2F.h"
#include "../Object/Common/Transform.h"
#include "../Manager/ResourceManager.h"
#include "../Object/Common/AnimationController.h"

class Camera;

enum class ActorType {
	NONE = -1,
	PLAYER,
	ENEMY,
	MAX
};

class ActorBase
{

public:

	struct CollisionData
	{

		// 右手のフレーム
		int rightHand;

		// 左手のフレーム
		int leftHand;

		// 右足のフレーム
		int rightFoot;

		// 左足のフレーム
		int leftFoot;

		// 右手の座標
		VECTOR rightHandPos;

		// 左手の座標
		VECTOR leftHandPos;

		// 右足の座標
		VECTOR rightFootPos;

		// 左足の座標
		VECTOR leftFootPos;

		// 右手の向き
		Quaternion rightHandRot;

		// 左手の向き
		Quaternion leftHandRot;

		// 右足の向き
		Quaternion rightFootRot;

		// 左足の向き
		Quaternion leftFootRot;

		// 当たり判定の半径
		float collisionRadius;

	};


	// 回転完了までの時間
	const float ROTATION_POW;

	// 当たり判定の半径
	const float COLLISION_RADIUS;

	ActorBase(const VECTOR& pos);

	virtual ~ActorBase() = default;

	virtual void Init(const VECTOR& pos);
	virtual void Create(const VECTOR& pos);
	virtual void Update(const float deltaTime);
	virtual void Draw();

	void SetPos(const VECTOR& pos) { transform_.pos = pos; };

	const bool GetIsActive() const { return isActive_; }

	void SetIsActive(const bool isActive) { isActive_ = isActive; }

	const ActorType GetActorType() const { return actorType_; }

protected:

	// リソース管理
	ResourceManager& resMng_;

#pragma region オブジェクトの情報

	// アニメーション
	std::unique_ptr<AnimationController> animationController_;

	// モデル制御の基本情報
	Transform transform_;

	// 衝突判定のデータ
	CollisionData collisionData_;

	// 方向
	VECTOR dir_;

	// 目的の角度
	Quaternion goalQuaRot_;

	// アニメーションコントローラーに状態を文字型で渡す変数
	std::string key_;

	// 1個前のアニメーション
	std::string preKey_;

	// スピード
	float speed_;

	// HP
	int hp_;

	// モデルID
	int modelId_;

	// モデルの大きさ
	float scl_;

	// アクターの種類
	ActorType actorType_;

	// 生きているかどうか
	bool isAlive_;

	// 回転する時間
	float stepRotTime_;

	// 機能の初期化
	virtual void InitFunction() = 0;

	// パラメータの初期化
	virtual void InitParameter() = 0;

	// アニメーションの初期化
	virtual void InitAnimation() = 0;

	// 関数ポインタの初期化
	virtual void InitFunctionPointer() = 0;

	// 衝突判定の初期化
	virtual void InitCollision() = 0;

	// 衝突判定の更新
	void CollisionUpdate();

	// 移動処理
	virtual void Move() = 0;

	// 攻撃処理
	virtual void ComboAttack(const float deltaTime) = 0;

	// 攻撃状態
	virtual bool AttackState() = 0;

	// 遅延回転
	void LazyRotation(float goalRot);

	// 衝突判定で使うものを登録
	void CollisionRegister();

#pragma endregion

private:

	// アクティブ状態かどうか
	bool isActive_;

	// デバッグ描画
	void DrawDebug();

};