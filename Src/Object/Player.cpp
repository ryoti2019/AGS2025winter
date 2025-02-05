#include <memory>
#include "../Lib/ImGui/imgui.h"
#include "../Utility/Utility.h"
#include "../Scene/GameScene.h"
#include "../Manager/SceneManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/Camera.h"
#include "../Component/InputComponent.h"
#include "../Component/MoveComponent.h"
#include "../Object/Common/InputController.h"
#include "../Component/InputComponent.h"
#include "../Manager/ActorManager.h"
#include "Player.h"

Player::Player(const VECTOR& pos, const json& data)
	:
	ActorBase(pos, data),
	ATTACK_JAB_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_JAB) - 1]["ATTACK_START_FRAME"]),
	ATTACK_JAB_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_JAB) - 1]["ATTACK_END_FRAME"]),
	ATTACK_JAB_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_JAB) - 1]["DAMAGE"]),
	ATTACK_STRAIGHT_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_STRAIGHT) - 1]["ATTACK_START_FRAME"]),
	ATTACK_STRAIGHT_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_STRAIGHT) - 1]["ATTACK_END_FRAME"]),
	ATTACK_STRAIGHT_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_STRAIGHT) - 1]["DAMAGE"]),
	ATTACK_HOOK_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_HOOK) - 1]["ATTACK_START_FRAME"]),
	ATTACK_HOOK_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_HOOK) - 1]["ATTACK_END_FRAME"]),
	ATTACK_HOOK_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_HOOK) - 1]["DAMAGE"]),
	ATTACK_LEFT_KICK_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_LEFT_KICK) - 1]["ATTACK_START_FRAME"]),
	ATTACK_LEFT_KICK_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_LEFT_KICK) - 1]["ATTACK_END_FRAME"]),
	ATTACK_LEFT_KICK_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_LEFT_KICK) - 1]["DAMAGE"]),
	ATTACK_RIGHT_KICK_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_RIGHT_KICK) - 1]["ATTACK_START_FRAME"]),
	ATTACK_RIGHT_KICK_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_RIGHT_KICK) - 1]["ATTACK_END_FRAME"]),
	ATTACK_RIGHT_KICK_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_RIGHT_KICK) - 1]["DAMAGE"]),
	ATTACK_UPPER_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_UPPER) - 1]["ATTACK_START_FRAME"]),
	ATTACK_UPPER_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_UPPER) - 1]["ATTACK_END_FRAME"]),
	ATTACK_UPPER_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_UPPER) - 1]["DAMAGE"]),
	ATTACK_CHARGE_PUNCH_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_CHARGE_PUNCH) - 1]["ATTACK_START_FRAME"]),
	ATTACK_CHARGE_PUNCH_END_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_CHARGE_PUNCH) - 1]["ATTACK_END_FRAME"]),
	ATTACK_CHARGE_PUNCH_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_CHARGE_PUNCH) - 1]["DAMAGE"]),
	ATTACK_CHARGE_PUNCH_TIME(data["ATTACK_CHARGE_PUNCH_TIME"]),
	ATTACK_SPECIAL_PUNCH_START_FRAME(data["ANIM"][static_cast<int>(PlayerState::ATTACK_SPECIAL_PUNCH) - 1]["ATTACK_START_FRAME"]),
	ATTACK_SPECIAL_PUNCH_DAMAGE(data["ANIM"][static_cast<int>(PlayerState::ATTACK_SPECIAL_PUNCH) - 1]["DAMAGE"]),
	ATTACK_SPECIAL_PUNCH_COLLISION_TIME(data["ATTACK_SPECIAL_PUNCH_COLLISION_TIME"]),
	EVASION_MOVE_POW(data["EVASION_MOVE_POW"])
{

	// 機能の初期化
	InitFunction();

	// モデルID
	modelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::MODEL_PLAYER)]);

	// 関数ポインタの初期化
	InitFunctionPointer();

	// 共通部分は基底クラスで初期化
	ActorBase::Init(pos);

	// パラメータの初期化
	InitParameter();

	// 画像の初期化
	InitImage();

	// アニメーションの初期化
	InitAnimation();

	// BGMとSEの初期化
	InitBGMAndSE();

}

void Player::Init(const VECTOR& pos)
{

	// 入力用のコンポーネントを追加
	inputComponent_ = std::make_unique<InputComponent>(std::static_pointer_cast<Player>(GetThis()));

	// 移動用のコンポーネントを追加
	moveComponent_ = std::make_unique<MoveComponent>(std::static_pointer_cast<Player>(GetThis()));

	// 描画用のコンポーネント
	drawComponent_ = std::make_unique<DrawComponent>(std::static_pointer_cast<Player>(GetThis()));

}

