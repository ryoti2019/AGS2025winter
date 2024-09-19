#pragma once
#include <DxLib.h>
#include "../Common/Vector2F.h"

enum class ActorType {
	NONE = -1,
	PLAYER,
	ENEMY,
	MAX
};

class ActorBase
{

public:

	ActorBase(const VECTOR& pos);

	virtual ~ActorBase() = default;

	virtual void Init(const VECTOR& pos);
	virtual void Create(const VECTOR& pos);
	virtual void Update(const float deltaTime);
	virtual void Draw();

	void SetPos(const VECTOR& pos) { pos_ = pos; };

	void SetIsActive(const bool isActive) { isActive_ = isActive; }

	const ActorType GetActorType() const { return actorType_; }

protected:

#pragma region オブジェクトの情報

	// 移動後座標
	VECTOR movedPos_;

	// スピード
	float speed_;

	// 移動量
	float movePow_;

	// HP
	int hp_;

	// 衝突しているか
	bool isHit_;

	// アクターの種類
	ActorType actorType_;

	// 生きているかどうか
	bool isAlive_;

#pragma endregion

private:

	// 座標
	VECTOR pos_;

	// アクティブ状態かどうか
	bool isActive_;

};