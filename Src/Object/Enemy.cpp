#include "../Manager/SceneManager.h"
#include "../Manager/CollisionManager.h"
#include "../Scene/GameScene.h"
#include "Enemy.h"

Enemy::Enemy(const VECTOR& pos, const json& data)
	:
	ActorBase(pos, data)
{

	// 機能の初期化
	InitFunction();

	// モデルID
	modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY);

	// モデルの大きさ
	scl_ = 10.0f;

	// 関数ポインタの初期化
	InitFunctionPointer();

	// パラメータの初期化
	InitParameter();

	// アニメーションの初期化
	InitAnimation();

}

void Enemy::Init(const VECTOR& pos)
{

	// 機能の初期化
	InitFunction();

	// モデルID
	modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY);

	// モデルの大きさ
	scl_ = 10.0f;

	// 共通部分は基底クラスで初期化
	ActorBase::Init(pos);

	// 関数ポインタの初期化
	InitFunctionPointer();

	// パラメータの初期化
	InitParameter();

	// アニメーションの初期化
	InitAnimation();

}

void Enemy::InitFunction()
{

}

void Enemy::InitParameter()
{

	// 体のフレーム名
	BODY_FRAME = jsonData_["BODY_FRAME_NAME"];

	// 体のフレーム番号を取得
	collisionData_.body = MV1SearchFrame(transform_.modelId, BODY_FRAME.c_str());

	// 体の衝突判定の半径
	collisionData_.bodyCollisionRadius = BODY_COLLISION_RADIUS;

}

void Enemy::InitFunctionPointer()
{
	//関数ポインタの初期化
	stateChange_.emplace(STATE::IDLE, std::bind(&Enemy::ChangeIdle, this));
	stateChange_.emplace(STATE::RUN, std::bind(&Enemy::ChangeRun, this));
}

void Enemy::InitAnimation()
{

	// アニメーションコントローラの生成
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);

	// 待機
	animationController_->Add("IDLE", "Data/Model/Enemy/Idle.mv1", 0.0f, 30.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY_IDLE), true, 0, false);

	// 走る
	animationController_->Add("RUN", "Data/Model/Enemy/Run.mv1", 0.0f, 30.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY_RUN), true, 0, false);

	// アニメーション再生するキー
	key_ = "IDLE";

	// 1個前のアニメーション
	preKey_ = key_;

	// 初期状態
	ChangeState(STATE::RUN);

}

void Enemy::Update(const float deltaTime)
{

	// 移動処理
	Move();

	if (hp_ <= 0)
	{
		ChangeState(STATE::IDLE);
	}

	// 衝突判定の更新
	ActorBase::CollisionUpdate();

	// 状態ごとの更新
	stateUpdate_();

	// アニメーション再生
	animationController_->Update(deltaTime);

	// モデル情報を更新
	transform_.Update();

}

bool Enemy::GetAttackState()
{
	return false;
}

void Enemy::Move()
{
}

void Enemy::ComboAttack(const float deltaTime)
{
}

bool Enemy::AttackState()
{
	return false;
}

void Enemy::ChangeState(STATE state)
{

	// 状態遷移
	state_ = state;

	// 関数ポインタの遷移
	stateChange_[state_]();

	// 前のアニメーションを保存
	preKey_ = key_;

	// 新しいアニメーションを保存
	key_ = ANIM_DATA_KEY[static_cast<int>(state)];

	// アニメーションコントローラー側のアニメーション遷移
	animationController_->ChangeAnimation(key_);

}

void Enemy::ChangeIdle()
{
	stateUpdate_ = std::bind(&Enemy::UpdateIdle, this);
	stateDraw_ = std::bind(&Enemy::DrawIdle, this);
}

void Enemy::ChangeRun()
{
	stateUpdate_ = std::bind(&Enemy::UpdateRun, this);
	stateDraw_ = std::bind(&Enemy::DrawRun, this);
}

void Enemy::UpdateIdle()
{
}

void Enemy::UpdateRun()
{
}

void Enemy::DrawIdle()
{
}

void Enemy::DrawRun()
{
}