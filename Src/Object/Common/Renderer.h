#pragma once
#include<DxLib.h>
#include<memory>
#include "../Common/Vector2.h"
class Material;
class ModelMaterial;

class Renderer
{

public:

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

