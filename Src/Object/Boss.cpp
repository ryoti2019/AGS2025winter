#include "../Lib/ImGui/imgui.h"
#include "Boss.h"

Boss::Boss(const VECTOR& pos, const json& data)
	:
	EnemyBase(pos, data),
	ATTACK_PROJECTILE_START_FRAME(data["ANIM"][static_cast<int>(BossState::ATTACK_PROJECTILE) - 1]["ATTACK_START_FRAME"]),
	ATTACK_PROJECTILE_DAMAGE(data["ANIM"][static_cast<int>(BossState::ATTACK_PROJECTILE) - 1]["DAMAGE"]),
	ATTACK_PROJECTILE_COLLISION_TIME(data["ATTACK_PROJECTILE_COLLISION_TIME"])
{

	// 機能の初期化
	InitFunction();

	// モデルID
	modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_BOSS);

	// 関数ポインタの初期化
	InitFunctionPointer();

	// 共通部分は基底クラスで初期化
	ActorBase::Init(pos);

	// パラメータの初期化
	InitParameter();

	// 共通部分は基底クラスで初期化
	ActorBase::Init(pos);

	// アニメーションの初期化
	InitAnimation();

	// エフェクトの初期化
	InitEffect();

}

void Boss::Init(const VECTOR& pos)
{

	// 機能の初期化
	InitFunction();

	// モデルID
	modelId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_BOSS);

	// 共通部分は基底クラスで初期化
	ActorBase::Init(pos);

	// 関数ポインタの初期化
	InitFunctionPointer();

	// 敵同士の共通部分の初期化
	EnemyBase::Init();

	// パラメータの初期化
	InitParameter();

	// アニメーションの初期化
	InitAnimation();

	//エフェクトの初期化
	InitEffect();

	// AIコンポーネントを追加
	aiComponent_ = std::make_unique<BossAIComponent>(std::static_pointer_cast<Boss>(GetThis()));

	// 移動用のコンポーネントを追加
	moveComponent_ = std::make_unique<MoveComponent>(std::static_pointer_cast<Boss>(GetThis()));

	// 描画用のコンポーネント
	drawComponent_ = std::make_unique<DrawComponent>(std::static_pointer_cast<Boss>(GetThis()));

}

void Boss::InitFunction()
{
}

void Boss::InitParameter()
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
	collisionData_.rightHand = MV1SearchFrame(transform_->modelId, RIGHT_HAND_FRAME.c_str());

	// 左手のフレーム番号を取得
	collisionData_.leftHand = MV1SearchFrame(transform_->modelId, LEFT_HAND_FRAME.c_str());

	// 右足のフレーム番号を取得
	collisionData_.rightFoot = MV1SearchFrame(transform_->modelId, RIGHT_FOOT_FRAME.c_str());

	// 左足のフレーム番号を取得
	collisionData_.leftFoot = MV1SearchFrame(transform_->modelId, LEFT_FOOT_FRAME.c_str());

	// 体のフレーム番号を取得
	collisionData_.body = MV1SearchFrame(transform_->modelId, BODY_FRAME.c_str());

	// 飛び道具の当たり判定の座標を設定
	collisionData_.projectilePos = VAdd(transform_->pos, BODY_RELATIVE_CENTER_POS);

	// 手足の衝突判定の半径
	collisionData_.handAndFootCollisionRadius = HAND_AND_FOOT_COLLISION_RADIUS;

	// 体の衝突判定の半径
	collisionData_.bodyCollisionRadius = BODY_COLLISION_RADIUS;

	// 飛び道具の衝突判定の半径
	collisionData_.projectileCollisionRadius = SPECIAL_ATTACK_COLLISION_RADIUS;

	// 右手に攻撃判定があるかどうか
	collisionData_.isRightHandAttack = false;

	// 左手に攻撃判定があるかどうか
	collisionData_.isLeftHandAttack = false;

	// 右足に攻撃判定があるかどうか
	collisionData_.isRightFootAttack = false;

	// 左足に攻撃判定があるかどうか
	collisionData_.isLeftFootAttack = false;

	// 飛び道具に攻撃判定があるかどうか
	collisionData_.isProjectileAttack = false;

	// プレイヤーの座標
	std::optional<VECTOR> playerPos = GetPlayerPos();

	// 相手の座標
	targetPos_ = playerPos.value();

	// パンチの攻撃開始フレーム
	PUNCH_ATTACK_START_FRAME = jsonData_["ANIM"][static_cast<int>(BossState::ATTACK_PUNCH) - 1]["ATTACK_START_FRAME"];

	// パンチの攻撃終了フレーム
	PUNCH_ATTACK_END_FRAME = jsonData_["ANIM"][static_cast<int>(BossState::ATTACK_PUNCH) - 1]["ATTACK_END_FRAME"];

	// キックの攻撃開始フレーム
	KICK_ATTACK_START_FRAME = jsonData_["ANIM"][static_cast<int>(BossState::ATTACK_KICK) - 1]["ATTACK_START_FRAME"];

	// キックの攻撃終了フレーム
	KICK_ATTACK_END_FRAME = jsonData_["ANIM"][static_cast<int>(BossState::ATTACK_KICK) - 1]["ATTACK_END_FRAME"];

	// 走るときの移動量
	RUN_MOVE_POW = jsonData_["RUN_MOVE_POW"];

	// アニメーション番号
	ANIM_INDEX = jsonData_["ANIM_INDEX"];

}

