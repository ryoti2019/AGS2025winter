#include <random>
#include "../Lib/ImGui/imgui.h"
#include "../Manager/SceneManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/ActorManager.h"
#include "../Scene/GameScene.h"
#include "Enemy.h"

Enemy::Enemy(const VECTOR& pos, const json& data)
	:
	ActorBase(pos, data),
	PUNCH_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(EnemyState::ATTACK_PUNCH) - 1]["ATTACK_START_FRAME"]),
	PUNCH_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(EnemyState::ATTACK_PUNCH) - 1]["ATTACK_END_FRAME"]),
	KICK_ATTACK_START_FRAME(data["ANIM"][static_cast<int>(EnemyState::ATTACK_KICK) - 1]["ATTACK_START_FRAME"]),
	KICK_ATTACK_END_FRAME(data["ANIM"][static_cast<int>(EnemyState::ATTACK_KICK) - 1]["ATTACK_END_FRAME"]),
	COOL_TIME(data["COOL_TIME"]),
	ACTIVATION_DISTANCE(data["ACTIVATION_DISTANCE"]),
	HIT_FLY_UP_VEC_POW(data["HIT_FLY_UP_VEC_POW"]),
	HIT_FLY_MOVE_POW(data["HIT_FLY_MOVE_POW"]),
	TRACKING_MAX_TIME(data["TRACKING_MAX_TIME"]),
	KNOCK_BACK_TIME(data["KNOCK_BACK_TIME"]),
	KNOCK_BACK_HEIGHT_OFFSET(data["KNOCK_BACK_HEIGHT_OFFSET"]),
	FLINCH_UP_UP_VEC_POW(data["FLINCH_UP_UP_VEC_POW"]),
	FLINCH_UP_SPEED(data["FLINCH_UP_SPEED"]),
	FLINCH_UP_ANGLE_X(data["FLINCH_UP_ANGLE_X"]),
	FLINCH_UP_GRAVITY_SCALE(data["FLINCH_UP_GRAVITY_SCALE"]),
	MOVE_RATE(data["MOVE_RATE"])
{

	// 機能の初期化
	InitFunction();

	// モデルID
	modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY);

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

	// 走るときの移動量
	RUN_MOVE_POW = jsonData_["RUN_MOVE_POW"];

	// 攻撃を受けたときに進む力
	HIT_MOVE_POW = jsonData_["HIT_MOVE_POW"];

	// アニメーション番号
	ANIM_INDEX = jsonData_["ANIM_INDEX"];

	// クールタイム
	coolTime_ = 0.0f;

	// 追いかけている時間
	trackingTime_ = 0.0f;

	// 敵がまっすく飛んでいくときのカウンタ
	knockBackCnt_ = 0.0f;

	// 行動を決めたかどうか
	isActionDecided_ = false;

	// すでに角度が変わっているかどうか
	isChangeAngle_ = false;

}

void Enemy::InitFunctionPointer()
{

	//関数ポインタの初期化
	stateChange_.emplace(EnemyState::IDLE, std::bind(&Enemy::ChangeIdle, this));
	stateChange_.emplace(EnemyState::RUN, std::bind(&Enemy::ChangeRun, this));
	stateChange_.emplace(EnemyState::ATTACK_PUNCH, std::bind(&Enemy::ChangePunch, this));
	stateChange_.emplace(EnemyState::ATTACK_KICK, std::bind(&Enemy::ChangeKick, this));
	stateChange_.emplace(EnemyState::HIT_HEAD, std::bind(&Enemy::ChangeHitHead, this));
	stateChange_.emplace(EnemyState::HIT_BODY, std::bind(&Enemy::ChangeHitBody, this));
	stateChange_.emplace(EnemyState::HIT_FLY, std::bind(&Enemy::ChangeHitFly, this));
	stateChange_.emplace(EnemyState::HIT_FLINCH_UP, std::bind(&Enemy::ChangeHitFlinchUp, this));
	stateChange_.emplace(EnemyState::HIT_KNOCK_BACK, std::bind(&Enemy::ChangeHitKnockBack, this));
	stateChange_.emplace(EnemyState::KIP_UP, std::bind(&Enemy::ChangeKipUp, this));
	stateChange_.emplace(EnemyState::DEATH, std::bind(&Enemy::ChangeDeath, this));

}