void Player::InitFunction()
{

	// カメラを生成
	std::weak_ptr<Camera> camera = SceneManager::GetInstance().GetCamera();

	inputController_ = std::make_unique<InputController>();

	// カメラのターゲットをプレイヤーに設定
	camera.lock()->SetPlayer(transform_);

}

void Player::InitFunctionPointer()
{
	
	//関数ポインタの初期化
	stateChange_.emplace(PlayerState::IDLE, std::bind(&Player::ChangeIdle, this));
	stateChange_.emplace(PlayerState::RUN, std::bind(&Player::ChangeRun, this));
	stateChange_.emplace(PlayerState::ATTACK_JAB, std::bind(&Player::ChangeJab, this));
	stateChange_.emplace(PlayerState::ATTACK_STRAIGHT, std::bind(&Player::ChangeStraight, this));
	stateChange_.emplace(PlayerState::ATTACK_HOOK, std::bind(&Player::ChangeHook, this));
	stateChange_.emplace(PlayerState::ATTACK_LEFT_KICK, std::bind(&Player::ChangeLeftKick, this));
	stateChange_.emplace(PlayerState::ATTACK_RIGHT_KICK, std::bind(&Player::ChangeRightKick, this));
	stateChange_.emplace(PlayerState::ATTACK_UPPER, std::bind(&Player::ChangeUpper, this));
	stateChange_.emplace(PlayerState::ATTACK_CHARGE_PUNCH, std::bind(&Player::ChangeChargePunch, this));
	stateChange_.emplace(PlayerState::ATTACK_SPECIAL_PUNCH, std::bind(&Player::ChangeSpecialAttack, this));
	stateChange_.emplace(PlayerState::POWER_CHARGE, std::bind(&Player::ChangePowerCharge, this));
	stateChange_.emplace(PlayerState::EVASION, std::bind(&Player::ChangeEvasion, this));
	stateChange_.emplace(PlayerState::HIT_HEAD, std::bind(&Player::ChangeHitHead, this));
	stateChange_.emplace(PlayerState::HIT_BODY, std::bind(&Player::ChangeHitBody, this));
	stateChange_.emplace(PlayerState::DEATH, std::bind(&Player::ChangeDeath, this));

}

void Player::InitParameter()
{

	// 攻撃の入力
	for (int i = static_cast<int>(PlayerState::ATTACK_JAB); i <= static_cast<int>(PlayerState::ATTACK_RIGHT_KICK); i++)
	{
		isCombo_.emplace(static_cast<PlayerState>(i), false);
	}

	// 方向
	dir_ = { 0.0f,0.0f,1.0f };

	// 動く方向
	moveDir_ = { 0.0f,0.0f,0.0f };

	// ボス戦のみ角度を変える
	if (SceneManager::GetInstance().GetSceneID() == SCENE_ID::BOSS_BATTLE)
	{
		// 角度を変更
		transform_->quaRot = Quaternion::Euler({ Utility::Deg2RadF(0.0f) , Utility::Deg2RadF(180.0f),Utility::Deg2RadF(0.0f) });
		transform_->Update();
	}

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

	// 必殺技の当たり判定の座標を設定
	collisionData_.projectilePos = VAdd(transform_->pos, BODY_RELATIVE_CENTER_POS);

	// 手足の衝突判定の半径
	collisionData_.handAndFootCollisionRadius = HAND_AND_FOOT_COLLISION_RADIUS;

	// 体の衝突判定の半径
	collisionData_.bodyCollisionRadius = BODY_COLLISION_RADIUS;

	// 必殺技の衝突判定の半径
	collisionData_.projectileCollisionRadius = jsonData_["PROJECTILE_COLLISION_RADIUS"];

	// 右手に攻撃判定があるかどうか
	collisionData_.isRightHandAttack = false;

	// 左手に攻撃判定があるかどうか
	collisionData_.isLeftHandAttack = false;

	// 右足に攻撃判定があるかどうか
	collisionData_.isRightFootAttack = false;

	// 左足に攻撃判定があるかどうか
	collisionData_.isLeftFootAttack = false;

	// 必殺技に攻撃判定があるかどうか
	collisionData_.isProjectileAttack = false;

	// 溜めパンチのカウンタ
	attackChargePunchCnt_ = 0.0f;

	// HPの最大値
	HP_MAX = jsonData_["HP"];

	// HP
	hp_ = HP_MAX;

	// HPバーの長さ
	HP_BAR_LENGTH = jsonData_["HP_BAR_LENGTH"];

	// 必殺技ゲージ
	specialAttackGauge_ = 0;

	// 必殺技ゲージの最大値
	SPECIAL_ATTACK_MAX_GAUGE = jsonData_["SPECIAL_ATTACK_MAX_GAUGE"];
		
	// 必殺技ゲージの長さ
	SPECIAL_ATTACK_GAUGE_LENGTH = jsonData_["SPECIAL_ATTACK_GAUGE_LENGTH"];

	// 走るときの移動量
	RUN_MOVE_POW = jsonData_["RUN_MOVE_POW"];

	// アニメーション番号
	ANIM_INDEX = jsonData_["ANIM_INDEX"];

	// ゲームシーンの情報を取得
	std::shared_ptr<BossBattleScene> bossBattleScene =
		std::dynamic_pointer_cast<BossBattleScene>(SceneManager::GetInstance().GetNowScene());

	// NULLチェック
	if (!bossBattleScene)return;

	// アクターマネージャーを取得
	if (bossBattleScene)
	{

		// プレイヤー必殺技のゲージを設定 
		specialAttackGauge_ = SceneManager::GetInstance().GetPlayerSpecialAttackGauge();
		//specialAttackGauge_ = 0;

		// プレイヤーのHPを設定
		hp_ = SceneManager::GetInstance().GetPlayerHp();
		//hp_ = 100;

	}

}

