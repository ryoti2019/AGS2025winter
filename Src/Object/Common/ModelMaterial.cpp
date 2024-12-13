#include <DxLib.h>
#include "ModelMaterial.h"

ModelMaterial::ModelMaterial(std::string shaderFileNameVS, int constBufFloat4SizeVS, std::vector<FLOAT4> constBufsVS,
							 std::string shaderFileNamePS, int constBufFloat4SizePS, std::vector<FLOAT4> constBufsPS, std::map<int, int> textures)
{

	// 頂点シェーダ
	shaderVS_ = LoadVertexShader(shaderFileNameVS.c_str());
	constBufVS_ = CreateShaderConstantBuffer(constBufFloat4SizeVS);
	constBufsVS_ = constBufsVS;

	// ピクセルシェーダ
	shaderPS_ = LoadPixelShader(shaderFileNamePS.c_str());
	constBufPS_ = CreateShaderConstantBuffer(constBufFloat4SizePS);
	constBufsPS_ = constBufsPS;
	textures_ = textures;
	texAddress_ = TEXADDRESS::CLAMP;

}

int ModelMaterial::GetShaderVS(void)
{
	return shaderVS_;
}

int ModelMaterial::GetShaderPS(void)
{
	return shaderPS_;
}

int ModelMaterial::GetConstBufSizeVS(void)
{
	return constBufFloat4SizeVS_;
}

int ModelMaterial::GetConstBufSizePS(void)
{
	return constBufFloat4SizePS_;
}

int ModelMaterial::GetconstBufVS(void)
{
	return constBufVS_;
}

int ModelMaterial::GetconstBufPS(void)
{
	return constBufPS_;
}

std::vector<FLOAT4> ModelMaterial::GetConstBufsVS(void)
{
	return constBufsVS_;
}

std::vector<FLOAT4> ModelMaterial::GetConstBufsPS(void)
{
	return constBufsPS_;
}

void ModelMaterial::SetConstBufsVS(FLOAT4 constBufs, int idx)
{
	if (idx > constBufsVS_.size())
	{
		return;
	}
	constBufsVS_[idx] = constBufs;
}

void ModelMaterial::SetConstBufsPS(FLOAT4 constBufs, int idx)
{
	if (idx > constBufsPS_.size())
	{
		return;
	}
	constBufsPS_[idx] = constBufs;
}

std::map<int, int> ModelMaterial::GetTextures(void)
{
	return textures_;
}

ModelMaterial::TEXADDRESS ModelMaterial::GetTextureAddress(void)
{
	return texAddress_;
}

void ModelMaterial::SetTextureAddress(TEXADDRESS texAddress)
{
	texAddress_ = texAddress;
}
