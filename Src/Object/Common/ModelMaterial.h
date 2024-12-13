#pragma once
#include<DxLib.h>
#include<vector>
#include<string>
#include<map>
class ModelMaterial
{
public:

	enum class TEXADDRESS
	{
		NONE,
		WRAP,
		MIRROR,
		CLAMP,
		BORDER,
		MAX,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="shaderFileName"></param>
	/// <param name="constBufFloat4Size"></param>
	ModelMaterial(std::string shaderFileNameVS, int constBufFloat4SizeVS, std::vector<FLOAT4> constBufsVS,
				  std::string shaderFileNamePS, int constBufFloat4SizePS, std::vector<FLOAT4> constBufsPS, std::map<int, int> textures);

	//シェーダハンドルの取得
	int GetShaderVS(void);
	int GetShaderPS(void);

	// 定数バッファサイズの取得
	int GetConstBufSizeVS(void);
	int GetConstBufSizePS(void);

	// 定数バッファハンドルの取得
	int GetconstBufVS(void);
	int GetconstBufPS(void);

	// 定数バッファハンドルのの設定
	void SetConstBufsVS(FLOAT4 constBufs, int idx);
	void SetConstBufsPS(FLOAT4 constBufs, int idx);

	// 定数バッファの取得
	std::vector<FLOAT4> GetConstBufsVS(void);
	std::vector<FLOAT4> GetConstBufsPS(void);

	// 画像の取得
	std::map<int, int> GetTextures(void);

	// アドレスの取得
	TEXADDRESS GetTextureAddress(void);
	void SetTextureAddress(TEXADDRESS texAddress);

private:

	// シェーダハンドル
	int shaderVS_;
	int shaderPS_;

	// 定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	int constBufFloat4SizeVS_;
	int constBufFloat4SizePS_;

	// 定数バッファハンドル
	int constBufVS_;
	int constBufPS_;
	
	// テクスチャアドレス
	TEXADDRESS texAddress_;

	// 定数バッファ
	std::vector<FLOAT4> constBufsVS_;
	std::vector<FLOAT4> constBufsPS_;

	// 画像(スロット、ハンドルID)
	std::map<int,int> textures_;

};