void Player::InitImage()
{

	// アイコンの画像
	iconImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_PLAYER_ICON)]).handleId_;

	// HPバーの画像
	hpBarImg_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_PLAYER_HP_BAR)]).handleId_;

}

void Player::InitAnimation()
{

	// アニメーションコントローラの生成
	animationController_ = std::make_unique<AnimationController>(transform_->modelId);

	// アニメーションコントローラーにアニメーションを追加
	for (int i = static_cast<int>(PlayerState::IDLE); i < static_cast<int>(PlayerState::MAX); ++i)
	{

		// ループ再生するアニメーションだけisLoopをtrueにする
		bool isLoop = i == static_cast<int>(PlayerState::IDLE) || i == static_cast<int>(PlayerState::RUN);
		animationController_->Add(

			// アニメーションの名前
			jsonData_["ANIM"][i - 1]["NAME"],

			// アニメーションが始まる時間
			0.0f,

			// アニメーションスピード
			jsonData_["ANIM"][i - 1]["SPEED"],

			// アニメーションハンドル
			resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::MODEL_PLAYER) + i]).handleId_,

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
	ChangeState(PlayerState::IDLE);

}

void Player::InitBGMAndSE()
{

	// 足音の初期化
	footStepsSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_PLAYER_FOOT_STEPS_SE)]).handleId_;

	// 足音のボリュームの変更
	ChangeVolumeSoundMem(255 * 60 / 100, footStepsSE_);

	// ジャブの音の初期化
	jabSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_PLAYER_JAB_SE)]).handleId_;

	// ストレートの音の初期化
	straightSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_PLAYER_STRAIGHT_SE)]).handleId_;

	// フックの音の初期化
	hookSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_PLAYER_HOOK_SE)]).handleId_;

	// アッパーの音の初期化
	upperSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_PLAYER_UPPER_SE)]).handleId_;

	// ため攻撃の音の初期化
	chargePunchSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_PLAYER_CHARGE_PUNCH_SE)]).handleId_;
	
	// 左キックの音の初期化
	leftKickSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_PLAYER_LEFT_KICK_SE)]).handleId_;
	
	// 右キックの音の初期化
	rightKickSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_PLAYER_RIGHT_KICK_SE)]).handleId_;

	// 必殺輪のの音の初期化
	specialAttackSE_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::SOUND_PLAYER_SPECIAL_ATTACK_SE)]).handleId_;

}

void Player::Update(const float deltaTime)
{

	// ImGuiのデバッグ描画の更新
	//UpdateDebugImGui();

	// 入力の更新
	inputComponent_->Update(deltaTime);

	// 衝突判定の更新
	ActorBase::CollisionUpdate();

	// 状態ごとの更新
	stateUpdate_(deltaTime);

	// 重力
	Gravity(gravityScale_);
	 
	// モデル情報を更新
	transform_->Update();

	// アニメーション再生
	animationController_->Update(deltaTime);

	// アニメーションのフレームを固定
	AnimationFrame();

	// 走っているとき以外は足音を止める
	if (state_ != PlayerState::RUN)
	{
		StopSoundMem(footStepsSE_);
	}

}

