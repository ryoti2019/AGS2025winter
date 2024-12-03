#pragma once
#include <DxLib.h>
#include <memory>
#include <vector>
#include "../Lib/nlohmann/json.hpp"
#include "../Common/Vector2F.h"
#include "../Object/Common/Transform.h"
#include "../Component/MoveComponent.h"
#include "../Manager/ResourceManager.h"
#include "../Object/Common/AnimationController.h"

class Component;
class Camera;

using json = nlohmann::json;

enum class ActorType {
	NONE = -1,
	PLAYER,
	ENEMY,
	STAGE,
	MAX
};

class ActorBase : public std::enable_shared_from_this<ActorBase>
{

public:

	struct CollisionData
	{

#pragma region 攻撃やステージとの当たり判定で使うもの

#pragma region フレーム

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

#pragma endregion

#pragma region 中心座標

		// 右手の中心座標
		VECTOR rightHandPos;

		// 左手の中心座標
		VECTOR leftHandPos;

		// 右足の中心座標
		VECTOR rightFootPos;

		// 左足の中心座標
		VECTOR leftFootPos;

		// 体全体の中心座標
		VECTOR bodyPos;

#pragma endregion

#pragma region カプセルの上座標

		// 右手のカプセルの上座標
		VECTOR rightHandCapsuleUpPos;

		// 左手のカプセルの上座標
		VECTOR leftHandCapsuleUpPos;

		// 右足のカプセルの上座標
		VECTOR rightFootCapsuleUpPos;

		// 左足のカプセルの上座標
		VECTOR leftFootCapsuleUpPos;

		// 体のカプセルの上座標
		VECTOR bodyCapsuleUpPos;

#pragma endregion

#pragma region カプセルの下座標

		// 右手のカプセルの下座標
		VECTOR rightHandCapsuleDownPos;

		// 左手のカプセルの下座標
		VECTOR leftHandCapsuleDownPos;

		// 右足のカプセルの下座標
		VECTOR rightFootCapsuleDownPos;

		// 左足のカプセルの下座標
		VECTOR leftFootCapsuleDownPos;

		// 体のカプセルの下座標
		VECTOR bodyCapsuleDownPos;

#pragma endregion

#pragma region 向き

		// 右手の向き
		Quaternion rightHandRot;

		// 左手の向き
		Quaternion leftHandRot;

		// 右足の向き
		Quaternion rightFootRot;

		// 左足の向き
		Quaternion leftFootRot;

#pragma endregion

#pragma region 半径

		// 手足の当たり判定の半径
		float handAndFootCollisionRadius;

		// 体の当たり判定の半径
		float bodyCollisionRadius;

#pragma endregion

#pragma region 攻撃判定があるかどうか

		// 右手に攻撃判定があるかどうか
		bool isRightHandAttack;

		// 左手に攻撃判定があるかどうか
		bool isLeftHandAttack;

		// 右足に攻撃判定があるかどうか
		bool isRightFootAttack;

		// 左足に攻撃判定があるかどうか
		bool isLeftFootAttack;

#pragma endregion

#pragma endregion

#pragma region プレイヤーと敵が重ならないようにするために当たり判定で使うもの

		// 最小点
		VECTOR minPos;

		// 最大点
		VECTOR maxPos; 

#pragma endregion

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

	// 体のカプセルの中心の相対座標
	const VECTOR BODY_RELATIVE_CENTER_POS;

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

	// 走るときの移動量
	float RUN_MOVE_POW;

	// 攻撃するときに進む移動量
	const float ATTACK_MOVE_POW;
	
	// 重力加速度
	const float GRAVITY;

	// 攻撃を受けたときに進む移動量
	float HIT_MOVE_POW;

	// 回転量
	const float ROTATION_POW;

	// 手足の当たり判定の半径
	const float HAND_AND_FOOT_COLLISION_RADIUS;

	// 体の当たり判定の半径
	const float BODY_COLLISION_RADIUS;

	// 最初に向いている角度
	float INIT_ANGLE;

	// 床の高さ
	const float FLOOR_HEIGHT;

	// HPの最大値
	int HP_MAX;

	// アニメーション番号
	int ANIM_INDEX;

	ActorBase(const VECTOR& pos, const json& data);

	virtual ~ActorBase() = default;

	virtual void Init(const VECTOR& pos);
	void Create(const VECTOR& pos);
	virtual void Update(const float deltaTime);
	virtual void Draw();

	// 座標を設定
	void SetPos(const VECTOR& pos) {
		transform_->pos = pos;
		transform_->Update();
		// 衝突判定の更新
		ActorBase::CollisionUpdate();
	}

	// 相手の座標を取得
	const VECTOR& GetTargetPos()const { return targetPos_; }

	// 相手の座標を設定
	void SetTargetPos(const VECTOR& pos) { targetPos_ = pos; }

	// 生存判定を設定
	void SetIsActive(const bool isActive) { isActive_ = isActive; }

	// 座標を足す
	void AddPos(const VECTOR& pos);

	// 座標を足す
	void SubPos(const VECTOR& pos) { transform_->pos = VSub(transform_->pos, pos); }

	// HPを設定
	void SetHp(const int hp) { hp_ = hp; }

