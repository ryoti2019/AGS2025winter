#pragma once
#include<DxLib.h>
#include<memory>
#include "../Common/Vector2.h"
class Material;
class ModelMaterial;

class Renderer
{

public:

	// コンストラクタ
	Renderer(int modelId, std::shared_ptr<ModelMaterial> modelMaterial);

	// デストラクタ
	~Renderer();

	// 描画矩形の生成
	void MakeSquereVertex(void);

	void SetConstBufsVS(FLOAT4 constBufs, int idx);

	void SetConstBufsPS(FLOAT4 constBufs, int idx);

	// 描画
	void Draw(void);

private:

	// 頂点シェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_VS = 7;

	// ピクセルシェーダ用のオリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

	// 頂点数
	static constexpr int NUM_VERTEX = 4;

	// 頂点インデックス
	static constexpr int NUM_VERTEX_IDX = 6;

	//ポリゴン数
	static constexpr int NUM_POLYGON = 2;

	// テクスチャ座標の中心
	static constexpr float HALF_TEX_COORD = 0.5f;

	// テクスチャ座標の最大値
	static constexpr float FULL_TEX_COORD = 1.0f;

	// インデックスバッファの要素数
	static constexpr int NUM_INDICES = 6;

	// 2つの三角形で描画するためのインデックス情報
	static constexpr int INDEX_SEQUENCE[NUM_INDICES] = { 0, 1, 2, 1, 3, 2 };

	// 色の最大値
	static constexpr int COLOR_MAX = 255;

	// モデルのハンドルID
	int modelId_;

	// 頂点
	VERTEX2DSHADER vertexs_[NUM_VERTEX];

	// 頂点インデックス
	WORD indexes_[NUM_VERTEX_IDX];

	// モデルマテリアル
	std::shared_ptr<ModelMaterial> modelMaterial_;

	int screen_;

	void SetReservePS(void);

	void SetReserveVS(void);

};