void Enemy::UpdateDebugImGui()
{

	// ウィンドウタイトル&開始処理
	ImGui::Begin("Enemy");

	// 大きさ
	ImGui::Text("scale");
	ImGui::InputFloat("Scl", &scl_);

	// 角度
	VECTOR rotDeg = VECTOR();
	rotDeg.x = Utility::Rad2DegF(transform_.quaRot.x);
	rotDeg.y = Utility::Rad2DegF(transform_.quaRot.y);
	rotDeg.z = Utility::Rad2DegF(transform_.quaRot.z);
	ImGui::Text("angle(deg)");
	ImGui::SliderFloat("RotX", &rotDeg.x, -90.0f, 90.0f);
	ImGui::SliderFloat("RotY", &rotDeg.y, -90.0f, 90.0f);
	ImGui::SliderFloat("RotZ", &rotDeg.z, -90.0f, 90.0f);
	transform_.quaRot.x = Utility::Deg2RadF(rotDeg.x);
	transform_.quaRot.y = Utility::Deg2RadF(rotDeg.y);
	transform_.quaRot.z = Utility::Deg2RadF(rotDeg.z);

	// 位置
	ImGui::Text("position");
	// 構造体の先頭ポインタを渡し、xyzと連続したメモリ配置へアクセス
	ImGui::InputFloat3("Pos", &transform_.pos.x);
	// 終了処理
	ImGui::End();

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
			ANIM_INDEX,

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

	// ImGuiのデバッグ描画の更新
	UpdateDebugImGui();

	// クールタイムを計算
	coolTime_ -= deltaTime;

	// どの行動をするか決める
	if (!isActionDecided_ && coolTime_ <= 0.0f)
	{
		SelsectAction(deltaTime);
	}

	// 状態ごとの更新
	stateUpdate_(deltaTime);

	// 重力がかかるアニメーションのみ処理する
	if (state_ != EnemyState::HIT_KNOCK_BACK)
	{
		// 重力
		Gravity(gravityScale_);
	}

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

	// どのヒットアニメーションかチェックする
	AttackHitCheck(state);

	// HPを減らす
	SubHp(damage);
	
	// HPが0になったら死ぬアニメーションに遷移
	if (hp_ <= 0)
	{
		DeathAnim(state);
	}

	// アニメーションの再生時間をリセットする
	animationController_->ResetStepAnim();

}

void Enemy::AttackHitCheck(const int state)
{

	// 上に飛ぶアニメーションかチェック
	for (const auto hitState : hitFlinchUpState_)
	{
		if (hitState == static_cast<PlayerState>(state))
		{
			ChangeState(EnemyState::HIT_FLINCH_UP);
			return;
		}
	}

	// 頭にヒットするアニメーションかチェック
	for (const auto hitState : hitHeadState_)
	{
		if (hitState == static_cast<PlayerState>(state))
		{
			ChangeState(EnemyState::HIT_HEAD);
			return;
		}
	}

	// 体にヒットするアニメーションかチェック
	for (const auto hitState : hitBodyState_)
	{
		if (hitState == static_cast<PlayerState>(state))
		{
			ChangeState(EnemyState::HIT_BODY);
			return;
		}
	}

	// 吹っ飛んでいくアニメーションかチェック
	for (const auto hitState : hitFlyState_)
	{
		if (hitState == static_cast<PlayerState>(state))
		{
			ChangeState(EnemyState::HIT_FLY);
			return;
		}
	}

	// 吹っ飛んでいくアニメーションかチェック
	for (const auto hitState : hitKnockBackState_)
	{
		if (hitState == static_cast<PlayerState>(state))
		{
			ChangeState(EnemyState::HIT_KNOCK_BACK);
			return;
		}
	}

}

