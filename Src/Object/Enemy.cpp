#include <random>
#include "../Manager/SceneManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/ActorManager.h"
#include "../Scene/GameScene.h"
#include "Enemy.h"

Enemy::Enemy(const VECTOR& pos, const json& data)
	:
	ActorBase(pos, data),
	PUNCH_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(EnemyState::PUNCH) - 1]["ATTACK_START_FRAME"]),
	PUNCH_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(EnemyState::PUNCH) - 1]["ATTACK_END_FRAME"]),
	KICK_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(EnemyState::KICK) - 1]["ATTACK_START_FRAME"]),
	KICK_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(EnemyState::KICK) - 1]["ATTACK_END_FRAME"]),
	COOL_TIME(data["COOL_TIME"])
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

	// 右手のフレーム名
	RIGHT_HAND_FRAME = jsonData_["RIGHT_HAND_FRAME_NAME"];

	// 左手のフレーム名
	LEFT_HAND_FRAME = jsonData_["LEFT_HAND_FRAME_NAME"];

	// 右足のフレーム名
	RIGHT_FOOT_FRAME = jsonData_["RIGHT_FOOT_FRAME_NAME"];

	// 左足のフレーム名
	LEFT_FOOT_FRAME = jsonData_["LEFT_FOOT_FRAME_NAME"];

	// 体のフレーム名
	BODY_FRAME = jsonData_["BODY_FRAME_NAME"];

	// 右手のフレーム番号を取得
	collisionData_.rightHand = MV1SearchFrame(transform_.modelId, RIGHT_HAND_FRAME.c_str());

	// 左手のフレーム番号を取得
	collisionData_.leftHand = MV1SearchFrame(transform_.modelId, LEFT_HAND_FRAME.c_str());

	// 右足のフレーム番号を取得
	collisionData_.rightFoot = MV1SearchFrame(transform_.modelId, RIGHT_FOOT_FRAME.c_str());

	// 左足のフレーム番号を取得
	collisionData_.leftFoot = MV1SearchFrame(transform_.modelId, LEFT_FOOT_FRAME.c_str());

	// 体のフレーム番号を取得
	collisionData_.body = MV1SearchFrame(transform_.modelId, BODY_FRAME.c_str());

	// 手足の衝突判定の半径
	collisionData_.handAndFootCollisionRadius = HAND_AND_FOOT_COLLISION_RADIUS;

	// 体の衝突判定の半径
	collisionData_.bodyCollisionRadius = BODY_COLLISION_RADIUS;

	// 右手に攻撃判定があるかどうか
	collisionData_.isRightHandAttack = false;

	// 左手に攻撃判定があるかどうか
	collisionData_.isLeftHandAttack = false;

	// 右足に攻撃判定があるかどうか
	collisionData_.isRightFootAttack = false;

	// 左足に攻撃判定があるかどうか
	collisionData_.isLeftFootAttack = false;

	// プレイヤーの座標
	std::optional<VECTOR> playerPos = GetPlayerPos();

	// 相手の座標
	targetPos_ = playerPos.value();

	// 攻撃を受けたときに進む力
	HIT_MOVE_POW = 20000.0f;

	// クールタイム
	coolTime_ = 0.0f;

	// 行動を決めたかどうか
	isActionDecided_ = false;

}

void Enemy::InitFunctionPointer()
{
	//関数ポインタの初期化
	stateChange_.emplace(EnemyState::IDLE, std::bind(&Enemy::ChangeIdle, this));
	stateChange_.emplace(EnemyState::RUN, std::bind(&Enemy::ChangeRun, this));
	stateChange_.emplace(EnemyState::PUNCH, std::bind(&Enemy::ChangePunch, this));
	stateChange_.emplace(EnemyState::KICK, std::bind(&Enemy::ChangeKick, this));
	stateChange_.emplace(EnemyState::HIT_HEAD, std::bind(&Enemy::ChangeHitHead, this));
	stateChange_.emplace(EnemyState::HIT_BODY, std::bind(&Enemy::ChangeHitBody, this));
	stateChange_.emplace(EnemyState::HIT_FLY, std::bind(&Enemy::ChangeHitFly, this));
	stateChange_.emplace(EnemyState::KIP_UP, std::bind(&Enemy::ChangeKipUp, this));
}

void Enemy::InitAnimation()
{

	// アニメーションコントローラの生成
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);

	// アニメーションコントローラーにアニメーションを追加
	for (int i = static_cast<int>(EnemyState::IDLE); i < static_cast<int>(EnemyState::MAX); ++i)
	{

		// ループ再生するアニメーションだけisLoopをtrueにする
		bool isLoop = i == static_cast<int>(EnemyState::IDLE) || i == static_cast<int>(EnemyState::RUN);
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
			1,

			// アニメーションの逆再生
			false
		);
	}

	// アニメーション再生するキー
	key_ = "IDLE";

	// 1個前のアニメーション
	preKey_ = key_;

	// 初期状態
	ChangeState(EnemyState::IDLE);

}

