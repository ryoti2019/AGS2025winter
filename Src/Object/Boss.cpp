#include "../Lib/ImGui/imgui.h"
#include "../Application.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Scene/GameScene.h"
#include "Boss.h"

Boss::Boss(const VECTOR& pos, const json& data)
	:
	EnemyBase(pos, data),
	ATTACK_PROJECTILE_START_FRAME(data["ANIM"][static_cast<int>(BossState::ATTACK_PROJECTILE) - 1]["ATTACK_START_FRAME"]),
	ATTACK_PROJECTILE_DAMAGE(data["ANIM"][static_cast<int>(BossState::ATTACK_PROJECTILE) - 1]["DAMAGE"]),
	ATTACK_PROJECTILE_COLLISION_TIME(data["ATTACK_PROJECTILE_COLLISION_TIME"]),
	LONG_RANGE_ATTACK_DISTANCE(data["LONG_RANGE_ATTACK_DISTANCE"]),
	SUPER_ARMOR_HP(data["SUPER_ARMOR_HP"]),
	SUPER_ARMOR_HP_COOL_TIME(data["SUPER_ARMOR_HP_COOL_TIME"]),
	projectileDir_({0.0f,0.0f,0.0f}),
	projectileCollisionCnt_(0.0f)
{

	// 機能の初期化
	InitFunction();

	// モデルID
	modelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::MODEL_BOSS)]);

	// 関数ポインタの初期化
	InitFunctionPointer();

	// 共通部分は基底クラスで初期化
	ActorBase::Init(pos);

	// パラメータの初期化
	InitParameter();

	// 敵同士の共通部分の初期化
	EnemyBase::Init();

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
	modelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::MODEL_BOSS)]);

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
	collisionData_.projectileCollisionRadius = jsonData_["PROJECTILE_COLLISION_RADIUS"];

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

	// HPの最大値
	HP_MAX = jsonData_["HP"];

	// HP
	hp_ = HP_MAX;

	// スーパーアーマーHP
	superArmorHp_ = SUPER_ARMOR_HP;

	// スーパーアーマーが回復するまでのクールタイムのカウンタ
	superArmorCoolTimeCnt_ = SUPER_ARMOR_HP_COOL_TIME;

	// HPバーの長さ
	HP_BAR_LENGTH = jsonData_["HP_BAR_LENGTH"];

	// パンチのダメージ量
	ATTACK_PUNCH_DAMAGE = jsonData_["ANIM"][static_cast<int>(BossState::ATTACK_PUNCH) - 1]["DAMAGE"];

	// キックのダメージ量
	ATTACK_KICK_DAMAGE = jsonData_["ANIM"][static_cast<int>(BossState::ATTACK_KICK) - 1]["DAMAGE"];

	// 敵がまっすく飛んでいくときのカウンタ
	knockBackCnt_ = 0.0f;

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
			resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::MODEL_BOSS) + i]),

			// アニメーションのループ再生
			isLoop,

			// アニメーション番号
			ANIM_INDEX,

			// アニメーションの逆再生
			false
		);
	}

	// アニメーション再生するキー
	key_ = "";

	// 1個前のアニメーション
	preKey_ = "";

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
			resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::EFFECT_BOSS_PROJECTILE)]).handleId_,

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
	stateChange_.emplace(BossState::HIT_HEAD, std::bind(&Boss::ChangeHitHead, this));
	stateChange_.emplace(BossState::HIT_BODY, std::bind(&Boss::ChangeHitBody, this));
	stateChange_.emplace(BossState::HIT_FLY, std::bind(&Boss::ChangeHitFly, this));
	stateChange_.emplace(BossState::HIT_FLINCH_UP, std::bind(&Boss::ChangeHitFlinchUp, this));
	stateChange_.emplace(BossState::HIT_KNOCK_BACK, std::bind(&Boss::ChangeHitKnockback, this));
	stateChange_.emplace(BossState::KIP_UP, std::bind(&Boss::ChangeKipUp, this));
	stateChange_.emplace(BossState::DEATH, std::bind(&Boss::ChangeDeath, this));

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
	if (animationController_->IsBlendPlay("ATTACK_KICK") || animationController_->IsBlendPlay("HIT_FLY"))
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
		if (animationController_->IsBlendPlay("ATTACK_KICK"))
		{
			mix = MMult(mix, MGetTranslate({ 0.0f, pos.y, 0.0f }));
		}
		else if (animationController_->IsBlendPlay("HIT_FLY"))
		{
			mix = MMult(mix, MGetTranslate({ 0.0f, 0.0f, 0.0f }));
		}

		// 合成した行列を対象フレームにセットし直して、
		// アニメーションの移動値を無効化
		MV1SetFrameUserLocalMatrix(transform_->modelId, collisionData_.body, mix);

	}

}