void Player::Draw(const float deltaTime)
{

	// 基底クラスの描画処理
	ActorBase::Draw(deltaTime);

	// HPバー描画
	int H;
	int hpGauge;
	H = hp_ * (static_cast<int>(512.0f) / HP_MAX) - 100;
	int R = min(max((384 - H), 0), 0xff);
	int G = min(max((H + 64), 0), 0xff);
	int B = max((H - 384), 0);
	hpGauge = HP_BAR_LENGTH * hp_ / HP_MAX;

	// 必殺技ゲージの描画
	int specialAttackGauge = SPECIAL_ATTACK_GAUGE_LENGTH * specialAttackGauge_ / SPECIAL_ATTACK_MAX_GAUGE;

	if (hp_ >= 0)
	{

		// HPバー
		DrawBox(200, 550, 200 + hpGauge, 570, GetColor(R, G, B), true);

		// HPバーの箱
		DrawExtendGraph(200, 550, 200 + HP_BAR_LENGTH, 570, hpBarImg_, true);

		// 必殺技ゲージ
		DrawBox(200, 600, 200 + specialAttackGauge, 620, 0x65bbe9, true);

		// 必殺技ゲージの箱
		DrawExtendGraph(200, 600, 200 + SPECIAL_ATTACK_GAUGE_LENGTH, 620, hpBarImg_, true);

	}

	// アイコンの描画
	DrawRotaGraph(100, 550, 0.5, 0.0, iconImg_, true);

}

