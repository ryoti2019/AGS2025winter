#pragma once
#include "../Object/ActorBase.h"
#include "../Manager/Camera.h"
#include "CollisionTypes.h"

class StageBase;

class CollisionManager
{

public:

	// 攻撃の最初の状態の番号
	static constexpr int ATTACK_START_NUM = 3;

	// ステージと衝突しているか判定する回数
	static constexpr int STAGE_COLLISION_COUNT = 10;

	// 敵同士の押し合う力
	static constexpr float ENEMY_PUSH_FORCE = 10.0f;

	// ステージとの当たり判定で押し返す力
	static constexpr float OBJECT_COLLISION_PUSH_FORCE = 50.0f;

	// ステージとカメラの当たり判定で押し返す力
	static constexpr float CAMERA_COLLISION_PUSH_FORCE = 1000.0f;

	// ほぼ真下への向き
	static constexpr float DOWN_DIR = 0.9f;

	// カメラの衝突判定で使う長さ
	static constexpr float LEN_CAMERACOLLISION_LINE = 500.0f;

	// カメラの衝突判定で一番短い長さ
	static constexpr float MIN_LEN_CAMERA_COLLISION_LINE = 1.0f;

	// 攻撃の半分を必殺技ゲージに加算する割合
	static constexpr int SPECIAL_ATTACK_GAUGE_RATIO = 10;

	// 無敵時間
	static constexpr int INVINCIBILITY_TIME = 0.5f;

	// カメラの押し返す力
	static constexpr float REPEL_FORCE = 10.0f;

	// カメラの最大距離
	static constexpr float MAX_CAMERA_DISTANCE = 5000.0f;

	// 線形補完の補完割合
	static constexpr float INTERPOLATION_RATIO = 0.1f;

	CollisionManager();
	~CollisionManager() = default;

	void Init();
	void Update(const float deltaTime);

	// 当たり判定を見たいものを登録する
	void Register(const std::shared_ptr<ActorBase>& actor);

	// ステージの当たり判定を消す
	void StageErasure(std::shared_ptr<StageBase>& actor);

	// カメラの情報を設定
	void SetCamera(const std::weak_ptr<Camera>& camera);

private:

	// カメラ
	std::weak_ptr<Camera> camera_;

	// ２つのActorの当たり判定を見るために組み合わせを登録する型
	struct collisionChannnelInfo
	{
		ActorType type1;
		ActorType type2;
	};

	// 敵とプレイヤーの攻撃が当たっているかの組み合わせを登録する
	const std::vector<collisionChannnelInfo> attackCollisionChannelList_ =
	{
		{
			{ActorType::PLAYER,ActorType::ENEMY},
			{ActorType::PLAYER,ActorType::BOSS},
			{ActorType::ENEMY,ActorType::PLAYER},
			{ActorType::BOSS,ActorType::PLAYER}
		}
	};

	// 敵とプレイヤーがステージに当たっているかの組み合わせを登録する
	const std::vector<collisionChannnelInfo> stageCollisionChannelList_ =
	{
		{
			{ActorType::PLAYER,ActorType::STAGE},
			{ActorType::ENEMY,ActorType::STAGE},
			{ActorType::BOSS,ActorType::STAGE}
		}
	};

	// プレイヤーと敵同士が当たっているかの組み合わせを登録する
	const std::vector<collisionChannnelInfo> resolveCollisionChannelList_ =
	{
		{
			{ActorType::PLAYER,ActorType::ENEMY},
			{ActorType::PLAYER,ActorType::BOSS},
			{ActorType::ENEMY,ActorType::ENEMY},
			{ActorType::ENEMY,ActorType::BOSS}
		}
	};

	// プレイヤーと敵の情報を登録する
	const std::vector<ActorType> objectData_ =
	{
		{
			{ActorType::PLAYER},
			{ActorType::ENEMY},
			{ActorType::BOSS},
		}
	};

	//壁衝突検知用の線分の長さ
	float  collisionLineStageCamera_;

	// どれだけ重なっているか
	VECTOR overlap_;

	// すべてのアクターの衝突判定をまとめたデータ
	std::unordered_map<ActorType, std::vector<std::shared_ptr<ActorBase>>> collisionActorData_;

	// 近距離攻撃が当たっているかどうかのデータ
	std::map<std::shared_ptr<ActorBase>, std::map<int, float>> isCloseRangeAttackHitData_;

	// 遠距離攻撃が当たっているかどうかのデータ
	std::map<std::shared_ptr<ActorBase>, float> isLongRangeAttackHitData_;

	// プレイヤーと敵の攻撃の当たり判定
	void CheckAttackCollision(const float deltaTime);

	// 攻撃が当たった時の処理
	void OnAttackCollision(const std::shared_ptr<ActorBase>& attacker, const std::shared_ptr<ActorBase>& target);

	// プレイヤーと敵の飛び道具の当たり判定
	void CheckProjectileCollision(const float deltaTime);

	// 飛び道具が当たった時の処理
	void OnProjectileCollision(const std::shared_ptr<ActorBase>& attacker, const std::shared_ptr<ActorBase>& target);

	// ステージとの当たり判定
	void CheckStageCollision();

	// アクターとステージとの当たり判定
	void CheckActorsAndStageCollision();

	// カメラとステージとの当たり判定
	void CheckCameraAndStageCollision();

	// プレイヤーや敵同士が重なってた時の当たり判定
	void CheckResolveCollision();

	// プレイヤーと敵が重なってた時の処理
	void ResolvePlayerEnemyCollision(const std::shared_ptr<ActorBase>& actor1, const std::shared_ptr<ActorBase>& actor2);

	// 敵同士が重なってた時の処理
	void ResolveEnemysCollision(const std::shared_ptr<ActorBase>& actor1, const std::shared_ptr<ActorBase>& actor2);

	// 制限エリアが出てくる場所との当たり判定
	void CheckRestrictedAreasCollision();

	// オブジェクトの当たり判定を消す
	void ObjectErasure();

};

