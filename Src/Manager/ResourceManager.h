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

#pragma region 画像

#pragma region プレイヤー

#pragma endregion

#pragma region 敵

#pragma endregion

#pragma region ボス

#pragma endregion

#pragma region ステージ

		IMAGE_NOISE_TEXTURE,

#pragma endregion

#pragma region タイトルシーン

		IMAGE_TITLE_LOGO,
		IMAGE_PRESS_A_BOTTON,

#pragma endregion

#pragma region ゲームシーン

		IMAGE_GAME_PAD_USER_GUIDE_1,
		IMAGE_GAME_PAD_USER_GUIDE_2,
		IMAGE_GAME_PAD_USER_GUIDE_3,
		IMAGE_GAME_PAD_USER_GUIDE_4,
		IMAGE_KEYBOARD_USER_GUIDE_1,
		IMAGE_KEYBOARD_USER_GUIDE_2,
		IMAGE_KEYBOARD_USER_GUIDE_3,
		IMAGE_KEYBOARD_USER_GUIDE_4,

#pragma endregion

#pragma endregion

#pragma region モデルとアニメーション

#pragma region プレイヤー

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
		ANIM_PLAYER_TITLE_IDLE,
#pragma endregion

#pragma region 敵

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

#pragma region ボス

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
		ANIM_BOSS_JUMP_IDLE,
		ANIM_BOSS_LAND,
		ANIM_DEATH,

#pragma endregion

#pragma region ステージ

		STAGE_MODEL,
		STAGE_COLLISION_MODEL,
		STAGE_AREA1_COLLISION_MODEL,
		STAGE_AREA2_COLLISION_MODEL,
		STAGE_AREA3_COLLISION_MODEL,
		STAGE_AREA4_COLLISION_MODEL,
		STAGE_AREA5_COLLISION_MODEL,

#pragma endregion

#pragma endregion

#pragma region エフェクト

#pragma region プレイヤー



#pragma endregion

#pragma region 敵



#pragma endregion

#pragma region ボス

		EFFECT_BOSS_PROJECTILE,

#pragma endregion

#pragma endregion

#pragma region BGM・SE

#pragma region タイトルシーン

		SOUND_TITLE_SCENE_BGM,
		SOUND_TITLE_SCENE_DECIDE_SE,

#pragma endregion

#pragma region ゲームシーン

		SOUND_GAME_SCENE_BGM,

#pragma endregion

#pragma endregion

		MAX

	};

	const std::string RESOURCE_KEY[static_cast<int>(SRC::MAX)] =
	{

#pragma region 画像

#pragma region プレイヤー

#pragma endregion

#pragma region 敵

#pragma endregion

#pragma region ボス

#pragma endregion

#pragma region ステージ

		"IMAGE_NOISE_TEXTURE",

#pragma endregion

#pragma region タイトルシーン

		"IMAGE_TITLE_LOGO",
		"IMAGE_PRESS_A_BOTTON",

#pragma endregion

#pragma region ゲームシーン

		"IMAGE_GAME_PAD_USER_GUIDE_1",
		"IMAGE_GAME_PAD_USER_GUIDE_2",
		"IMAGE_GAME_PAD_USER_GUIDE_3",
		"IMAGE_GAME_PAD_USER_GUIDE_4",
		"IMAGE_KEYBOARD_USER_GUIDE_1",
		"IMAGE_KEYBOARD_USER_GUIDE_2",
		"IMAGE_KEYBOARD_USER_GUIDE_3",
		"IMAGE_KEYBOARD_USER_GUIDE_4",

#pragma endregion

#pragma endregion

#pragma region モデルとアニメーション

#pragma region プレイヤー

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
		"ANIM_PLAYER_TITLE_IDLE",
#pragma endregion

#pragma region 敵

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

#pragma region ボス

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
		"ANIM_BOSS_JUMP_IDLE",
		"ANIM_BOSS_LAND",
		"ANIM_DEATH",

#pragma endregion

#pragma region ステージ

		"STAGE_MODEL",
		"STAGE_COLLISION_MODEL",
		"STAGE_AREA1_COLLISION_MODEL",
		"STAGE_AREA2_COLLISION_MODEL",
		"STAGE_AREA3_COLLISION_MODEL",
		"STAGE_AREA4_COLLISION_MODEL",
		"STAGE_AREA5_COLLISION_MODEL",

#pragma endregion

#pragma endregion

#pragma region エフェクト

#pragma region プレイヤー

#pragma endregion

#pragma region 敵

#pragma endregion

#pragma region ボス

		"EFFECT_BOSS_PROJECTILE",

#pragma endregion

#pragma endregion

#pragma region BGM・SE

#pragma region タイトルシーン

		"SOUND_TITLE_SCENE_BGM",
		"SOUND_TITLE_SCENE_DECIDE_SE",

#pragma endregion

#pragma region ゲームシーン

		"SOUND_GAME_SCENE_BGM"

#pragma endregion

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

	// 画像のロード
	void ImgLoad(const nlohmann::json_abi_v3_11_3::json& objectData);

	// モデルとアニメーションのロード
	void ModelAndAnimationLoad(const nlohmann::json_abi_v3_11_3::json& objectData);

	// エフェクトのロード
	void EffectLoad(const nlohmann::json_abi_v3_11_3::json& objectData);

	// BGMとSEのロード
	void BGMAndSELoad(const nlohmann::json_abi_v3_11_3::json& objectData);

};