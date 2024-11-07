#include <random>
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

	// アニメーションを固定するフレーム名
	FIXATION_FRAME = jsonData_["FIXATION_FRAME_NAME"];

	// 体のフレーム番号を取得
	collisionData_.body = MV1SearchFrame(transform_.modelId, BODY_FRAME.c_str());

	// アニメーションを固定するフレーム番号を取得
	fixationFrame_ = MV1SearchFrame(transform_.modelId, FIXATION_FRAME.c_str());

	// 体の衝突判定の半径
	collisionData_.bodyCollisionRadius = BODY_COLLISION_RADIUS;

	// 攻撃を受けたときに進む力
	HIT_MOVE_POW = 20000.0f;

	// 行動を決めたかどうか
	isActionDecided_ = false;

}

void Enemy::InitFunctionPointer()
{
	//関数ポインタの初期化
	stateChange_.emplace(STATE::IDLE, std::bind(&Enemy::ChangeIdle, this));
	stateChange_.emplace(STATE::RUN, std::bind(&Enemy::ChangeRun, this));
	stateChange_.emplace(STATE::PUNCH, std::bind(&Enemy::ChangePunch, this));
	stateChange_.emplace(STATE::KICK, std::bind(&Enemy::ChangeKick, this));
	stateChange_.emplace(STATE::HIT, std::bind(&Enemy::ChangeHit, this));
	stateChange_.emplace(STATE::HIT_FLY, std::bind(&Enemy::ChangeHitFly, this));
	stateChange_.emplace(STATE::KIP_UP, std::bind(&Enemy::ChangeKipUp, this));
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

	// どの行動をするか決める
	if (!isActionDecided_)
	{
		SelsectAction();
	}

	// 衝突判定の更新
	ActorBase::CollisionUpdate();

	// 状態ごとの更新
	stateUpdate_();

	// アニメーション再生
	animationController_->Update(deltaTime);

	// モデル情報を更新
	transform_.Update();

	// アニメーションのフレームを固定
	AnimationFrame();

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

void Enemy::AnimationFrame()
{

	// 対象フレームのローカル行列を初期値にリセットする
	MV1ResetFrameUserLocalMatrix(transform_.modelId, fixationFrame_);

	// ジャンプ攻撃時に座標を固定する
	if (animationController_->IsBlendPlay("HIT_FLY"))
	{

		// 対象フレームのローカル行列(大きさ、回転、位置)を取得する
		auto mat = MV1GetFrameLocalMatrix(transform_.modelId, fixationFrame_);

		auto scl = MGetSize(mat); // 行列から大きさを取り出す
		auto rot = MGetRotElem(mat); // 行列から回転を取り出す
		auto pos = MGetTranslateElem(mat); // 行列から移動値を取り出す

		// 大きさ、回転、位置をローカル行列に戻す
		MATRIX mix = MGetIdent();
		mix = MMult(mix, MGetScale(scl)); // 大きさ
		mix = MMult(mix, rot); // 回転

		// ここでローカル座標を行列に、そのまま戻さず、
		// 調整したローカル座標を設定する
		mix = MMult(mix, MGetTranslate({ pos.x, pos.y, 0.0f }));

		// 合成した行列を対象フレームにセットし直して、
		// アニメーションの移動値を無効化
		MV1SetFrameUserLocalMatrix(transform_.modelId, fixationFrame_, mix);

	}

}

std::optional<VECTOR> Enemy::GetPlayerPos()
{

	// ゲームシーンの情報を持ってくる
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	// NULLチェック
	if (!gameScene)
	{
		return std::nullopt;
	}

	// アクターマネージャーを取得
	auto actorManager = gameScene->GetActorManager();

	// 追従対象
	auto players = actorManager->GetActiveActorData().find(ActorType::PLAYER);

	// プレイヤーの座標を取得
	for (const auto& player : players->second)
	{
		return player->GetPos();
	}

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

void Enemy::SelsectAction()
{

	if (state_ == STATE::HIT || state_ == STATE::HIT_FLY)return;

	// 乱数
	
	// 非決定的な乱数生成器
	std::random_device rd;

	// メルセンヌ・ツイスタ法による乱数生成器
	std::mt19937 gen(rd());

	// 指定の範囲でランダムな数を取得
	std::uniform_int_distribution<> dist_int(0,1); 
	int number = dist_int(gen);

	if (number == 0)
	{
		// 移動処理
		Move();
	}
	else if (number == 1)
	{
		//攻撃処理
		Attack();
	}

}

void Enemy::Move()
{

	// プレイヤーの座標
	std::optional<VECTOR> playerPos = GetPlayerPos();

	VECTOR pos = playerPos.value();

	// 敵からプレイヤーへのベクトル
	VECTOR vec = VSub(pos, transform_.pos);

	// ベクトルの長さ
	float length = Utility::Magnitude(vec);

	// プレイヤーの近くに移動
	if (length >= 1000.0f)
	{

		ChangeState(STATE::RUN);

		// 行動を決めた
		isActionDecided_ = true;

	}

}

void Enemy::Attack()
{

	// 乱数

	// 非決定的な乱数生成器
	std::random_device rd;

	// メルセンヌ・ツイスタ法による乱数生成器
	std::mt19937 gen(rd());

	// 指定の範囲でランダムな数を取得
	std::uniform_int_distribution<> dist_int(0, 1);
	int number = dist_int(gen);

	// プレイヤーの座標
	std::optional<VECTOR> playerPos = GetPlayerPos();

	VECTOR pos = playerPos.value();

	// 敵からプレイヤーへのベクトル
	VECTOR vec = VSub(pos, transform_.pos);

	// ベクトルの長さ
	float length = Utility::Magnitude(vec);

	// プレイヤーとの距離が遠かったら攻撃できない
	if (length >= 1000.0f) return;

	if (number == 0)
	{

		ChangeState(STATE::PUNCH);

		// 行動を決めた
		isActionDecided_ = true;

	}
	else if (number == 1)
	{

		ChangeState(STATE::KICK);

		// 行動を決めた
		isActionDecided_ = true;

	}

}

void Enemy::ChangeIdle()
{
	stateUpdate_ = std::bind(&Enemy::UpdateIdle, this);
}

void Enemy::ChangeRun()
{
	stateUpdate_ = std::bind(&Enemy::UpdateRun, this);
}

void Enemy::ChangePunch()
{
	stateUpdate_ = std::bind(&Enemy::UpdatePunch, this);
}

void Enemy::ChangeKick()
{
	stateUpdate_ = std::bind(&Enemy::UpdateKick, this);
}

void Enemy::ChangeHit()
{

	stateUpdate_ = std::bind(&Enemy::UpdateHit, this);

	// プレイヤーの座標
	std::optional<VECTOR> playerPos = GetPlayerPos();

	VECTOR pos = playerPos.value();

	// プレイヤーの方向を求める
	VECTOR vec = VSub(pos, transform_.pos);

	// 正規化
	vec = VNorm(vec);

	// プレイヤーの方向と逆方向のベクトル
	vec = { -vec.x, vec.y,-vec.z };

	// 移動量
	movePow_ = VAdd(transform_.pos, VScale(vec, ATTACK_MOVE_POW));

}

void Enemy::ChangeHitFly()
{
	stateUpdate_ = std::bind(&Enemy::UpdateHitFly, this);

	// プレイヤーの座標
	std::optional<VECTOR> playerPos = GetPlayerPos();

	VECTOR pos = playerPos.value();

	// プレイヤーの方向を求める
	VECTOR vec = VSub(pos, transform_.pos);

	// 正規化
	vec = VNorm(vec);

	// プレイヤーの方向と逆方向のベクトル
	vec = { -vec.x, vec.y,-vec.z };

	// 上方向に飛ばす
	vec.y = 0.3f;

	// 移動量
	movePow_ = VAdd(transform_.pos, VScale(vec, HIT_MOVE_POW));

}

void Enemy::ChangeKipUp()
{
	stateUpdate_ = std::bind(&Enemy::UpdateKipUp, this);
}

void Enemy::UpdateIdle()
{

	// ゲームシーンの情報を持ってくる
	std::shared_ptr<GameScene> gameScene =
		std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

	// プレイヤーの座標
	std::optional<VECTOR> playerPos = GetPlayerPos();

	VECTOR pos = playerPos.value();

	// プレイヤーの方向を求める
	VECTOR vec = VSub(pos, transform_.pos);

	// 正規化
	vec = VNorm(vec);

	// 方向を角度に変換する
	float angle = atan2f(vec.x, vec.z);

	// プレイヤー方向に回転
	LazyRotation(angle);

	// 次の行動を考える
	isActionDecided_ = false;

}

void Enemy::UpdateRun()
{

	// プレイヤーの座標
	std::optional<VECTOR> playerPos = GetPlayerPos();

	VECTOR pos = playerPos.value();

	// プレイヤーの方向を求める
	VECTOR vec = VSub(pos, transform_.pos);

	// ベクトルの長さ
	float length = Utility::Magnitude(vec);

	// プレイヤーの近くに来たら待機に遷移
	if (length <= 1000.0f)
	{
		ChangeState(STATE::IDLE);
	}
	// 正規化
	vec = VNorm(vec);

	// 方向を角度に変換する
	float angle = atan2f(vec.x, vec.z);

	// プレイヤー方向に回転
	LazyRotation(angle);

	// 移動量
	movePow_ = VScale(vec, 100.0f);

	// プレイヤー方向に移動
 	transform_.pos = VAdd(transform_.pos, movePow_);

}

void Enemy::UpdatePunch()
{

	// アニメーションが終了したら待機状態へ遷移する
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(STATE::IDLE);
	}

}

void Enemy::UpdateKick()
{

	// アニメーションが終了したら待機状態へ遷移する
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(STATE::IDLE);
	}

}

void Enemy::UpdateHit()
{

	// 少し後ろにゆっくり移動
	transform_.pos = Utility::Lerp(transform_.pos, movePow_, 0.1f);

	// アニメーションが終了したら待機状態へ遷移する
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(STATE::IDLE);
	}

}

void Enemy::UpdateHitFly()
{

	// 後ろにゆっくり移動
	transform_.pos = Utility::Lerp(transform_.pos, movePow_, 0.05f);

	// アニメーションが終了したら起き上がり状態へ遷移する
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(STATE::KIP_UP);
	}

}

void Enemy::UpdateKipUp()
{

	// アニメーションが終了したら待機状態へ遷移する
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(STATE::IDLE);
	}

}
