#include "../Manager/SceneManager.h"
#include "../Application.h"
#include "../Common/ModelMaterial.h"
#include "Renderer.h"

Renderer::Renderer(int modelId, std::shared_ptr<ModelMaterial> modelMaterial)
{
	modelId_ = modelId;
	modelMaterial_ = modelMaterial;
}
Renderer::~Renderer()
{
	DeleteShader(modelMaterial_->GetShaderVS());
	DeleteShaderConstantBuffer(modelMaterial_->GetconstBufVS());
	DeleteShader(modelMaterial_->GetShaderPS());
	DeleteShaderConstantBuffer(modelMaterial_->GetconstBufPS());
}

void Renderer::MakeSquereVertex(void)
{

	// 頂点インデックス
	for (auto& v : vertexs_)
	{
		v.dif = GetColorU8(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		v.spc = GetColorU8(0, 0, 0, 0);
		v.rhw = 1.0f;
		v.su = HALF_TEX_COORD;
		v.sv = HALF_TEX_COORD;
	}
	// 左上
	vertexs_[0].pos = { 0.0f, 0.0f, 0.0f };
	vertexs_[0].u = 0.0f;
	vertexs_[0].v = 0.0f;
	// 右上
	vertexs_[1].pos = { Application::SCREEN_SIZE_X, 0.0f, 0.0f };
	vertexs_[1].u = FULL_TEX_COORD;
	vertexs_[1].v = 0.0f;
	// 左下
	vertexs_[2].pos = { 0.0f, Application::SCREEN_SIZE_Y, 0.0f };
	vertexs_[2].u = 0.0f;
	vertexs_[2].v = FULL_TEX_COORD;
	// 右下
	vertexs_[3].pos = { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0.0f };
	vertexs_[3].u = FULL_TEX_COORD;
	vertexs_[3].v = FULL_TEX_COORD;

	// インデックス設定
	for (int i = 0; i < NUM_INDICES; ++i)
	{
		indexes_[i] = INDEX_SEQUENCE[i];
	}

}

void Renderer::Draw(void)
{

	// オリジナルシェーダ設定(ON)
	MV1SetUseOrigShader(true);

	// 頂点シェーダの設定
	SetReserveVS();

	// ピクセルシェーダの設定
	SetReservePS();

	// テクスチャアドレスタイプの取得
	auto texA = modelMaterial_->GetTextureAddress();
	int texAType = static_cast<int>(texA);

	// テクスチャアドレスタイプを変更
	SetTextureAddressModeUV(texAType, texAType);

	// 描画
	MV1DrawModel(modelId_);

	// 後始末
	// テクスチャアドレスタイプを元に戻す
	SetTextureAddressModeUV(DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP);

	// テクスチャ解除
	const auto& textures = modelMaterial_->GetTextures();
	for (const auto& tex : textures)
	{
		SetUseTextureToShader(tex.first,-1);
	}

	// 頂点シェーダ解除
	SetUseVertexShader(-1);

	// ピクセルシェーダ解除
	SetUsePixelShader(-1);

	// オリジナルシェーダ設定(OFF)
	MV1SetUseOrigShader(false);

}

void Renderer::SetReservePS(void)
{
	// シェーダ設定
	SetUsePixelShader(modelMaterial_->GetShaderPS());

	size_t size;

	// テクスチャの設定
	const auto& textures = modelMaterial_->GetTextures();
	size = textures.size();
	if (size == 0)
	{
		// 前回使用分のテクスチャを引き継がないように
		SetUseTextureToShader(0, -1);
	}
	else
	{
		for (auto tex : textures)
		{
			SetUseTextureToShader(tex.first, tex.second);
		}
	}

	// 定数バッファ
	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(modelMaterial_->GetconstBufPS());
	const auto& constBufs = modelMaterial_->GetConstBufsPS();

	size = modelMaterial_->GetConstBufsPS().size();


	// 乗算色
	for (int i = 0; i < size; i++)
	{
		if (i != 0)
		{
			constBufsPtr++;
		}
		constBufsPtr->x = constBufs[i].x;
		constBufsPtr->y = constBufs[i].y;
		constBufsPtr->z = constBufs[i].z;
		constBufsPtr->w = constBufs[i].w;
	}

	// 定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(modelMaterial_->GetconstBufPS());

	// 定数バッファをピクセルシェーダー用定数バッファレジスタに
	SetShaderConstantBuffer(modelMaterial_->GetconstBufPS(), DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

}

void Renderer::SetReserveVS(void)
{
	// 頂点シェーダ設定
	SetUseVertexShader(modelMaterial_->GetShaderVS());

	// 定数バッファ
	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(modelMaterial_->GetconstBufVS());
	const auto& constBufs = modelMaterial_->GetConstBufsVS();

	size_t size = modelMaterial_->GetConstBufsVS().size();

	// 乗算色
	for (int i = 0; i < size; i++)
	{
		if (i != 0)
		{
			constBufsPtr++;
		}
		constBufsPtr->x = constBufs[i].x;
		constBufsPtr->y = constBufs[i].y;
		constBufsPtr->z = constBufs[i].z;
		constBufsPtr->w = constBufs[i].w;
	}

	// 定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(modelMaterial_->GetconstBufVS());

	// 定数バッファをピクセルシェーダー用定数バッファレジスタに
	SetShaderConstantBuffer(modelMaterial_->GetconstBufVS(), DX_SHADERTYPE_VERTEX, CONSTANT_BUF_SLOT_BEGIN_VS);
}