void Enemy::Update(const float deltaTime)
{

	// クールタイムを計算
	coolTime_ -= deltaTime;

	// どの行動をするか決める
	if (!isActionDecided_ && coolTime_ <= 0.0f)
	{
		//SelsectAction();
	}

	// 状態ごとの更新
	stateUpdate_();

	// 重力
	Gravity();

	// アニメーション再生
	animationController_->Update(deltaTime);

	// モデル情報を更新
	transform_.Update();

	// 衝突判定の更新
	ActorBase::CollisionUpdate();

	// アニメーションのフレームを固定
	AnimationFrame();

}

void Enemy::Draw()
{

	ActorBase::Draw();

	// HPバー
	int hpLength = 200;
	int hpGauge;

	hpGauge = hpLength * hp_ / HP_MAX;

	// HPを描画
	DrawBox(0, 100, 0 + hpGauge, 120, 0xff0000, true);

	DrawFormatString(0, 15, 0xff0000, "velocity:(%0.2f,%0.2f,%0.2f)", velocity_.x, velocity_.y, velocity_.z);

}

bool Enemy::GetAttackState()
{

	// 攻撃の状態か判定
	for (const auto state : attackState_)
	{
		if (state_ == state)
		{
			return true;
		}
	}

	return false;

}

bool Enemy::GetHitState()
{

	// 攻撃を受けている状態か判定
	for (const auto state : hitState_)
	{
		if (state_ == state)
		{
			return true;
		}
	}

	return false;

}

void Enemy::AttackHit(const int damage, const int state)
{

	// 頭にヒットするアニメーションかチェック
	for (const auto hitState : hitHeadState_)
	{
		if (hitState == static_cast<PlayerState>(state))
		{
			ChangeState(EnemyState::HIT_HEAD);
		}
	}

	// 体にヒットするアニメーションかチェック
	for (const auto hitState : hitBodyState_)
	{
		if (hitState == static_cast<PlayerState>(state))
		{
			ChangeState(EnemyState::HIT_BODY);
		}
	}

	// 吹っ飛んでいくアニメーションかチェック
	for (const auto hitState : hitFlyState_)
	{
		if (hitState == static_cast<PlayerState>(state))
		{
			ChangeState(EnemyState::HIT_FLY);
		}
	}

	// HPを減らす
	SubHp(damage);
	
	// アニメーションの再生時間をリセットする
	animationController_->ResetStepAnim();

}