void Boss::AttackHitCheck(const int state)
{

	// 上に飛ぶアニメーションかチェック
	for (const auto hitState : hitFlinchUpState_)
	{
		if (hitState == static_cast<PlayerState>(state))
		{
			ChangeState(BossState::HIT_FLINCH_UP);
			return;
		}
	}

	// 地面についていないかチェック
	if (!isOnGround_)
	{
		// 空中に浮き続けるアニメーションかチェック
		for (const auto hitState : hitAirState_)
		{
			if (hitState == static_cast<PlayerState>(state))
			{
				ChangeState(BossState::HIT_FLINCH_UP);
				return;
			}
		}
	}

	// 頭にヒットするアニメーションかチェック
	for (const auto hitState : hitHeadState_)
	{
		if (hitState == static_cast<PlayerState>(state))
		{
			ChangeState(BossState::HIT_HEAD);
			return;
		}
	}

	// 体にヒットするアニメーションかチェック
	for (const auto hitState : hitBodyState_)
	{
		if (hitState == static_cast<PlayerState>(state))
		{
			ChangeState(BossState::HIT_BODY);
			return;
		}
	}

	// 吹っ飛んでいくアニメーションかチェック
	for (const auto hitState : hitFlyState_)
	{
		if (hitState == static_cast<PlayerState>(state))
		{
			ChangeState(BossState::HIT_FLY);
			return;
		}
	}

	// 吹っ飛んでいくアニメーションかチェック
	for (const auto hitState : hitKnockBackState_)
	{
		if (hitState == static_cast<PlayerState>(state))
		{
			ChangeState(BossState::HIT_KNOCK_BACK);
			return;
		}
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

	// 飛び道具の更新
	Projectile(deltaTime);

	// スーパーアーマーのHPを回復するまでのクールタイムを更新
	ResetSuperArmorCoolTime(deltaTime);

	// 状態ごとの更新
	// 重力がかかる前に処理しないとおかしな挙動になるので注意！
	stateUpdate_(deltaTime);

	// 重力がかかるアニメーションのみ処理する
	// 重力
	if (state_ != BossState::HIT_KNOCK_BACK)
	{
		Gravity(gravityScale_);
	}

	// モデル情報を更新
	transform_->Update();

	// アニメーション再生
	animationController_->Update(deltaTime);

	// アニメーションのフレームを固定
	AnimationFrame();

}

void Boss::Draw(const float deltaTime)
{

	ActorBase::Draw(deltaTime);

	// HPバーの長さ
	const int HP_LENGTH = 400;

	// HPバーの半分の長さ
	const int HP_LENGTH_HARF = HP_LENGTH / 2;

	// HPバーの高さ
	const int HP_HEIGHT = 10;

	// HPバーの幅
	const int HP_BAR_WIDTH = 25;

	// RGBのスケール調整値
	const float RGB_SCALE = 512.0f;

	// RとBの変化を制御するためのバランス点
	const int COLOR_BALANCE_POINT = 384;

	// 緑色の変化を調整するオフセット
	const int GREEN_COLOR_SHIFT_OFFSET = 64;

	// HPの割合を元にバーの色を決める処理
	int H = hp_ * (RGB_SCALE / HP_MAX) - 100;

	// 赤成分
	int R = min(max((COLOR_BALANCE_POINT - H), 0), 0xff);

	//緑成分
	int G = min(max((H + GREEN_COLOR_SHIFT_OFFSET), 0), 0xff);

	// 青成分
	int B = max((H - COLOR_BALANCE_POINT), 0);

	// HPゲージ
	int hpGauge = HP_LENGTH * hp_ / HP_MAX;

	// HPゲージ背景を描画
	DrawBox(
		Application::SCREEN_SIZE_X / 2 - HP_LENGTH_HARF,
		HP_HEIGHT,
		Application::SCREEN_SIZE_X / 2 + HP_LENGTH_HARF,
		HP_HEIGHT + HP_BAR_WIDTH,
		0x000000, true);

	// HPゲージを描画
	DrawBox(
		Application::SCREEN_SIZE_X / 2 - HP_LENGTH_HARF,
		HP_HEIGHT,
		Application::SCREEN_SIZE_X / 2 - HP_LENGTH_HARF + hpGauge,
		HP_HEIGHT + HP_BAR_WIDTH,
		GetColor(R, G, B), true);

	// HPバーの高さ
	const int SUPER_AMOR_HP_HEIGHT = 50;

	// スーパーアーマーHPバーの幅
	const int SUPER_ARMOR_HP_BAR_WIDTH = 10;

	// スーパーアーマーHPの割合を元にバーの色を決める処理
	H = superArmorHp_ * (RGB_SCALE / SUPER_ARMOR_HP) - 100;

	// 赤成分
	R = min(max((COLOR_BALANCE_POINT - H), 0), 0xff);

	//緑成分
	G = min(max((H + GREEN_COLOR_SHIFT_OFFSET), 0), 0xff);

	// 青成分
	B = max((H - COLOR_BALANCE_POINT), 0);

	// HPゲージ
	int superArmorHpGauge = HP_LENGTH * superArmorHp_ / SUPER_ARMOR_HP;

	// HPゲージ背景を描画
	DrawBox(
		Application::SCREEN_SIZE_X / 2 - HP_LENGTH_HARF,
		SUPER_AMOR_HP_HEIGHT,
		Application::SCREEN_SIZE_X / 2 + HP_LENGTH_HARF,
		SUPER_AMOR_HP_HEIGHT + SUPER_ARMOR_HP_BAR_WIDTH,
		0x000000, true);

	// HPゲージを描画
	DrawBox(
		Application::SCREEN_SIZE_X / 2 - HP_LENGTH_HARF,
		SUPER_AMOR_HP_HEIGHT,
		Application::SCREEN_SIZE_X / 2 - HP_LENGTH_HARF + superArmorHpGauge,
		SUPER_AMOR_HP_HEIGHT + SUPER_ARMOR_HP_BAR_WIDTH,
		0xffffff, true);

}

void Boss::Projectile(const float deltaTime)
{

	// 攻撃判定があるフレーム
	if (collisionData_.isProjectileAttack && ATTACK_PROJECTILE_COLLISION_TIME >= projectileCollisionCnt_)
	{

		// 飛び道具の当たり判定の座標設定
		collisionData_.projectilePos = VAdd(collisionData_.projectilePos, VScale(projectileDir_,100.0f));

		// 飛び道具の衝突判定が続く時間のカウンタを加算
		projectileCollisionCnt_ += deltaTime;

		// エフェクトを追従させる
		effekseerController_->FollowPos(collisionData_.projectilePos, Quaternion::Identity(), { 0.0f,500.0f,0.0f }, "PROJECTILE");

	}
	else if (!collisionData_.isProjectileAttack || ATTACK_PROJECTILE_COLLISION_TIME <= projectileCollisionCnt_)
	{

		// 飛び道具の当たり判定をなくす
		collisionData_.isProjectileAttack = false;

		// 必殺技の衝突判定が続く時間のカウンタをリセット
		projectileCollisionCnt_ = 0.0f;

		// エフェクトを止める
		effekseerController_->DrawStop("PROJECTILE");

	}

}

void Boss::SubSuperArmorHp(const int superArmorHp)
{
	superArmorHp_ -= superArmorHp;
}

void Boss::ResetSuperArmorCoolTime(const float deltaTime)
{

	// スーパーアーマーHPが0以下か判定
	if (superArmorHp_ <= 0)
	{
		superArmorCoolTimeCnt_ -= deltaTime;
	}

	if (superArmorCoolTimeCnt_ <= 0)
	{
		superArmorHp_ = SUPER_ARMOR_HP;
	}

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

const bool Boss::GetCloseRangeAttackState() const
{

	// 近接攻撃の状態か判定
	for (const auto state : closeRangeAttackState_)
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

const bool Boss::GetHitState() const
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

const bool Boss::GetIsSuperArmor() const
{
	return superArmorHp_ > 0;
}

void Boss::AttackHit(const int damage, const int state)
{

	// スーパーアーマー状態か判定
	if (GetIsSuperArmor())
	{
		// スーパーアーマーの耐久値を減らす
		SubSuperArmorHp(damage);
		return;
	}

	// どのヒットアニメーションかチェックする
	AttackHitCheck(state);

	// HPを減らす
	SubHp(damage);

	if (hp_ <= 0)
	{
		// 死ぬアニメーションに遷移
		DeathAnim(state);
	}

	// アニメーションの再生時間をリセットする
	animationController_->ResetStepAnim();

}

void Boss::ProjectileHit(const int damage)
{

	// スーパーアーマー状態か判定
	if (GetIsSuperArmor())
	{
		// スーパーアーマーの耐久値を減らす
		SubSuperArmorHp(damage);
		return;
	}

	// ヒットアニメーションに遷移
	ChangeState(BossState::HIT_FLY);

	// HPを減らす
	SubHp(damage);

	// アニメーションの再生時間をリセットする
	animationController_->ResetStepAnim();

}

void Boss::ChangeIdle()
{

	stateUpdate_ = std::bind(&Boss::UpdateIdle, this, std::placeholders::_1);

	// 重力を通常状態に戻す
	gravityScale_ = 1.0f;

	// 角度が変更されたかどうかをリセットする
	isChangeAngle_ = false;

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

	// 右手の攻撃判定をなくす
	collisionData_.isRightHandAttack = false;

	// ダメージ量
	damage_ = ATTACK_PUNCH_DAMAGE;

	// スピード
	speed_ = ATTACK_MOVE_POW;

}

void Boss::ChangeKick()
{

	stateUpdate_ = std::bind(&Boss::UpdateKick, this, std::placeholders::_1);

	// 右足の攻撃判定をなくす
	collisionData_.isRightFootAttack = false;

	// ダメージ量
	damage_ = ATTACK_KICK_DAMAGE;

	// スピード
	speed_ = ATTACK_MOVE_POW;

}

void Boss::ChangeProjectile()
{

	stateUpdate_ = std::bind(&Boss::UpdateProjectile, this, std::placeholders::_1);

	// 飛び道具の攻撃判定をなくす
	collisionData_.isProjectileAttack = false;

	// ダメージ量
	damage_ = ATTACK_PROJECTILE_DAMAGE;

	// 必殺技の当たり判定の座標を設定
	collisionData_.projectilePos = VAdd(transform_->pos, BODY_RELATIVE_CENTER_POS);

	// 飛び道具の飛んでいく方向
	projectileDir_ = VNorm(transform_->quaRot.GetForward());

}

void Boss::ChangeHitHead()
{

	stateUpdate_ = std::bind(&Boss::UpdateHitHead, this, std::placeholders::_1);

	// プレイヤーの方向を求める
	VECTOR vec = VSub(targetPos_, transform_->pos);

	// プレイヤーの方向と逆方向のベクトル
	moveDir_ = { -vec.x, vec.y,-vec.z };

	// スピード
	speed_ = ATTACK_MOVE_POW;

}

void Boss::ChangeHitBody()
{

	stateUpdate_ = std::bind(&Boss::UpdateHitBody, this, std::placeholders::_1);

	// プレイヤーの方向を求める
	VECTOR vec = VSub(targetPos_, transform_->pos);

	// プレイヤーの方向と逆方向のベクトル
	moveDir_ = { -vec.x, vec.y,-vec.z };

	// スピード
	speed_ = ATTACK_MOVE_POW;

}

void Boss::ChangeHitFly()
{

	stateUpdate_ = std::bind(&Boss::UpdateHitFly, this, std::placeholders::_1);

	// プレイヤーの方向を求める
	VECTOR vec = VSub(targetPos_, transform_->pos);

	// 正規化
	vec = VNorm(vec);

	// プレイヤーの方向と逆方向のベクトル
	moveDir_ = { -vec.x, vec.y,-vec.z };

	// 一個前のアニメーションがまっすぐ飛んでいくのだったら上方向に飛ばさない
	if (key_ != ANIM_DATA_KEY[static_cast<int>(BossState::HIT_KNOCK_BACK)])
	{

		// 上方向に飛ばす
		velocity_.y = HIT_FLY_UP_VEC_POW;
		moveDir_.y = velocity_.y;

		// スピード
		speed_ = HIT_FLY_MOVE_POW;

	}

	// 体の角度をリセット
	transform_->quaRot.x = Quaternion().x;
	transform_->quaRot.z = Quaternion().z;

	// 重力を通常状態に戻す
	gravityScale_ = 1.0f;

}

void Boss::ChangeHitFlinchUp()
{

	stateUpdate_ = std::bind(&Boss::UpdateHitFlinchUp, this, std::placeholders::_1);

	// プレイヤーの方向を求める
	VECTOR vec = VSub(targetPos_, transform_->pos);

	// 正規化
	vec = VNorm(vec);

	// プレイヤーの方向と逆方向のベクトル
	vec = { -vec.x, vec.y,-vec.z };

	// 地面についていたら上に移動させる
	if (isOnGround_)
	{
		velocity_.y = FLINCH_UP_UP_VEC_POW;
	}
	// 地面についていなかったら少し移動させる
	else
	{
		velocity_.y = FLINCH_UP_UP_VEC_SMALL_POW;
	}

	// 上方向に飛ばす
	vec.y = velocity_.y;

	// 実際に動く方向
	moveDir_ = vec;

	// スピード
	speed_ = FLINCH_UP_SPEED;

	// すでに角度が変わっていたら処理しない
	if (!isChangeAngle_)
	{
		// 体の角度を変更
		transform_->quaRot = Quaternion::Mult(transform_->quaRot, Quaternion::AngleAxis(Utility::Deg2RadF(FLINCH_UP_ANGLE_X), Utility::AXIS_X));
		isChangeAngle_ = true;
	}

	// 重力を緩くする
	gravityScale_ = FLINCH_UP_GRAVITY_SCALE;

}

void Boss::ChangeHitKnockback()
{

	stateUpdate_ = std::bind(&Boss::UpdateHitKnockback, this, std::placeholders::_1);

	// プレイヤーの方向を求める
	VECTOR vec = VSub(targetPos_, transform_->pos);

	// プレイヤーの方向と逆方向のベクトル
	moveDir_ = { -vec.x, vec.y,-vec.z };

	// y方向を消す
	moveDir_.y = 0.0f;

	// スピード
	speed_ = HIT_FLY_MOVE_POW;

	// 高さを調整する
	transform_->pos.y = transform_->pos.y + KNOCK_BACK_HEIGHT_OFFSET;

	// 衝突判定の更新
	ActorBase::CollisionUpdate();

}

void Boss::ChangeKipUp()
{

	stateUpdate_ = std::bind(&Boss::UpdateHitKipUp, this, std::placeholders::_1);

	// 体の角度をリセット
	transform_->quaRot.x = Quaternion().x;
	transform_->quaRot.z = Quaternion().z;

}

void Boss::ChangeDeath()
{

	stateUpdate_ = std::bind(&Boss::UpdateDeath, this, std::placeholders::_1);

	// 体の角度をリセット
	transform_->quaRot.x = Quaternion().x;
	transform_->quaRot.z = Quaternion().z;

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

	// すでに描画しているかとアニメーションが始まっているか判定
	if (!effekseerController_->IsDraw("PROJECTILE") && ATTACK_PROJECTILE_START_FRAME <= animationController_->GetStepAnim())
	{

		// 当たり判定をつける
		collisionData_.isProjectileAttack = true;

		// エフェクトを描画
		effekseerController_->Draw(collisionData_.projectilePos, Quaternion::Identity(), { 0.0f,500.0f,0.0f }, "PROJECTILE");

	}

	// 待機状態に遷移
	if (animationController_->IsEndPlayAnimation())
	{

		ChangeState(BossState::IDLE);

		// クールタイムを設定
		coolTime_ = COOL_TIME;

	}

}

void Boss::UpdateHitHead(const float deltaTime)
{

	// 少し後ろにゆっくり移動
	moveComponent_->Lerp();

	// アニメーションが終了したら待機状態へ遷移する
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(BossState::IDLE);
	}

}

void Boss::UpdateHitBody(const float deltaTime)
{

	// 少し後ろにゆっくり移動
	moveComponent_->Lerp();

	// アニメーションが終了したら待機状態へ遷移する
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(BossState::IDLE);
	}

}

void Boss::UpdateHitFly(const float deltaTime)
{

	// 地面につくまで加算する
	if (!isOnGround_)
	{
		// 後ろに飛んでいきながら移動
		moveComponent_->HitMove();
	}

	// HPが0以下だったら非アクティブにする
	if (animationController_->IsEndPlayAnimation() && hp_ <= 0)
	{
		isActive_ = false;
	}

	// HPが0以下は通らない
	if (hp_ <= 0)return;

	// アニメーションが終了したら起き上がり状態へ遷移する
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(BossState::KIP_UP);
	}

}

void Boss::UpdateHitFlinchUp(const float deltaTime)
{

	// 地面につくまで加算する
	if (!isOnGround_)
	{
		// 上に緩く移動する
		moveComponent_->HitMove();
	}

	// アニメーションが終了したら起き上がり状態へ遷移する
	if (isOnGround_)
	{
		ChangeState(BossState::KIP_UP);
	}

}

void Boss::UpdateHitKnockback(const float deltaTime)
{
	
	// 飛んでいられる時間まで移動し続ける
	if (KNOCK_BACK_TIME > knockBackCnt_)
	{
		// 後ろに飛んでいきながら移動
		moveComponent_->Move();
	}
	else
	{
		knockBackCnt_ = 0.0f;
		ChangeState(BossState::HIT_FLY);
	}

	// 飛んでいる時間をカウント
	knockBackCnt_ += deltaTime;

}

void Boss::UpdateHitKipUp(const float deltaTime)
{

	// アニメーションが終了したら待機状態へ遷移する
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(BossState::IDLE);
	}

}

void Boss::UpdateDeath(const float deltaTime)
{

	// アニメーションが終了したら非アクティブにする
	if (animationController_->IsEndPlayAnimation())
	{
		isActive_ = false;
	}

}
