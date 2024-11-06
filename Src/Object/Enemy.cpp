#include "../Manager/SceneManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/ActorManager.h"
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

	HIT_MOVE_POW = 2000.0f;

}

void Enemy::InitFunctionPointer()
{
	//関数ポインタの初期化
	stateChange_.emplace(STATE::IDLE, std::bind(&Enemy::ChangeIdle, this));
	stateChange_.emplace(STATE::RUN, std::bind(&Enemy::ChangeRun, this));
	stateChange_.emplace(STATE::HIT, std::bind(&Enemy::ChangeHit, this));
	stateChange_.emplace(STATE::HIT_FLY, std::bind(&Enemy::ChangeHitFly, this));
}

void Enemy::InitAnimation()
{

	// アニメーションコントローラの生成
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);

	// アニメーションコントローラーにアニメーションを追加
	for (int i = static_cast<int>(STATE::IDLE); i < static_cast<int>(STATE::MAX); ++i)
	{

		// ループ再生するアニメーションだけisLoopをtrueにする
		bool isLoop = i == static_cast<int>(STATE::IDLE) || i == static_cast<int>(STATE::RUN);
		animationController_->Add(

			// アニメーションの名前
			jsonData_["ANIM"][i - 1]["NAME"],

			// アニメーションのパス
			jsonData_["ANIM"][i - 1]["PATH"],

			// アニメーションが始まる時間
			0.0f,

			// アニメーションスピード
			jsonData_["ANIM"][i - 1]["SPEED"],

			// アニメーションハンドル
			resMng_.LoadModelDuplicate(static_cast<ResourceManager::SRC>(static_cast<int>(ResourceManager::SRC::ENEMY) + i)),

			// アニメーションのループ再生
			isLoop,

			// アニメーション番号
			0,

			// アニメーションの逆再生
			false
		);
	}

	// アニメーション再生するキー
	key_ = "IDLE";

	// 1個前のアニメーション
	preKey_ = key_;

	// 初期状態
	ChangeState(STATE::IDLE);

}

void Enemy::Update(const float deltaTime)
{

	// 移動処理
	Move();

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

void Enemy::AttackHit()
{

	hp_ -= 10;
	ChangeState(STATE::HIT);

}

void Enemy::AttackHitFly()
{
	ChangeState(STATE::HIT_FLY);
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
}

void Enemy::ChangeRun()
{
	stateUpdate_ = std::bind(&Enemy::UpdateRun, this);
}

void Enemy::ChangeHit()
{

	stateUpdate_ = std::bind(&Enemy::UpdateHit, this);

	// ゲームシーンの情報を持ってくる
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	// NULLチェック
	if (!gameScene) return;

	// アクターマネージャーを取得
	auto actorManager = gameScene->GetActorManager();

	// 追従対象
	auto players = actorManager->GetActiveActorData().find(ActorType::PLAYER);

	for (const auto& player : players->second)
	{

		// プレイヤーの方向を求める
		VECTOR vec = VSub(player->GetPos(), transform_.pos);

		// 正規化
		vec = VNorm(vec);

		// プレイヤーの方向と逆方向のベクトル
		vec = { -vec.x, vec.y,-vec.z };

		// 少し前に移動
		movePow_ = VAdd(transform_.pos, VScale(vec, ATTACK_MOVE_POW));

	}

}

void Enemy::ChangeHitFly()
{
	stateUpdate_ = std::bind(&Enemy::UpdateHitFly, this);

	// ゲームシーンの情報を持ってくる
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	// NULLチェック
	if (!gameScene) return;

	// アクターマネージャーを取得
	auto actorManager = gameScene->GetActorManager();

	// 追従対象
	auto players = actorManager->GetActiveActorData().find(ActorType::PLAYER);

	for (const auto& player : players->second)
	{

		// プレイヤーの方向を求める
		VECTOR vec = VSub(player->GetPos(), transform_.pos);

		// 正規化
		vec = VNorm(vec);

		// プレイヤーの方向と逆方向のベクトル
		vec = { -vec.x, vec.y,-vec.z };

		// 上方向に飛ばす
		vec.y = 1.0f;

		// 少し前に移動
		movePow_ = VAdd(transform_.pos, VScale(vec, HIT_MOVE_POW));

	}

}

void Enemy::UpdateIdle()
{

	// ゲームシーンの情報を持ってくる
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	// NULLチェック
	if (!gameScene) return;

	// アクターマネージャーを取得
	auto actorManager = gameScene->GetActorManager();

	// 追従対象
	auto players = actorManager->GetActiveActorData().find(ActorType::PLAYER);

	for (const auto& player : players->second)
	{

		// プレイヤーの方向を求める
		VECTOR vec = VSub(player->GetPos(), transform_.pos);

		// 正規化
		vec = VNorm(vec);

		// 方向を角度に変換する
		float angle = atan2f(vec.x,vec.z);

		// プレイヤー方向に回転
		LazyRotation(angle);

	}

}

void Enemy::UpdateRun()
{
}

void Enemy::UpdateHit()
{

	// 少し前にゆっくり移動
	transform_.pos = Utility::Lerp(transform_.pos, movePow_, 0.1f);

	// アニメーションが終了したら待機状態へ遷移する
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(STATE::IDLE);
	}

}

void Enemy::UpdateHitFly()
{

	// 少し前にゆっくり移動
	transform_.pos = Utility::Lerp(transform_.pos, movePow_, 0.1f);

	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(STATE::IDLE);
	}

}