void Enemy::DeathAnim(int state)
{

	// ノーマルの死亡アニメーションかチェック
	for (const auto hitState : deathState_)
	{
		if (hitState == static_cast<PlayerState>(state))
		{
			ChangeState(EnemyState::DEATH);
			return;
		}
	}

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
		return player->GetTransform().pos;
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

void Enemy::SelsectAction(const float deltaTime)
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
		Attack(deltaTime);
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
	if (length >= ACTIVATION_DISTANCE)
	{

		ChangeState(EnemyState::RUN);

		// 行動を決めた
		isActionDecided_ = true;

	}

}

void Enemy::Attack(const float deltaTime)
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

	// 相手の座標
	targetPos_ = playerPos.value();

	// 敵からプレイヤーへのベクトル
	VECTOR vec = VSub(targetPos_, transform_.pos);

	// ベクトルの長さ
	float length = Utility::Magnitude(vec);

	// プレイヤーとの距離が遠かったら攻撃できない
	if (length >= ACTIVATION_DISTANCE) return;

	if (number == 0)
	{

		ChangeState(EnemyState::ATTACK_PUNCH);

		// 行動を決めた
		isActionDecided_ = true;

	}
	else if (number == 1)
	{

		ChangeState(EnemyState::ATTACK_KICK);

		// 行動を決めた
		isActionDecided_ = true;

	}

}

void Enemy::ChangeIdle()
{

	stateUpdate_ = std::bind(&Enemy::UpdateIdle, this, std::placeholders::_1);

	// 右手の攻撃判定をなくす
	collisionData_.isRightHandAttack = false;

	// 左手の攻撃判定をなくす
	collisionData_.isLeftHandAttack = false;

	// 右足の攻撃判定をなくす
	collisionData_.isRightFootAttack = false;

	// 左足の攻撃判定をなくす
	collisionData_.isLeftFootAttack = false;

	// 攻撃が当たっているかをリセットする
	isAttackHit_ = false;

	// 重力を通常状態に戻す
 	gravityScale_ = 1.0f;

	// 角度が変更されたかどうかをリセットする
	isChangeAngle_ = false;

}

void Enemy::ChangeRun()
{

	stateUpdate_ = std::bind(&Enemy::UpdateRun, this, std::placeholders::_1);

	// 追いかけている時間をリセット
	trackingTime_ = 0.0f;

}

void Enemy::ChangePunch()
{

	stateUpdate_ = std::bind(&Enemy::UpdatePunch, this, std::placeholders::_1);

	// 攻撃が当たっているかをリセットする
	isAttackHit_ = false;

	// スピード
	speed_ = ATTACK_MOVE_POW;

	// どれだけ進むか計算
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, speed_));

}

void Enemy::ChangeKick()
{

	stateUpdate_ = std::bind(&Enemy::UpdateKick, this, std::placeholders::_1);

	// 攻撃が当たっているかをリセットする
	isAttackHit_ = false;

	// スピード
	speed_ = ATTACK_MOVE_POW;

	// どれだけ進むか計算
	movePow_ = VAdd(transform_.pos, VScale(moveDir_, speed_));

}

void Enemy::ChangeHitHead()
{

	stateUpdate_ = std::bind(&Enemy::UpdateHitHead, this, std::placeholders::_1);

	// プレイヤーの方向を求める
	VECTOR vec = VSub(targetPos_, transform_.pos);

	// 正規化
	vec = VNorm(vec);

	// プレイヤーの方向と逆方向のベクトル
	vec = { -vec.x, vec.y,-vec.z };

	// スピード
	speed_ = ATTACK_MOVE_POW;

	// 移動量
	movePow_ = VAdd(transform_.pos, VScale(vec, speed_));

}