void Enemy::AnimationFrame()
{

	// 対象フレームのローカル行列を初期値にリセットする
	MV1ResetFrameUserLocalMatrix(transform_.modelId, collisionData_.body);

	// 座標を固定する
	if (animationController_->IsBlendPlay("HIT_FLY"))
	{

		// 対象フレームのローカル行列(大きさ、回転、位置)を取得する
		auto mat = MV1GetFrameLocalMatrix(transform_.modelId, collisionData_.body);

		auto scl = MGetSize(mat); // 行列から大きさを取り出す
		auto rot = MGetRotElem(mat); // 行列から回転を取り出す
		auto pos = MGetTranslateElem(mat); // 行列から移動値を取り出す

		// 大きさ、回転、位置をローカル行列に戻す
		MATRIX mix = MGetIdent();
		mix = MMult(mix, MGetScale(scl)); // 大きさ
		mix = MMult(mix, rot); // 回転

		// ここでローカル座標を行列に、そのまま戻さず、
		// 調整したローカル座標を設定する
		mix = MMult(mix, MGetTranslate({ 0.0f, 0.0f, 0.0f }));

		// 合成した行列を対象フレームにセットし直して、
		// アニメーションの移動値を無効化
		MV1SetFrameUserLocalMatrix(transform_.modelId, collisionData_.body, mix);

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

void Enemy::ChangeState(EnemyState state)
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

	// ヒット中は行動できない
	for (const auto hitState : hitState_)
	{
		if (hitState == state_)
		{
			return;
		}
	}

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
	targetPos_ = playerPos.value();

	// 敵からプレイヤーへのベクトル
	VECTOR vec = VSub(targetPos_, transform_.pos);

	// ベクトルの長さ
	float length = Utility::Magnitude(vec);

	// プレイヤーの近くに移動
	if (length >= 1000.0f)
	{

		ChangeState(EnemyState::RUN);

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
	//int number = 1;

	// プレイヤーの座標
	std::optional<VECTOR> playerPos = GetPlayerPos();

	// 相手の座標
	targetPos_ = playerPos.value();

	// 敵からプレイヤーへのベクトル
	VECTOR vec = VSub(targetPos_, transform_.pos);

	// ベクトルの長さ
	float length = Utility::Magnitude(vec);

	// プレイヤーとの距離が遠かったら攻撃できない
	if (length >= 1000.0f) return;

	if (number == 0)
	{

		ChangeState(EnemyState::PUNCH);

		// 行動を決めた
		isActionDecided_ = true;

	}
	else if (number == 1)
	{

		ChangeState(EnemyState::KICK);

		// 行動を決めた
		isActionDecided_ = true;

	}

}

void Enemy::ChangeIdle()
{

	stateUpdate_ = std::bind(&Enemy::UpdateIdle, this);

	// 右手の攻撃判定をなくす
	collisionData_.isRightHandAttack = false;

	// 左手の攻撃判定をなくす
	collisionData_.isLeftHandAttack = false;

	// 右足の攻撃判定をなくす
	collisionData_.isRightFootAttack = false;

	// 左足の攻撃判定をなくす
	collisionData_.isLeftFootAttack = false;

}

void Enemy::ChangeRun()
{
	stateUpdate_ = std::bind(&Enemy::UpdateRun, this);
}

void Enemy::ChangePunch()
{

	stateUpdate_ = std::bind(&Enemy::UpdatePunch, this);

	// 攻撃が当たっているかをリセットする
	isAttackHit_ = false;

}

void Enemy::ChangeKick()
{

	stateUpdate_ = std::bind(&Enemy::UpdateKick, this);

	// 攻撃が当たっているかをリセットする
	isAttackHit_ = false;

}

void Enemy::ChangeHitHead()
{

	stateUpdate_ = std::bind(&Enemy::UpdateHitHead, this);

	// プレイヤーの方向を求める
	VECTOR vec = VSub(targetPos_, transform_.pos);

	// 正規化
	vec = VNorm(vec);

	// プレイヤーの方向と逆方向のベクトル
	vec = { -vec.x, vec.y,-vec.z };

	// 移動量
	movePow_ = VAdd(transform_.pos, VScale(vec, ATTACK_MOVE_POW));

}

void Enemy::ChangeHitBody()
{

	stateUpdate_ = std::bind(&Enemy::UpdateHitBody, this);

	// プレイヤーの方向を求める
	VECTOR vec = VSub(targetPos_, transform_.pos);

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

	// プレイヤーの方向を求める
	VECTOR vec = VSub(targetPos_, transform_.pos);
	
	// 正規化
	vec = VNorm(vec);

	// プレイヤーの方向と逆方向のベクトル
	vec = { -vec.x, vec.y,-vec.z };

	// 上方向に飛ばす
	velocity_.y += 11.0f;
	vec.y = velocity_.y;

	// 移動量
	movePow_ = VScale(vec,10000.0f);

	movePow_ = VAdd(transform_.pos, movePow_);

}

void Enemy::ChangeKipUp()
{
	stateUpdate_ = std::bind(&Enemy::UpdateKipUp, this);
}

void Enemy::UpdateIdle()
{

	// プレイヤーの方向を求める
	VECTOR vec = VSub(targetPos_, transform_.pos);

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

	// プレイヤーの方向を求める
	VECTOR vec = VSub(targetPos_, transform_.pos);

	// ベクトルの長さ
	float length = Utility::Magnitude(vec);

	// プレイヤーの近くに来たら待機に遷移
	if (length <= 1000.0f)
	{
		ChangeState(EnemyState::IDLE);
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

	// 攻撃判定があるフレーム
	if (PUNCH_ATTACK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= PUNCH_ATTACK_END_FRAME)
	{
		collisionData_.isRightHandAttack = true;
	}
	else
	{
		collisionData_.isRightHandAttack = false;
	}

	// アニメーションが終了したら待機状態へ遷移する
	if (animationController_->IsEndPlayAnimation())
	{

		ChangeState(EnemyState::IDLE);

		// クールタイムを設定
		coolTime_ = COOL_TIME;

	}

}

void Enemy::UpdateKick()
{

	// 攻撃判定があるフレーム
	if (KICK_ATTACK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= KICK_ATTACK_END_FRAME)
	{
		collisionData_.isRightFootAttack = true;
	}
	else
	{
		collisionData_.isRightFootAttack = false;
	}

	// アニメーションが終了したら待機状態へ遷移する
	if (animationController_->IsEndPlayAnimation())
	{

		ChangeState(EnemyState::IDLE);

		// クールタイムを設定
		coolTime_ = COOL_TIME;

	}

}

void Enemy::UpdateHitHead()
{

	// 少し後ろにゆっくり移動
	transform_.pos = Utility::Lerp(transform_.pos, movePow_, 0.1f);

	// アニメーションが終了したら待機状態へ遷移する
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(EnemyState::IDLE);
	}

}

void Enemy::UpdateHitBody()
{

	// 少し後ろにゆっくり移動
	transform_.pos = Utility::Lerp(transform_.pos, movePow_, 0.1f);

	// アニメーションが終了したら待機状態へ遷移する
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(EnemyState::IDLE);
	}

}

void Enemy::UpdateHitFly()
{

	if (transform_.pos.x != movePow_.x || transform_.pos.y != movePow_.y || transform_.pos.z != movePow_.z)
	{
		// 後ろにゆっくり移動
		transform_.pos = Utility::Lerp(transform_.pos, movePow_, 0.5f);
	}

	//transform_.pos = VAdd(transform_.pos, movePow_);

	// アニメーションが終了したら起き上がり状態へ遷移する
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(EnemyState::KIP_UP);
	}

}

void Enemy::UpdateKipUp()
{

	// アニメーションが終了したら待機状態へ遷移する
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(EnemyState::IDLE);
	}

}