void Player::UpdateDebugImGui()
{

	// ウィンドウタイトル&開始処理
	ImGui::Begin("Player");

	// 大きさ
	ImGui::Text("scale");
	ImGui::InputFloat("Scl", &scl_);

	// 角度
	VECTOR rotDeg = VECTOR();
	rotDeg.x = Utility::Rad2DegF(static_cast<float>(transform_->quaRot.x));
	rotDeg.y = Utility::Rad2DegF(static_cast<float>(transform_->quaRot.y));
	rotDeg.z = Utility::Rad2DegF(static_cast<float>(transform_->quaRot.z));
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

void Player::AnimationFrame()
{

	// 対象フレームのローカル行列を初期値にリセットする
	MV1ResetFrameUserLocalMatrix(transform_->modelId, collisionData_.body);

	// 座標を固定する
	if (animationController_->IsBlendPlay("EVASION"))
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

const bool Player::GetAttackState()const
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

const bool Player::GetCloseRangeAttackState() const
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

const std::vector<int> Player::GetTotalAttackTypes() const
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

const bool Player::GetHitState()const
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

const bool Player::GetIsSuperArmor() const
{
	// スーパーアーマー状態か判定
	for (const auto state : superArmorState_)
	{
		if (state_ == state)
		{
			return true;
		}
	}

	return false;
}

bool Player::GetComboState()
{

	// コンボ中か判定
	for (const auto state : comboState_)
	{
		if (state_ == state)
		{
			return true;
		}
	}

	return false;

}

void Player::AttackHit(const int damage, const int state)
{

	// スーパーアーマー状態か判定
	if (GetIsSuperArmor())
	{
		return;
	}

	// どのアニメーションかチェックする
	AttackHitCheck(state);

	// HPを減らす
	SubHp(damage);
	
	if (hp_ <= 0)
	{

		// 0以下にならないようにする
		hp_ = 0;

		// 死ぬアニメーションに遷移
		DeathAnim(state);

	}

	// アニメーションの再生時間をリセットする
	animationController_->ResetStepAnim();

}

void Player::ProjectileHit(const int damage)
{

	// スーパーアーマー状態か判定
	if (GetIsSuperArmor())
	{
		return;
	}

	// ヒットアニメーションに遷移
	ChangeState(PlayerState::HIT_BODY);

	// HPを減らす
	SubHp(damage);

	// アニメーションの再生時間をリセットする
	animationController_->ResetStepAnim();

}

void Player::AttackHitCheck(const int type)
{

	// 頭にヒットするアニメーションかチェック
	for (const auto hitState : hitHeadEnemyState_)
	{
		if (hitState == static_cast<EnemyState>(type))
		{
			ChangeState(PlayerState::HIT_HEAD);
			return;
		}
	}

	// 頭にヒットするアニメーションかチェック
	for (const auto hitState : hitHeadBossState_)
	{
		if (hitState == static_cast<BossState>(type))
		{
			ChangeState(PlayerState::HIT_HEAD);
			return;
		}
	}

	// 体にヒットするアニメーションかチェック
	for (const auto hitState : hitBodyEnemyState_)
	{
		if (hitState == static_cast<EnemyState>(type))
		{
			ChangeState(PlayerState::HIT_BODY);
			return;
		}
	}

	// 体にヒットするアニメーションかチェック
	for (const auto hitState : hitBodyBossState_)
	{
		if (hitState == static_cast<BossState>(type))
		{
			ChangeState(PlayerState::HIT_BODY);
			return;
		}
	}

}

void Player::Rotation()
{

	float cameraAngleY = 0.0f;

	// カメラの角度
	if (!Utility::EqualsVZero(inputController_->Dir()))
	{
		cameraAngleY_ = SceneManager::GetInstance().GetCamera().lock()->GetAngle().y;
		cameraAngleY = cameraAngleY_;
	}

	// Y軸の行列
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotY(cameraAngleY_));

	// 回転行列を使用して、ベクトルを回転させる
	moveDir_ = VTransform(dir_, mat);

	// 方向を角度に変換する(XZ平面 Y軸)
	float angle = atan2f(dir_.x, dir_.z);
	
	// ゆっくり回転する
	if (cameraAngleY != 0.0f)
	{
		LazyRotation(cameraAngleY + angle);
	}

}

void Player::DeathAnim(int state)
{

	// 死亡アニメーションかチェック
	for (const auto hitState : deathEnemyState_)
	{
		if (hitState == static_cast<EnemyState>(state))
		{
			ChangeState(PlayerState::DEATH);
			return;
		}
	}

	// 死亡アニメーションかチェック
	for (const auto hitState : deathBossState_)
	{
		if (hitState == static_cast<BossState>(state))
		{
			ChangeState(PlayerState::DEATH);
			return;
		}
	}

}

void Player::ChangeState(const PlayerState state)
{

	// 前のアニメーションを保存
	preKey_ = key_;

	// 新しいアニメーションを保存
	key_ = ANIM_DATA_KEY[static_cast<int>(state)];

	// 前と同じアニメーションなら処理しない
	if (preKey_ == key_)return;

	// 状態遷移
	state_ = state;

	// 関数ポインタの遷移
	stateChange_[state_]();

	// 前と同じ状態でなければカウンタをリセットする
	if (preKey_ != key_)
	{
		// 溜めパンチのカウンタをリセット
		attackChargePunchCnt_ = 0.0f;
	}

	// アニメーションコントローラー側のアニメーション遷移
	animationController_->ChangeAnimation(key_);

}

void Player::ChangeIdle(void)
{

	stateUpdate_ = std::bind(&Player::UpdateIdle, this, std::placeholders::_1);

	// 重力を通常状態に戻す
	gravityScale_ = 1.0f;

}

void Player::ChangeRun(void)
{

	stateUpdate_ = std::bind(&Player::UpdateRun, this, std::placeholders::_1);

	// スピード
	speed_ = RUN_MOVE_POW;

}

void Player::ChangeJab()
{

	stateUpdate_ = std::bind(&Player::UpdateJab, this, std::placeholders::_1);

	// 左手の攻撃判定をなくす
	collisionData_.isLeftHandAttack = false;

	// ダメージ量
	damage_ = ATTACK_JAB_DAMAGE;

	// スピード
	speed_ = ATTACK_MOVE_POW;

	// 移動処理
	inputComponent_->Dir();

}

void Player::ChangeStraight()
{

	stateUpdate_ = std::bind(&Player::UpdateStraight, this, std::placeholders::_1);

	// 右手の攻撃判定をなくす
	collisionData_.isRightHandAttack = false;

	// ダメージ量
	damage_ = ATTACK_STRAIGHT_DAMAGE;

	// スピード
	speed_ = ATTACK_MOVE_POW;

	// 移動処理
	inputComponent_->Dir();

}

void Player::ChangeHook()
{

	stateUpdate_ = std::bind(&Player::UpdateHook, this, std::placeholders::_1);

	// 左手の攻撃判定をなくす
	collisionData_.isLeftHandAttack = false;

	// ダメージ量
	damage_ = ATTACK_HOOK_DAMAGE;

	// スピード
	speed_ = ATTACK_MOVE_POW;

	// 移動処理
	inputComponent_->Dir();

}

void Player::ChangeLeftKick()
{

	stateUpdate_ = std::bind(&Player::UpdateLeftKick, this, std::placeholders::_1);

	// 左足の攻撃判定をなくす
	collisionData_.isLeftFootAttack = false;

	// ダメージ量
	damage_ = ATTACK_LEFT_KICK_DAMAGE;

	// スピード
	speed_ = ATTACK_MOVE_POW;

	// 移動処理
	inputComponent_->Dir();

}

void Player::ChangeRightKick()
{

	stateUpdate_ = std::bind(&Player::UpdateRightKick, this, std::placeholders::_1);

	// 右足の攻撃判定をなくす
	collisionData_.isRightFootAttack = false;

	// ダメージ量
	damage_ = ATTACK_RIGHT_KICK_DAMAGE;

	// スピード
	speed_ = ATTACK_MOVE_POW;

	// 移動処理
	inputComponent_->Dir();

}

void Player::ChangeUpper()
{

	stateUpdate_ = std::bind(&Player::UpdateUpper, this, std::placeholders::_1);

	// 右手の攻撃判定をなくす
	collisionData_.isRightHandAttack = false;

	// ダメージ量
	damage_ = ATTACK_UPPER_DAMAGE;

	// スピード
	speed_ = ATTACK_MOVE_POW;

}

void Player::ChangeChargePunch()
{

	stateUpdate_ = std::bind(&Player::UpdateChargePunch, this, std::placeholders::_1);

	// 右手の攻撃判定をなくす
	collisionData_.isRightHandAttack = false;

	// ダメージ量
	damage_ = ATTACK_CHARGE_PUNCH_DAMAGE;

	// スピード
	speed_ = ATTACK_MOVE_POW;

}

void Player::ChangeSpecialAttack()
{

	stateUpdate_ = std::bind(&Player::UpdateSpecialAttack, this, std::placeholders::_1);

	// ダメージ量
	damage_ = ATTACK_SPECIAL_PUNCH_DAMAGE;

	// 必殺技ゲージをリセット
	specialAttackGauge_ = 0;

	// 必殺技の当たり判定の座標を設定
	collisionData_.projectilePos = VAdd(transform_->pos, BODY_RELATIVE_CENTER_POS);

	// 必殺技の衝突判定が続く時間のカウンタをリセット
	attackSpecialPunchCollisionCnt_ = 0.0f;

	// カメラを生成
	std::weak_ptr<Camera> camera = SceneManager::GetInstance().GetCamera();
	camera.lock()->ChangeMode(Camera::MODE::FOLLOW);

}

void Player::ChangePowerCharge()
{

	stateUpdate_ = std::bind(&Player::UpdatePowerCharge, this, std::placeholders::_1);

	// カメラを生成
	std::weak_ptr<Camera> camera = SceneManager::GetInstance().GetCamera();
	camera.lock()->ChangeMode(Camera::MODE::SPECIAL);

}

void Player::ChangeEvasion()
{

	stateUpdate_ = std::bind(&Player::UpdateEvasion, this, std::placeholders::_1);

	// スピード
	speed_ = EVASION_MOVE_POW;

}

void Player::ChangeHitHead()
{

	stateUpdate_ = std::bind(&Player::UpdateHitHead, this, std::placeholders::_1);

	// プレイヤーの方向を求める
	VECTOR vec = VSub(targetPos_, transform_->pos);

	// 正規化
	vec = VNorm(vec);

	// プレイヤーの方向と逆方向のベクトル
	vec = { -vec.x, vec.y,-vec.z };

	// スピード
	speed_ = ATTACK_MOVE_POW;

}

void Player::ChangeHitBody()
{

	stateUpdate_ = std::bind(&Player::UpdateHitBody, this, std::placeholders::_1);

	// プレイヤーの方向を求める
	VECTOR vec = VSub(targetPos_, transform_->pos);

	// 正規化
	vec = VNorm(vec);

	// プレイヤーの方向と逆方向のベクトル
	vec = { -vec.x, vec.y,-vec.z };

	// スピード
	speed_ = ATTACK_MOVE_POW;

}

void Player::ChangeDeath()
{

	stateUpdate_ = std::bind(&Player::UpdateDeath, this, std::placeholders::_1);

	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::GAME_OVER);

}