void Enemy::ChangeHitBody()
{

	stateUpdate_ = std::bind(&Enemy::UpdateHitBody, this, std::placeholders::_1);

	// プレイヤーの方向を求める
	VECTOR vec = VSub(targetPos_, transform_.pos);

	// 正規化
	vec = VNorm(vec);

	// プレイヤーの方向と逆方向のベクトル
	vec = { -vec.x, vec.y,-vec.z };

	// スピード
	speed_ = ATTACK_MOVE_POW;

	// 移動量
	movePow_ = VAdd(transform_.pos, VScale(vec, speed_));

}

void Enemy::ChangeHitFly()
{

	stateUpdate_ = std::bind(&Enemy::UpdateHitFly, this, std::placeholders::_1);

	// プレイヤーの方向を求める
	VECTOR vec = VSub(targetPos_, transform_.pos);
	
	// 正規化
	vec = VNorm(vec);

	// プレイヤーの方向と逆方向のベクトル
	vec = { -vec.x, vec.y,-vec.z };

	// 一個前のアニメーションがまっすぐ飛んでいくのだったら上方向に飛ばさない
	if (key_ != ANIM_DATA_KEY[static_cast<int>(EnemyState::HIT_KNOCK_BACK)])
	{
		// 上方向に飛ばす
		velocity_.y = HIT_FLY_UP_VEC_POW;
		vec.y = velocity_.y;

		// スピード
		speed_ = HIT_FLY_MOVE_POW;

		// 移動量
		movePow_ = VScale(vec, speed_);
	}

}

void Enemy::ChangeHitFlinchUp()
{

	stateUpdate_ = std::bind(&Enemy::UpdateHitFlinchUp, this, std::placeholders::_1);

	// プレイヤーの方向を求める
	VECTOR vec = VSub(targetPos_, transform_.pos);

	// 正規化
	vec = VNorm(vec);

	// プレイヤーの方向と逆方向のベクトル
	vec = { -vec.x, vec.y,-vec.z };

	// 上方向に飛ばす
	velocity_.y = FLINCH_UP_UP_VEC_POW;
	vec.y = velocity_.y;

	// スピード
	speed_ = FLINCH_UP_SPEED;

	// 移動量
	movePow_ = VScale(vec, speed_);

	// すでに角度が変わっていたら処理しない
	if (!isChangeAngle_)
	{
		// 体の角度を変更
		transform_.quaRot = Quaternion::Mult(transform_.quaRot, Quaternion::AngleAxis(Utility::Deg2RadF(FLINCH_UP_ANGLE_X), Utility::AXIS_X));
		isChangeAngle_ = true;
	}

	// 重力を緩くする
	gravityScale_ = FLINCH_UP_GRAVITY_SCALE;

}

void Enemy::ChangeHitKnockBack()
{

	stateUpdate_ = std::bind(&Enemy::UpdateHitKnockBack, this, std::placeholders::_1);

	// プレイヤーの方向を求める
	VECTOR vec = VSub(targetPos_, transform_.pos);

	// 正規化
	vec = VNorm(vec);

	// プレイヤーの方向と逆方向のベクトル
	vec = { -vec.x, vec.y,-vec.z };

	// y方向を消す
	vec.y = 0.0f;

	// スピード
	speed_ = HIT_FLY_MOVE_POW;

	// 移動量
	movePow_ = VScale(vec, speed_);

	// 高さを調整する
	transform_.pos.y = transform_.pos.y + KNOCK_BACK_HEIGHT_OFFSET;

}

void Enemy::ChangeKipUp()
{
	stateUpdate_ = std::bind(&Enemy::UpdateKipUp, this, std::placeholders::_1);
}

void Enemy::ChangeDeath()
{
	stateUpdate_ = std::bind(&Enemy::UpdateDeath, this, std::placeholders::_1);
}

