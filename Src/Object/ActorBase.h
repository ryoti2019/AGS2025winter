#pragma once
#include <DxLib.h>
#include <memory>
#include <vector>
#include "../Lib/nlohmann/json.hpp"
#include "../Common/Vector2F.h"
#include "../Object/Common/Transform.h"
#include "../Manager/ResourceManager.h"
#include "../Object/Common/AnimationController.h"

class Camera;

using json = nlohmann::json;

enum class ActorType {
	NONE = -1,
	PLAYER,
	ENEMY,
	MAX
};

class ActorBase : public std::enable_shared_from_this<ActorBase>
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

		// 体全体のフレーム
		int body;

		// 右手の座標
		VECTOR rightHandPos;

		// 左手の座標
		VECTOR leftHandPos;

		// 右足の座標
		VECTOR rightFootPos;

		// 左足の座標
		VECTOR leftFootPos;

		// 体全体の座標
		VECTOR bodyPos;

		// 右手のカプセルの上座標
		VECTOR rightHandCapsuleUpPos;

		// 右手のカプセルの下座標
		VECTOR rightHandCapsuleDownPos;

		// 左手のカプセルの上座標
		VECTOR leftHandCapsuleUpPos;

		// 左手のカプセルの下座標
		VECTOR leftHandCapsuleDownPos;

		// 右足のカプセルの上座標
		VECTOR rightFootCapsuleUpPos;

		// 右足のカプセルの下座標
		VECTOR rightFootCapsuleDownPos;

		// 左足のカプセルの上座標
		VECTOR leftFootCapsuleUpPos;

		// 左足のカプセルの下座標
		VECTOR leftFootCapsuleDownPos;

		// 体のカプセルの上座標
		VECTOR bodyCapsuleUpPos;

		// 体のカプセルの下座標
		VECTOR bodyCapsuleDownPos;

		// 右手の向き
		Quaternion rightHandRot;

		// 左手の向き
		Quaternion leftHandRot;

		// 右足の向き
		Quaternion rightFootRot;

		// 左足の向き
		Quaternion leftFootRot;

		// 体の向き
		Quaternion bodyRot;

		// 手足の当たり判定の半径
		float handAndFootCollisionRadius;

		// 体の当たり判定の半径
		float bodyCollisionRadius;

	};

	// 右手のカプセルの上の相対座標
	const VECTOR RIGHT_HAND_RELATIVE_UP_POS;

	// 右手のカプセルの下の相対座標
	const VECTOR RIGHT_HAND_RELATIVE_DOWN_POS;

	// 左手のカプセルの上の相対座標
	const VECTOR LEFT_HAND_RELATIVE_UP_POS;

	// 左手のカプセルの下の相対座標
	const VECTOR LEFT_HAND_RELATIVE_DOWN_POS;

	// 右足のカプセルの上の相対座標
	const VECTOR RIGHT_FOOT_RELATIVE_UP_POS;

	// 右足のカプセルの下の相対座標
	const VECTOR RIGHT_FOOT_RELATIVE_DOWN_POS;

	// 左足のカプセルの上の相対座標
	const VECTOR LEFT_FOOT_RELATIVE_UP_POS;

	// 左足のカプセルの下の相対座標
	const VECTOR LEFT_FOOT_RELATIVE_DOWN_POS;

	// 体のカプセルの上の相対座標
	const VECTOR BODY_RELATIVE_UP_POS;

	// 体のカプセルの下の相対座標
	const VECTOR BODY_RELATIVE_DOWN_POS;

	// 右手のフレーム名
	std::string RIGHT_HAND_FRAME;

	// 左手のフレーム名
	std::string LEFT_HAND_FRAME;

	// 右足のフレーム名
	std::string RIGHT_FOOT_FRAME;

	// 左足のフレーム名
	std::string LEFT_FOOT_FRAME;

	// 体のフレーム名
	std::string BODY_FRAME;

	// 攻撃するときに進む移動量
	const float ATTACK_MOVE_POW;

	// 回転量
	const float ROTATION_POW;

	// 手足の当たり判定の半径
	const float HAND_AND_FOOT_COLLISION_RADIUS;

	// 体の当たり判定の半径
	const float BODY_COLLISION_RADIUS;

	ActorBase(const VECTOR& pos, const json& data);

	virtual ~ActorBase() = default;

	virtual void Init(const VECTOR& pos);
	virtual void Create(const VECTOR& pos);
	virtual void Update(const float deltaTime);
	virtual void Draw();

	// 座標を設定
	void SetPos(const VECTOR& pos) { transform_.pos = pos; };

	// 生存判定を取得
	const bool GetIsActive() const { return isActive_; }

	// 生存判定を設定
	void SetIsActive(const bool isActive) { isActive_ = isActive; }

	// HPを取得
	const int GetHp()const { return hp_; };

	// HPを設定
	void SetHp(const int hp) { hp_ = hp; };

	// HPを減らす
	void SubHp(const int hp) { hp_ -= hp;};

	// アクタータイプを取得
	const ActorType& GetActorType() const { return actorType_; }

	// コリジョンデータのを取得
	const CollisionData& GetCollisionData() const { return collisionData_; };

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

	// JSONデータ
	json jsonData_;

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
	
	// ポインタの取得
	const std::shared_ptr<ActorBase>& GetThis() { return shared_from_this(); };
	
	// 機能の初期化
	virtual void InitFunction() = 0;

	// パラメータの初期化
	virtual void InitParameter() = 0;

	// アニメーションの初期化
	virtual void InitAnimation() = 0;

	// 関数ポインタの初期化
	virtual void InitFunctionPointer() = 0;

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