void Player::UpdateIdle(const float deltaTime)
{

	// 攻撃の入力を初期化
	for (int i = static_cast<int>(PlayerState::ATTACK_JAB); i <= static_cast<int>(PlayerState::ATTACK_RIGHT_KICK); i++)
	{
		isCombo_.at(static_cast<PlayerState>(i)) = false;
	}

}

void Player::UpdateRun(const float deltaTime)
{

	// 攻撃の入力を初期化
	for (int i = static_cast<int>(PlayerState::ATTACK_JAB); i <= static_cast<int>(PlayerState::ATTACK_RIGHT_KICK); i++)
	{
		isCombo_.at(static_cast<PlayerState>(i)) = false;
	}

	// 回転処理
	Rotation();

	// 移動処理
	moveComponent_->Move();

	if (!CheckSoundMem(footStepsSE_))
	{
		// 足音を再生
		PlaySoundMem(footStepsSE_, DX_PLAYTYPE_BACK, true);
	}

}

void Player::UpdateJab(const float deltaTime)
{

	// 回転処理
	Rotation();

	// 少し前にゆっくり移動
	moveComponent_->Lerp();

	// 攻撃判定があるフレーム
	if (ATTACK_JAB_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= ATTACK_JAB_END_FRAME)
	{
		collisionData_.isLeftHandAttack = true;
	}
	else
	{
		collisionData_.isLeftHandAttack = false;
	}

	// 攻撃が当たっていたら音を再生
	if (isHitAttack_)
	{
		PlaySoundMem(jabSE_, DX_PLAYTYPE_BACK, true);
		isHitAttack_ = false;
	}

	// ストレートに遷移
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(PlayerState::ATTACK_STRAIGHT))
	{
		ChangeState(PlayerState::ATTACK_STRAIGHT);
	}

}

