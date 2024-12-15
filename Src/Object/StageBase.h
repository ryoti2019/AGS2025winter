#pragma once
#include "ActorBase.h"

class ModelMaterial;
class Renderer;

class StageBase : public ActorBase
{

public:

	StageBase(const VECTOR& pos, const json& data);

	virtual ~StageBase() = default;

	void Init(const VECTOR& pos)override;

	void Update(const float deltaTime)override;

	virtual void Draw(const float deltaTime)override;

	// 当たり判定の設定
	void SetIsArea1Collision(const bool isCollision) { isStageCollision_ = isCollision; }

	// 当たり判定を取得
	const bool GetIsCollision() { return isStageCollision_; }

	// ディゾルブさせるか設定
	void SetIsDissolve(const bool isDissolve) { isDissolve_ = isDissolve; }

	// どのくらい透過しているか取得
	const float GetAlphaTime() const { return alphaTime_; }

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

	// モデル制御の基本情報を初期化
	void CollisionInit(std::shared_ptr<Transform>& transform, const int modelId, const bool isCollision);

private:

};

