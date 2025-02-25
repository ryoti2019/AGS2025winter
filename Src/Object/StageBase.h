#pragma once
#include "ActorBase.h"

class ModelMaterial;
class Renderer;

class StageBase : public ActorBase
{

public:

	StageBase(const VECTOR& pos, const json& data);

	//virtual ~StageBase() = default;
	virtual ~StageBase() = default;

	void Init(const VECTOR& pos)override;

	void Update(const float deltaTime)override;

	virtual void Draw(const float deltaTime)override;

	// 衝突の壁の描画
	void CollisionDraw(const float deltaTime);

	// 当たり判定の設定
	void SetIsCollision(const bool isCollision) { isStageCollision_ = isCollision; }

	// 当たり判定を取得
	const bool GetIsCollision() { return isStageCollision_; }

	// ディゾルブさせるか設定
	const bool GetIsDissolve()const { return isDissolve_; }

	// ディゾルブさせるか設定
	void SetIsDissolve(const bool isDissolve) { isDissolve_ = isDissolve; }

	// どのくらい透過しているか取得
	const float GetAlphaTime() const { return alphaTime_; }

	// 制限エリアとの当たり判定を出す座標を取得
	const VECTOR GetRestrictedAreaPos() const { return restrictedAreaPos_; }

	// 制限エリアとの当たり判定を出す半径を取得
	const float GetRestrictedAreaRadius()const { return restrictedAreaRadius_; }

	// 制限エリアかの判定を取得
	const bool GetIsRestrictedArea()const { return isRestrictedArea_; }

	// 制限エリアかの判定を取得
	void SetIsRestrictedArea(const bool isRestrictedArea) { isRestrictedArea_ = isRestrictedArea; }

protected:

	// ステージのコリジョンモデル
	std::unique_ptr<Transform> stageCollision_;

	// モデルマテリアル
	std::shared_ptr<ModelMaterial> modelMaterial_;

	// レンダラー
	std::shared_ptr<Renderer> renderer_;

	// ステージモデルID
	int stageCollisionModelId_;

	// ステージの衝突判定しているかどうか
	bool isStageCollision_;

	// テクスチャ
	int texId_;

	// デルタタイム
	float deltaTime_;

	// 透過用のタイム
	float alphaTime_;

	// ディゾルブさせるかどうか
	bool isDissolve_;

	// 制限エリアとの当たり判定を出す座標
	VECTOR restrictedAreaPos_;

	// 制限エリアとの当たり判定を出す半径
	float restrictedAreaRadius_;
	
	// 制限エリアかの判定
	bool isRestrictedArea_;

	// モデル制御の基本情報を初期化
	void CollisionInit(std::shared_ptr<Transform>& transform);

private:

	// アルファ値の増加量
	static constexpr float IMG1_ALPHA_INCREMENT = 0.005f;

	// ピクセルシェーダーの定数バッファスロット2
	static constexpr int PS_CONST_BUF_SLOT_2 = 2;

	// ピクセルシェーダーの定数バッファスロット3
	static constexpr int PS_CONST_BUF_SLOT_3 = 3;

	// 拡散光の強さ
	static constexpr FLOAT4 DIFFUSE_LIGHT = { 1.0f,1.0f,1.0f,1.0f };

	// 環境光の強さ
	static constexpr FLOAT4 AMBIENT_LIGHT = { 0.2f,0.2f,0.2f,1.0f };

	// テクスチャスロット番号
	static constexpr int TEXTURE_SLOT_DIFFUSE = 1;

	// 頂点シェーダーの定数バッファサイズ
	static constexpr size_t VS_CONST_BUF_SIZE = sizeof(FLOAT4) * 1;

	// 頂点シェーダーの定数バッファサイズ
	static constexpr size_t PS_CONST_BUF_SIZE = sizeof(FLOAT4) * 4;

};