void Player::UpdateStraight(const float deltaTime)
{

	// 回転処理
	Rotation();

	// 少し前にゆっくり移動
	moveComponent_->Lerp();

	// 攻撃判定があるフレーム
	if (ATTACK_STRAIGHT_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= ATTACK_STRAIGHT_END_FRAME)
	{
		collisionData_.isRightHandAttack = true;
	}
	else
	{
		collisionData_.isRightHandAttack = false;
	}

	// 攻撃が当たっていたら音を再生
	if (isHitAttack_)
	{
		PlaySoundMem(straightSE_, DX_PLAYTYPE_BACK, true);
		isHitAttack_ = false;
	}

	// フックに遷移
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(PlayerState::ATTACK_HOOK))
	{
		ChangeState(PlayerState::ATTACK_HOOK);
	}

}

void Player::UpdateHook(const float deltaTime)
{

	// 回転処理
	Rotation();

	// 少し前にゆっくり移動
	moveComponent_->Lerp();

	// 攻撃判定があるフレーム
	if (ATTACK_HOOK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= ATTACK_HOOK_END_FRAME)
	{
		collisionData_.isLeftHandAttack = true;
	}
	else
	{
		collisionData_.isLeftHandAttack = false;
	}

	// 攻撃が当たっていたら音を再生
	if (isHitAttack_)
	{
		PlaySoundMem(hookSE_, DX_PLAYTYPE_BACK, true);
		isHitAttack_ = false;
	}

	// 左キックに遷移
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(PlayerState::ATTACK_LEFT_KICK))
	{
		ChangeState(PlayerState::ATTACK_LEFT_KICK);
	}

}

void Player::UpdateLeftKick(const float deltaTime)
{

	// 回転処理
	Rotation();

	// 少し前にゆっくり移動
	moveComponent_->Lerp();

	// 攻撃判定があるフレーム
	if (ATTACK_LEFT_KICK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= ATTACK_LEFT_KICK_END_FRAME)
	{
		collisionData_.isLeftFootAttack = true;
	}
	else
	{
		collisionData_.isLeftFootAttack = false;
	}

	// 攻撃が当たっていたら音を再生
	if (isHitAttack_)
	{
		PlaySoundMem(leftKickSE_, DX_PLAYTYPE_BACK, true);
		isHitAttack_ = false;
	}

	// 右キックに遷移
	if (animationController_->IsEndPlayAnimation() && isCombo_.at(PlayerState::ATTACK_RIGHT_KICK))
	{
		ChangeState(PlayerState::ATTACK_RIGHT_KICK);
	}

}