void Boss::InitAnimation()
{

	// アニメーションコントローラーの生成
	animationController_ = std::make_unique<AnimationController>(transform_->modelId);

	// アニメーションコントローラーにアニメーションを追加
	for (int i = static_cast<int>(BossState::IDLE); i < static_cast<int>(BossState::MAX); ++i)
	{

		// ループ再生するアニメーションだけisLoopをtrueにする
		bool isLoop = i == static_cast<int>(BossState::IDLE) || i == static_cast<int>(BossState::RUN);
		animationController_->Add(

			// アニメーションの名前
			jsonData_["ANIM"][i - 1]["NAME"],

			// アニメーションが始まる時間
			0.0f,

			// アニメーションスピード
			jsonData_["ANIM"][i - 1]["SPEED"],

			// アニメーションハンドル
			resMng_.LoadModelDuplicate(static_cast<ResourceManager::SRC>(static_cast<int>(ResourceManager::SRC::MODEL_BOSS) + i)),

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
	ChangeState(BossState::IDLE);

}

void Boss::InitEffect(void)
{

	// エフェクトコントローラーの生成
	effekseerController_ = std::make_unique<EffekseerController>();

	// エフェクトコントローラーにアニメーションを追加
	for (int i = static_cast<int>(EffectData::PROJECTILE); i < static_cast<int>(EffectData::MAX); ++i)
	{

		effekseerController_->Add(

			// エフェクトの名前
			jsonData_["EFFECT"][i - 1]["NAME"],

			// エフェクトハンドル
			resMng_.Load(static_cast<ResourceManager::SRC>(static_cast<int>(ResourceManager::SRC::EFFECT_BOSS_PROJECTILE))).handleId_,

			// エフェクトのスケール
			jsonData_["EFFECT"][i - 1]["SCALE"]);

	}

}

void Boss::InitFunctionPointer()
{

	//関数ポインタの初期化
	stateChange_.emplace(BossState::IDLE, std::bind(&Boss::ChangeIdle, this));
	stateChange_.emplace(BossState::RUN, std::bind(&Boss::ChangeRun, this));
	stateChange_.emplace(BossState::ATTACK_PUNCH, std::bind(&Boss::ChangePunch, this));
	stateChange_.emplace(BossState::ATTACK_KICK, std::bind(&Boss::ChangeKick, this));
	stateChange_.emplace(BossState::ATTACK_PROJECTILE, std::bind(&Boss::ChangeProjectile, this));

}

void Boss::UpdateDebugImGui()
{

	// ウィンドウタイトル&開始処理
	ImGui::Begin("Boss");

	// 大きさ
	ImGui::Text("scale");
	ImGui::InputFloat("Scl", &scl_);

	// 角度
	VECTOR rotDeg = VECTOR();
	rotDeg.x = Utility::Rad2DegF(transform_->quaRot.x);
	rotDeg.y = Utility::Rad2DegF(transform_->quaRot.y);
	rotDeg.z = Utility::Rad2DegF(transform_->quaRot.z);
	ImGui::Text("angle(deg)");
	ImGui::SliderFloat("RotX", &rotDeg.x, -90.0f, 90.0f);
	ImGui::SliderFloat("RotY", &rotDeg.y, -90.0f, 90.0f);
	ImGui::SliderFloat("RotZ", &rotDeg.z, -90.0f, 90.0f);
	transform_->quaRot.x = Utility::Deg2RadF(rotDeg.x);
	transform_->quaRot.y = Utility::Deg2RadF(rotDeg.y);
	transform_->quaRot.z = Utility::Deg2RadF(rotDeg.z);

	// 位置
	ImGui::Text("position");
	// 構造体の先頭ポインタを渡し、xyzと連続したメモリ配置へアクセス
	ImGui::InputFloat3("Pos", &transform_->pos.x);
	// 終了処理
	ImGui::End();

}

void Boss::AnimationFrame()
{

	// 対象フレームのローカル行列を初期値にリセットする
	MV1ResetFrameUserLocalMatrix(transform_->modelId, collisionData_.body);

	// 座標を固定する
	if (animationController_->IsBlendPlay("ATTACK_KICK"))
	{

		// 対象フレームのローカル行列(大きさ、回転、位置)を取得する
		auto mat = MV1GetFrameLocalMatrix(transform_->modelId, collisionData_.body);

		auto scl = MGetSize(mat); // 行列から大きさを取り出す
		auto rot = MGetRotElem(mat); // 行列から回転を取り出す
		auto pos = MGetTranslateElem(mat); // 行列から移動値を取り出す

		// 大きさ、回転、位置をローカル行列に戻す
		MATRIX mix = MGetIdent();
		mix = MMult(mix, MGetScale(scl)); // 大きさ
		mix = MMult(mix, rot); // 回転

		// ここでローカル座標を行列に、そのまま戻さず、
		// 調整したローカル座標を設定する
		mix = MMult(mix, MGetTranslate({ 0.0f, pos.y, 0.0f }));

		// 合成した行列を対象フレームにセットし直して、
		// アニメーションの移動値を無効化
		MV1SetFrameUserLocalMatrix(transform_->modelId, collisionData_.body, mix);

	}

}

void Boss::Update(const float deltaTime)
{

	// ImGuiのデバッグ描画の更新
	UpdateDebugImGui();

	// クールタイムを計算
	coolTime_ -= deltaTime;

	// 攻撃や移動を更新
	aiComponent_->Update(deltaTime);

	// 衝突判定の更新
	ActorBase::CollisionUpdate();

	// 状態ごとの更新
	// 重力がかかる前に処理しないとおかしな挙動になるので注意！
	stateUpdate_(deltaTime);

	//// 重力がかかるアニメーションのみ処理する
	//if (state_ != BossState::HIT_KNOCK_BACK && transform_->pos.y > FLOOR_HEIGHT)
	//{
		// 重力
		Gravity(gravityScale_);
	//}

	// モデル情報を更新
	transform_->Update();

	// アニメーション再生
	animationController_->Update(deltaTime);

	// アニメーションのフレームを固定
	AnimationFrame();

}

void Boss::ChangeState(const BossState state)
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

const bool Boss::GetAttackState() const
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

const std::vector<int> Boss::GetTotalAttackTypes() const
{

	std::vector<int> intStates;
	intStates.reserve(attackState_.size());

	// 変換処理
	for (const auto& state : attackState_)
	{
		intStates.push_back(static_cast<int>(state));
	}

	return intStates;

}

void Boss::ChangeIdle()
{

	stateUpdate_ = std::bind(&Boss::UpdateIdle, this, std::placeholders::_1);

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

}

void Boss::ChangeRun()
{

	stateUpdate_ = std::bind(&Boss::UpdateRun, this, std::placeholders::_1);

	// 追いかけている時間をリセット
	trackingTime_ = 0.0f;

}

void Boss::ChangePunch()
{

	stateUpdate_ = std::bind(&Boss::UpdatePunch, this, std::placeholders::_1);

	// 攻撃が当たっているかをリセットする
	isAttackHit_ = false;

	// スピード
	speed_ = ATTACK_MOVE_POW;

}

void Boss::ChangeKick()
{

	stateUpdate_ = std::bind(&Boss::UpdateKick, this, std::placeholders::_1);

	// 攻撃が当たっているかをリセットする
	isAttackHit_ = false;

	// スピード
	speed_ = ATTACK_MOVE_POW;

}

void Boss::ChangeProjectile()
{
	stateUpdate_ = std::bind(&Boss::UpdateProjectile, this, std::placeholders::_1);

	// 攻撃が当たっているかをリセットする
	isAttackHit_ = false;

	// ダメージ量
	damage_ = ATTACK_PROJECTILE_DAMAGE;

	// 必殺技の当たり判定の座標を設定
	collisionData_.projectilePos = VAdd(transform_->pos, BODY_RELATIVE_CENTER_POS);

	// 必殺技の衝突判定が続く時間のカウンタをリセット
	attackProjectileCollisionCnt_ = 0.0f;

	// 必殺技の当たり判定をリセット
	collisionData_.isProjectileAttack = false;

	// エフェクトを描画
	effekseerController_->Draw(collisionData_.projectilePos, Quaternion::Identity(), { 0.0f,0.0f,0.0f }, "PROJECTILE");

}

void Boss::UpdateIdle(const float deltaTime)
{

	// プレイヤーの座標
	std::optional<VECTOR> playerPos = GetPlayerPos();
	targetPos_ = playerPos.value();

	// プレイヤーの方向を求める
	VECTOR vec = VSub(targetPos_, transform_->pos);

	// 正規化
	vec = VNorm(vec);

	// 方向を角度に変換する
	float angle = atan2f(vec.x, vec.z);

	// プレイヤー方向に回転
	LazyRotation(angle);

	// 次の行動を考える
	isActionDecided_ = false;

}

void Boss::UpdateRun(const float deltaTime)
{

	// プレイヤーの座標
	std::optional<VECTOR> playerPos = GetPlayerPos();
	targetPos_ = playerPos.value();

	// プレイヤーの方向を求める
	moveDir_ = VSub(targetPos_, transform_->pos);

	// ベクトルの長さ
	float length = Utility::Magnitude(moveDir_);

	// スピード
	speed_ = RUN_MOVE_POW;

	// 移動処理
	moveComponent_->Move();

	// 方向を角度に変換する
	float angle = atan2f(moveDir_.x, moveDir_.z);

	// プレイヤー方向に回転
	LazyRotation(angle);

	// プレイヤーの近くに来たら次の行動を決める
	if (length <= ACTIVATION_DISTANCE)
	{
		isActionDecided_ = false;
	}
	// プレイヤーに追いつけなかったら待機に遷移
	else if (trackingTime_ >= TRACKING_MAX_TIME)
	{

		ChangeState(BossState::IDLE);

		// クールタイムを設定
		coolTime_ = COOL_TIME;

	}

	trackingTime_ += deltaTime;

}

void Boss::UpdatePunch(const float deltaTime)
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

		ChangeState(BossState::IDLE);

		// クールタイムを設定
		coolTime_ = COOL_TIME;

	}

}