	// HPを減らす
	void SubHp(const int hp) { hp_ -= hp;}

	// ロックオンされているか判定を設定
	void SetIsLockOn(const bool isLockOn);

	// モデルの基本情報を取得
	const std::shared_ptr<Transform>& GetTransform() const { return transform_; }

	// アクタータイプを取得
	const ActorType& GetActorType() const { return actorType_; }

	// コリジョンデータのを取得
	const CollisionData& GetCollisionData() const { return collisionData_; }

	// アクターの重力方向の速度の取得
	const VECTOR& GetVelocity()const { return velocity_; }

	// アクターの重力方向の速度の設定
	void SetVelocity(const VECTOR& velocity) { velocity_ = velocity; }

	// 生存判定を取得
	const bool GetIsActive() const { return isActive_; }

	// 攻撃状態を取得
	virtual const bool GetAttackState()const ;

	// 攻撃種類を取得
	virtual const std::vector<int> GetTotalAttackTypes()const { return {}; }

	// 攻撃を受けている状態を取得
	virtual const bool GetHitState()const;

	// 攻撃が当たっているか設定
	void SetIsAttackHit(const bool hit) { isAttackHit_ = hit; }

	// 攻撃が当たっているか
	const bool GetIsAttackHit()const { return isAttackHit_; }

	// 地面に当たっているか
	void SetIsOnGround(const bool isOnGround) { isOnGround_ = isOnGround; }

	// HPを取得
	const int GetHp()const { return hp_; }

	// ロックオンされているか判定を取得
	const bool GetIsLockOn()const;

	// 攻撃のヒット処理
	virtual void AttackHit(const int damage, const int state);

	// 今の状態を取得
	virtual const int GetState()const { return 0; }

	// ダメージ量を取得
	virtual const int GetDamage()const { return 0; }

	// スピードを取得
	virtual const float GetSpeed()const { return speed_; }

	// 方向を取得
	const VECTOR& GetDir()const { return dir_; }

	// 方向を設定
	void SetDir(const VECTOR& dir) { dir_ = dir; }

	// 実際に動く方向を取得
	const VECTOR& GetMoveDir()const { return moveDir_; }

	// アニメーションコントローラーに状態を文字型で渡す変数を取得
	const std::string& GetKey()const { return key_; }

	// アニメーション
	const std::unique_ptr<AnimationController>& GetAnimationController()const { return animationController_; }

	// JSONデータ
	const json& GetJsonData()const { return jsonData_; }

protected:

	// リソース管理
	ResourceManager& resMng_;

	// アニメーション
	std::unique_ptr<AnimationController> animationController_;

	// 移動のコンポーネント
	std::shared_ptr<MoveComponent> moveComponent_;

	// モデル制御の基本情報
	std::shared_ptr<Transform> transform_;

	// 衝突判定のデータ
	CollisionData collisionData_;

	// JSONデータ
	json jsonData_;

	// アクターの種類
	ActorType actorType_;

	// 方向
	VECTOR dir_;

	// 実際に動く方向
	VECTOR moveDir_;

	// 移動量
	VECTOR movePow_;

	// 相手の座標
	VECTOR targetPos_;

	// 物体の速度
	VECTOR velocity_;

	// 物体の加速度(重力)
	VECTOR acceleration_;

	// 目的の角度
	Quaternion goalQuaRot_;

	// アニメーションコントローラーに状態を文字型で渡す変数
	std::string key_;

	// 1個前のアニメーション
	std::string preKey_;

	// HP
	int hp_;

	// モデルID
	int modelId_;

	// ダメージ量
	int damage_;

	// スピード
	float speed_;

	// モデルの大きさ
	float scl_;

	// 回転する時間
	float stepRotTime_;

	// 重力を緩くする
	float gravityScale_;

	// 生きているか
	bool isAlive_;

	// 攻撃が当たっているか
	bool isAttackHit_;

	// 地面に当たっているか
	bool isOnGround_;

	// ロックオンされているか(敵のみ)
	bool isLockOn_;

	// アクティブ状態かどうか
	bool isActive_;
	
	// ポインタの取得
	const std::shared_ptr<ActorBase>& GetThis() { return shared_from_this(); };
	
	// 機能の初期化
	virtual void InitFunction();

	// パラメータの初期化
	virtual void InitParameter();

	// アニメーションの初期化
	virtual void InitAnimation();

	// 関数ポインタの初期化
	virtual void InitFunctionPointer();

	// ImGuiのデバッグ描画の更新
	virtual void UpdateDebugImGui();

	// 衝突判定の更新
	void CollisionUpdate();

	// 移動処理
	virtual void Move();

	// 攻撃処理
	virtual void Attack(const float deltaTime);

	// 遅延回転
	void LazyRotation(float goalRot);

	// 衝突判定で使うものを登録
	void CollisionRegister();

	// アニメーションのフレーム固定
	virtual void AnimationFrame();

	// どのヒットアニメーションかチェックする
	virtual void AttackHitCheck(const int state);

	// 重力
	void Gravity(const float scale);

	// HPが0になったら死亡アニメーションに遷移
	virtual void DeathAnim(int state);

private:

	// デバッグ描画
	void DrawDebug();

};