void Player::UpdateRightKick(const float deltaTime)
{

	// 回転処理
	Rotation();

	// 少し前にゆっくり移動
	moveComponent_->Lerp();

	// 攻撃判定があるフレーム
	if (ATTACK_RIGHT_KICK_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= ATTACK_RIGHT_KICK_END_FRAME)
	{
		collisionData_.isRightFootAttack = true;
	}
	else
	{
		collisionData_.isRightFootAttack = false;
	}

	// 攻撃が当たっていたら音を再生
	if (isHitAttack_)
	{
		PlaySoundMem(rightKickSE_, DX_PLAYTYPE_BACK, true);
		isHitAttack_ = false;
	}

	// 待機状態に遷移
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);
	}

}

void Player::UpdateUpper(const float deltaTime)
{

	// 少し前にゆっくり移動
	moveComponent_->Lerp();

	// 攻撃判定があるフレーム
	if (ATTACK_UPPER_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= ATTACK_UPPER_END_FRAME)
	{
		collisionData_.isRightHandAttack = true;
	}
	else
	{
		collisionData_.isRightHandAttack = false;
	}

	// 攻撃が当たっていたら音を再生
	if (isHitAttack_)
	{
		PlaySoundMem(upperSE_, DX_PLAYTYPE_BACK, true);
		isHitAttack_ = false;
	}

	// 待機状態に遷移
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);
	}

}

void Player::UpdateChargePunch(const float deltaTime)
{

	// 少し前にゆっくり移動
	moveComponent_->Lerp();

	// 攻撃判定があるフレーム
	if (ATTACK_CHARGE_PUNCH_START_FRAME <= animationController_->GetStepAnim() && animationController_->GetStepAnim() <= ATTACK_CHARGE_PUNCH_END_FRAME)
	{
		collisionData_.isRightHandAttack = true;
	}
	else
	{
		collisionData_.isRightHandAttack = false;
	}

	// 攻撃が当たっていたら音を再生
	if (isHitAttack_)
	{
		PlaySoundMem(chargePunchSE_, DX_PLAYTYPE_BACK, true);
		isHitAttack_ = false;
	}

	// 待機状態に遷移
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);
	}

}

void Player::UpdateSpecialAttack(const float deltaTime)
{

	// 攻撃判定があるフレーム
	if (ATTACK_SPECIAL_PUNCH_START_FRAME <= animationController_->GetStepAnim() && ATTACK_SPECIAL_PUNCH_COLLISION_TIME >= attackSpecialPunchCollisionCnt_)
	{

		// 当たり判定をつける
		collisionData_.isProjectileAttack = true;

		// 必殺技の当たり判定の座標設定
		collisionData_.projectilePos = VAdd(collisionData_.projectilePos, VScale(transform_->quaRot.GetForward(), 1000.0f));

		// 必殺技の衝突判定が続く時間のカウンタを加算
		attackSpecialPunchCollisionCnt_ += deltaTime;

	}

	// 攻撃が当たっていたら音を再生
	if (isHitAttack_)
	{
		PlaySoundMem(specialAttackSE_, DX_PLAYTYPE_BACK, true);
		isHitAttack_ = false;
	}

	// 待機状態に遷移
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);

		// 当たり判定を消す
		collisionData_.isProjectileAttack = false;

	}

}

void Player::UpdatePowerCharge(const float deltaTime)
{

	// 必殺技に遷移
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::ATTACK_SPECIAL_PUNCH);
	}

}

void Player::UpdateEvasion(const float deltaTime)
{

	// 移動処理
	moveComponent_->Move();

	// 待機状態に遷移
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);
	}

}

void Player::UpdateHitHead(const float deltaTime)
{

	// 待機状態に遷移
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);
	}

}

void Player::UpdateHitBody(const float deltaTime)
{

	// 待機状態に遷移
	if (animationController_->IsEndPlayAnimation())
	{
		ChangeState(PlayerState::IDLE);
	}

}

void Player::UpdateDeath(const float deltaTime)
{

	// 待機状態に遷移
	if (animationController_->IsEndPlayAnimation())
	{

		// ゲームシーンの情報を取得
		std::shared_ptr<GameScene> gameScene =
			std::dynamic_pointer_cast<GameScene>(SceneManager::GetInstance().GetNowScene());

		// ボスバトルシーンの情報を取得
		std::shared_ptr<BossBattleScene> bossBattleScene =
			std::dynamic_pointer_cast<BossBattleScene>(SceneManager::GetInstance().GetNowScene());

		// NULLチェック
		if (gameScene)
		{
			// アニメション再生が終わったら死ぬ
			gameScene->SetIsPlayerDeath(true);
		}

		// NULLチェック
		if (bossBattleScene)
		{
			// アニメション再生が終わったら死ぬ
			bossBattleScene->SetIsPlayerDeath(true);
		}

	}

}