void Boss::UpdateKick(const float deltaTime)
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

		ChangeState(BossState::IDLE);

		// クールタイムを設定
		coolTime_ = COOL_TIME;

	}

}

void Boss::UpdateProjectile(const float deltaTime)
{

	// 攻撃判定があるフレーム
	if (ATTACK_PROJECTILE_START_FRAME <= animationController_->GetStepAnim() && ATTACK_PROJECTILE_COLLISION_TIME >= attackProjectileCollisionCnt_)
	{

		// 当たり判定をONにする
		collisionData_.isProjectileAttack = true;

		// 飛び道具の当たり判定の座標設定
		collisionData_.projectilePos = VAdd(collisionData_.projectilePos, VScale(transform_->quaRot.GetForward(), 100.0f));

		// 飛び道具の衝突判定が続く時間のカウンタを加算
		attackProjectileCollisionCnt_ += deltaTime;

		// エフェクトを追従させる
		effekseerController_->FollowPos(collisionData_.projectilePos, Quaternion::Identity(), { 0.0f,0.0f,0.0f }, "PROJECTILE");

	}

	// 待機状態に遷移
	if (animationController_->IsEndPlayAnimation())
	{

		ChangeState(BossState::IDLE);

		// クールタイムを設定
		coolTime_ = COOL_TIME;

	}

}
