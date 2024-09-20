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
		PLAYER,
		PLAYER_IDLE
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