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

	// 回転完了までの時間
	const float ROTATION_POW;

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

	// 方向
	VECTOR dir_;

	// 目的の角度
	Quaternion goalQuaRot_;

	// スピード
	float speed_;

	// HP
	int hp_;

	// アクターの種類
	ActorType actorType_;

	// 生きているかどうか
	bool isAlive_;

	// 回転する時間
	float stepRotTime_;

	// 移動処理
	virtual void Move() = 0;

	// 遅延回転
	void LazyRotation(float goalRot);

#pragma endregion

private:

	// アクティブ状態かどうか
	bool isActive_;

};