void Enemy::UpdateIdle(const float deltaTime)
{

	// プレイヤーの座標
	std::optional<VECTOR> playerPos = GetPlayerPos();
	targetPos_ = playerPos.value();

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

void Enemy::UpdateRun(const float deltaTime)
{

	// プレイヤーの座標
	std::optional<VECTOR> playerPos = GetPlayerPos();
	targetPos_ = playerPos.value();

	// プレイヤーの方向を求める
	VECTOR vec = VSub(targetPos_, transform_.pos);

	// ベクトルの長さ
	float length = Utility::Magnitude(vec);

	// 正規化
	vec = VNorm(vec);

	// 方向を角度に変換する
	float angle = atan2f(vec.x, vec.z);

	// プレイヤー方向に回転
	LazyRotation(angle);

	// スピード
	speed_ = RUN_MOVE_POW;

	// 移動量
	movePow_ = VScale(vec, speed_);

	// プレイヤー方向に移動
 	transform_.pos = VAdd(transform_.pos, movePow_);

	// プレイヤーの近くに来たら次の行動を決める
	if (length <= ACTIVATION_DISTANCE)
	{
		isActionDecided_ = false;
	}
	// プレイヤーに追いつけなかったら待機に遷移
	else if (trackingTime_ >= TRACKING_MAX_TIME)
	{

		ChangeState(EnemyState::IDLE);

		// クールタイムを設定
		coolTime_ = COOL_TIME;

	}

	trackingTime_ += deltaTime;
}

void Enemy::UpdatePunch(const float deltaTime)
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

void Enemy::UpdateKick(const float deltaTime)
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

void Enemy::UpdateHitHead(const float deltaTime)
{

	// 少し後ろにゆっくり移動
	transform_.pos = Utility::Lerp(transform_.pos, movePow_, MOVE_RATE);

	// アニメーションが終了したら待機状態へ遷移する
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(EnemyState::IDLE);
	}

}

void Enemy::UpdateHitBody(const float deltaTime)
{

	// 少し後ろにゆっくり移動
	transform_.pos = Utility::Lerp(transform_.pos, movePow_, MOVE_RATE);

	// アニメーションが終了したら待機状態へ遷移する
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(EnemyState::IDLE);
	}

}

void Enemy::UpdateHitFly(const float deltaTime)
{

	// 地面につくまで加算する
	if (velocity_.y != 0.0f)
	{
		// 後ろに飛んでいきながら移動
		transform_.pos = VAdd(transform_.pos, movePow_);
	}

	// アニメーションが終了したら起き上がり状態へ遷移する
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(EnemyState::KIP_UP);
	}

}

void Enemy::UpdateHitFlinchUp(const float deltaTime)
{

	// 地面につくまで加算する
	if (velocity_.y != 0.0f)
	{
		// 後ろに飛んでいきながら移動
		transform_.pos = VAdd(transform_.pos, movePow_);
	}

	// アニメーションが終了したら起き上がり状態へ遷移する
	if (transform_.pos.y <= FLOOR_HEIGHT && velocity_.y <= 0.0f)
	{
		ChangeState(EnemyState::KIP_UP);
	}

}

void Enemy::UpdateHitKnockBack(const float deltaTime)
{

	// 飛んでいられる時間まで移動し続ける
	if (KNOCK_BACK_TIME > knockBackCnt_)
	{
		// 後ろに飛んでいきながら移動
		transform_.pos = VAdd(transform_.pos, movePow_);
	}
	else
	{
		knockBackCnt_ = 0.0f;
		ChangeState(EnemyState::HIT_FLY);
	}

	// 飛んでいる時間をカウント
	knockBackCnt_ += deltaTime;

}

void Enemy::UpdateKipUp(const float deltaTime)
{

	// アニメーションが終了したら待機状態へ遷移する
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(EnemyState::IDLE);
	}

}

void Enemy::UpdateDeath(const float deltaTime)
{

	// アニメーションが終了したら待機状態へ遷移する
	if (animationController_->IsEndPlayAnimation())
	{
		isActive_ = false;
	}

}
