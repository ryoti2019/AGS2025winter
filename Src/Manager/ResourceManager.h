#pragma once
#include <map>
#include <array>
#include <string>
#include "../Lib/nlohmann/json.hpp"
#include "Resource.h"

class ResourceManager
{

public:

	// リソース名
	enum class SRC
	{

#pragma region プレイヤーのモデルとアニメーション

		MODEL_PLAYER,
		ANIM_PLAYER_IDLE,
		ANIM_PLAYER_RUN,
		ANIM_PLAYER_JAB,
		ANIM_PLAYER_STRAIGHT,
		ANIM_PLAYER_HOOK,
		ANIM_PLAYER_LEFT_KICK,
		ANIM_PLAYER_RIGHT_KICK,
		ANIM_PLAYER_UPPER,
		ANIM_PLAYER_CHARGE_PUNCH,
		ANIM_PLAYER_SPECIAL_ATTACK,
		ANIM_PLAYER_POWER_CHARGE,
		ANIM_PLAYER_HIT_HEAD,
		ANIM_PLAYER_HIT_BODY,
#pragma endregion

#pragma region 敵のモデルとアニメーション

		MODEL_ENEMY,
		ANIM_ENEMY_IDLE,
		ANIM_ENEMY_RUN,
		ANIM_ENEMY_PUNCH,
		ANIM_ENEMY_KICK,
		ANIM_ENEMY_HIT_HEAD,
		ANIM_ENEMY_HIT_BODY,
		ANIM_ENEMY_HIT_FLY,
		ANIM_ENEMY_HIT_FLINCH,
		ANIM_ENEMY_HIT_KNOCK_BACK,
		ANIM_ENEMY_KIP_UP,
		ANIM_ENEMY_NORMAL_DEATH,

#pragma endregion

#pragma region ボスのモデルとアニメーション

		MODEL_BOSS,
		ANIM_BOSS_IDLE,
		ANIM_BOSS_RUN,
		ANIM_BOSS_PUNCH,
		ANIM_BOSS_KICK,
		ANIM_BOSS_PROJECTILE,
		ANIM_HIT_HEAD,
		ANIM_HIT_BODY,
		ANIM_HIT_FLY,
		ANIM_HIT_FLINCH_UP,
		ANIM_HIT_KNOCK_BACK,
		ANIM_KIP_UP,
		ANIM_DEATH,

#pragma endregion

#pragma region ステージのモデル

		STAGE_MODEL,
		STAGE_COLLISION_MODEL,
		STAGE_AREA1_COLLISION_MODEL,
		STAGE_AREA2_COLLISION_MODEL,
		STAGE_AREA3_COLLISION_MODEL,
		STAGE_AREA4_COLLISION_MODEL,
		STAGE_AREA5_COLLISION_MODEL,

#pragma endregion

#pragma region プレイヤーのエフェクト



#pragma endregion

#pragma region 敵のエフェクト



#pragma endregion

#pragma region ボスのエフェクト

		EFFECT_BOSS_PROJECTILE,

#pragma endregion

		MAX

	};

	const std::string RESOURCE_KEY[static_cast<int>(SRC::MAX)] =
	{

#pragma region プレイヤーのモデルとアニメーション

		"MODEL_PLAYER",
		"ANIM_PLAYER_IDLE",
		"ANIM_PLAYER_RUN",
		"ANIM_PLAYER_JAB",
		"ANIM_PLAYER_STRAIGHT",
		"ANIM_PLAYER_HOOK",
		"ANIM_PLAYER_LEFT_KICK",
		"ANIM_PLAYER_RIGHT_KICK",
		"ANIM_PLAYER_UPPER",
		"ANIM_PLAYER_CHARGE_PUNCH",
		"ANIM_PLAYER_SPECIAL_ATTACK",
		"ANIM_PLAYER_POWER_CHARGE",
		"ANIM_PLAYER_HIT_HEAD",
		"ANIM_PLAYER_HIT_BODY",
#pragma endregion

#pragma region 敵のモデルとアニメーション

		"MODEL_ENEMY",
		"ANIM_ENEMY_IDLE",
		"ANIM_ENEMY_RUN",
		"ANIM_ENEMY_PUNCH",
		"ANIM_ENEMY_KICK",
		"ANIM_ENEMY_HIT_HEAD",
		"ANIM_ENEMY_HIT_BODY",
		"ANIM_ENEMY_HIT_FLY",
		"ANIM_ENEMY_HIT_FLINCH",
		"ANIM_ENEMY_HIT_KNOCK_BACK",
		"ANIM_ENEMY_KIP_UP",
		"ANIM_ENEMY_NORMAL_DEATH",

#pragma endregion

#pragma region ボスのモデルとアニメーション

		"MODEL_BOSS",
		"ANIM_BOSS_IDLE",
		"ANIM_BOSS_RUN",
		"ANIM_BOSS_PUNCH",
		"ANIM_BOSS_KICK",
		"ANIM_BOSS_PROJECTILE",
		"ANIM_HIT_HEAD",
		"ANIM_HIT_BODY",
		"ANIM_HIT_FLY",
		"ANIM_HIT_FLINCH_UP",
		"ANIM_HIT_KNOCK_BACK",
		"ANIM_KIP_UP",
		"ANIM_DEATH",

#pragma endregion

#pragma region ステージのモデル

		"STAGE_MODEL",
		"STAGE_COLLISION_MODEL",
		"STAGE_AREA1_COLLISION_MODEL",
		"STAGE_AREA2_COLLISION_MODEL",
		"STAGE_AREA3_COLLISION_MODEL",
		"STAGE_AREA4_COLLISION_MODEL",
		"STAGE_AREA5_COLLISION_MODEL",

#pragma endregion

#pragma region プレイヤーのエフェクト



#pragma endregion

#pragma region 敵のエフェクト



#pragma endregion

#pragma region ボスのエフェクト

		"EFFECT_BOSS_PROJECTILE"

#pragma endregion



	};

	// 明示的にインステンスを生成する
	static void CreateInstance();

	// 静的インスタンスの取得
	static ResourceManager& GetInstance();

	// 初期化
	void Init();

	// 解放(シーン切替時に一旦解放)
	void Release();

	// リソースの完全破棄
	void Destroy();

	// リソースのロード
	Resource Load(std::string src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(std::string src);

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<std::string, Resource> resourcesMap_;

	// 読み込み済みリソース
	std::map<std::string, Resource*> loadedMap_;

	// リソースの初期化
	void InitResource(const std::string& pathName, const nlohmann::json_abi_v3_11_3::json jsonData);

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager();

	// デストラクタも同様
	~ResourceManager() = default;

	// 内部ロード
	Resource* _Load(std::string src);

};