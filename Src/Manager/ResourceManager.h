#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// リソース名
	enum class SRC
	{

#pragma region プレイヤーのアニメーション

		PLAYER,
		PLAYER_IDLE,
		PLAYER_RUN,
		PLAYER_JAB,
		PLAYER_STRAIGHT,
		PLAYER_HOOK,
		PLAYER_LEFT_KICK,
		PLAYER_RIGHT_KICK,
		PLAYER_UPPER,
		PLAYER_CHARGE_PUNCH,
		PLAYER_SPECIAL_PUNCH,
		PLAYER_POWER_CHARGE,
		PLAYER_HIT_HEAD,
		PLAYER_HIT_BODY,
#pragma endregion

#pragma region 敵のアニメーション

		ENEMY,
		ENEMY_IDLE,
		ENEMY_RUN,
		ENEMY_PUNCH,
		ENEMY_KICK,
		ENEMY_HIT_HEAD,
		ENEMY_HIT_BODY,
		ENEMY_HIT_FLY,
		ENEMY_HIT_FLINCH,
		ENEMY_HIT_KNOCK_BACK,
		ENEMY_KIP_UP,
		ENEMY_NORMAL_DEATH,

#pragma endregion

#pragma region ボスのアニメーション

		BOSS,
		BOSS_IDLE,
		BOSS_RUN,
		BOSS_PUNCH,
		BOSS_KICK,
		BOSS_SONIC_BOOM,

#pragma endregion

#pragma region ステージ

		STAGE_MODEL,
		STAGE_COLLISION_MODEL

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
	Resource Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, Resource> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource*> loadedMap_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);

	// デストラクタも同様
	~ResourceManager(void) = default;

	// 内部ロード
	Resource* _Load(SRC src);

};