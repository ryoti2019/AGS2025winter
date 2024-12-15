#include "../Lib/ImGui/imgui.h"
#include "../Application.h"
#include "../Common/Quaternion.h"
#include "../Object/Common/Transform.h"
#include "../Object/Common/ModelMaterial.h"
#include "../Object/Common/Renderer.h"
#include "StageBase.h"

StageBase::StageBase(const VECTOR& pos, const json& data)
	:
	ActorBase(pos,data)
{

	// 衝突判定のモデルID
	//stageCollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_COLLISION_MODEL)]);
	//area1CollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA1_COLLISION_MODEL)]);
	//area2CollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA2_COLLISION_MODEL)]);
	//area3CollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA3_COLLISION_MODEL)]);
	//area4CollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA4_COLLISION_MODEL)]);
	//area5CollisionModelId_ = resMng_.LoadModelDuplicate(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::STAGE_AREA5_COLLISION_MODEL)]);

	// ステージの衝突判定しているかどうか
	//isStageCollision_ = true;
	//isArea1Collision_ = false;
	//isArea2Collision_ = false;
	//isArea3Collision_ = false;
	//isArea4Collision_ = false;
	//isArea5Collision_ = false;

	// モデル制御の基本情報を初期化
	//CollisionInit(stageCollision_,stageCollisionModelId_, isStageCollision_);
	//CollisionInit(area1Collision_, area1CollisionModelId_, isArea1Collision_);
	//CollisionInit(area2Collision_, area2CollisionModelId_, isArea2Collision_);
	//CollisionInit(area3Collision_, area3CollisionModelId_, isArea3Collision_);
	//CollisionInit(area4Collision_, area4CollisionModelId_, isArea4Collision_);
	//CollisionInit(area5Collision_, area5CollisionModelId_, isArea5Collision_);

	// 共通部分は基底クラスで初期化
	//ActorBase::Init(pos);

}

void StageBase::Init(const VECTOR& pos)
{

	// 共通部分は基底クラスで初期化
	ActorBase::Init(pos);

	// 描画用のコンポーネント
	drawComponent_ = std::make_unique<DrawComponent>(std::static_pointer_cast<StageBase>(GetThis()));

}

void StageBase::Update(const float deltaTime)
{
}

void StageBase::Draw(const float deltaTime)
{

	ActorBase::Draw(deltaTime);

}

void StageBase::CollisionInit(std::shared_ptr<Transform>& transform, const int modelId, const bool isCollision)
{

	// テクスチャ
	texId_ = resMng_.Load(resMng_.RESOURCE_KEY[static_cast<int>(ResourceManager::SRC::IMAGE_NOISE_TEXTURE)]).handleId_;

	// モデル描画用
	std::vector<FLOAT4> constBufsPtrVS;
	//constBufsPtrVS.push_back({ 1.0f, 1.0f, 1.0f, 1.0f });

	std::vector<FLOAT4> constBufsPtrPS;
	// 拡散光
	constBufsPtrPS.push_back({ 1.0f, 1.0f, 1.0f, 1.0f });

	// 環境光
	constBufsPtrPS.push_back({ 0.2f, 0.2f, 0.2f, 1.0f });

	// 光の向いている方向(ワールド空間)(ディレクショナルライト)
	auto lDir = GetLightDirection();
	deltaTime_ = 0.0f;
	constBufsPtrPS.push_back({ lDir.x,lDir.y,lDir.z, deltaTime_ });
	alphaTime_ = 0.0f;
	constBufsPtrPS.push_back({ alphaTime_,0.0f,0.0f,0.0f });

	std::map<int, int> textures;
	textures.emplace(1, texId_);
	modelMaterial_ = std::make_shared<ModelMaterial>(
		(Application::PATH_SHADER + "GateModelVS.cso"), sizeof(FLOAT4) * 1, constBufsPtrVS,
		(Application::PATH_SHADER + "GateModelPS.cso"), sizeof(FLOAT4) * 4, constBufsPtrPS, textures
	);

	renderer_ = std::make_shared<Renderer>(transform->modelId, modelMaterial_);

	isDissolve_ = false;

}